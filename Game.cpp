#include "Game.h"

#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::exception;
using std::regex;
using std::regex_match;

Game::Game() {
    //Building tile bag
    bag = new TileBag();
    bag->buildBag();
    gameBoard = new GameBoard();
}

Game::Game(ifstream& gameFile) {
    bag = new TileBag();
    gameBoard = new GameBoard();
    Player *player = new Player();
    Player *player2 = new Player();
    gameFile >> *player;
    gameFile >> *player2;
    gameFile >> *gameBoard;
    gameFile >> *bag;
    string currentPlayer;
    getline(gameFile, currentPlayer);

    if (currentPlayer == player->getName()) {
        players.push_back(player);
        players.push_back(player2);
    } else {
        players.push_back(player2);
        players.push_back(player);
    }


}

Game::~Game() {
    delete bag;

    for (Player *player: players) {
        delete player;
    }
}

void Game::startGameLoop() {
    //  game start, get first two players

    std::string input;

    bool playersMade = makePlayers();


    if (playersMade) {
        // Game start

            mainGameLoop();
        }
    }



bool Game::makePlayers() {
    string input;
    bool playersMade = false;
    int madePlayers = 0;
    regex playerNameExpr("[A-Z]+");

    while (madePlayers < 2 && !cin.eof()) {
        cout << "Enter a name for player " << (madePlayers + 1)
             << " (uppercase characters only)" << endl;

        cout << "> ";
        std::getline(cin, input);

        if (regex_match(input, playerNameExpr)) {
            Player *player = new Player(input);
            player->setPlayerHand(bag);
            players.push_back(player);
            madePlayers++;
        }
    }

    playersMade = true;

    return playersMade;
}

void Game::saveGame(string currPlayerName, string filename) {
    ofstream outfile(filename);
    if (!outfile) {
        cout << "Error saving file";
    } else {
        for (Player *player: players) {
            // //Saving each players name, score and hand
            outfile << *player;
        }
        //Saving board state
        outfile << *gameBoard;

        //Saving tile bag state
        outfile << *bag;

        //Saving current players name
        outfile << currPlayerName << endl;
        outfile.close();
        cout << "Game saved to " << filename << endl;
    }
}

void Game::quitGame() {
    /*
    TODO: Check if this is properly implemented
    std exit does not perform any cleanup
    unsure if calling game destructor is enough, need to check back on this!
    */

    delete bag;

    for(Player* p: players) {
        delete p;
    }
}

void Game::handlePlayerTurn(Player *player) {
    string input;
    cout << player->getName() << ", it's your turn" << endl;

    for (Player *p: players) {
        cout << "Score for " << p->getName() << ": " << p->getScore()
             << endl;
    }

    cout << *gameBoard;

    cout << "Your hand is\n" << *player->getHand();

    bool turnEnd = false;

    // auto pointers to remove need to free memory
    auto *placedTiles = new vector<Tile *>;
    auto *placedTilesPositions = new vector<string>;

    // direction is set once the second tile is placed.
    // horizontal = 0; vertical = 1;

    for (int i = 0; i < 7 && !turnEnd; i++) {
        cout << "> ";
        getline(cin, input);

        regex place_expr("^[place]+[ ][A-Z][ ][at]+[ ][A-Z]([0-9]+)");
        regex placeDoneExpr("^[place]+[\\s]([D][one]+)");
        regex saveGameExpr("save [A-Za-z\\d\\-_.]+");

//        Tile *placedTile = nullptr;

        /*
         * This if chain checks the input and executes the relevant action,
         * e.g. "place", "replace", etc.
         */
        if (regex_match(input, place_expr)) {

            // TODO: implement the scrabble "horizontal" or "vertical" rule

            handlePlace(player, input, placedTiles, placedTilesPositions);
        } else if (regex_match(input, placeDoneExpr)) {
            turnEnd = true;

            player->addScore(gameBoard->computeTurnScore(*placedTiles,
                                                         *placedTilesPositions));

        } else if(regex_match(input, saveGameExpr)) {
            string filename = input.substr(4);
            saveGame(player->getName(), filename);
        } else if (cin.eof()) {
            turnEnd = true;
        } else {
            cout << "Invalid action provided. Please try again" << endl;
        }

    }
}

void
Game::handlePlace(Player *player, string input, vector<Tile *> *placedTiles,
                  vector<string> *positions) {
    bool tilePlaced = false;

    vector<string> tilePosVec = splitPlaceCommand(input);

    if (!tilePosVec.empty()) {

        char tileLetter = tilePosVec.at(0)[0];
        string position = tilePosVec.at(1);

        if (player->hasTileLetter(tileLetter)) {
            auto toPlace = player->getTileFromLetter(tileLetter);

            if (gameBoard->placeTile(toPlace, position)) {
                tilePlaced = true;
                placedTiles->push_back(new Tile(*toPlace));
                positions->push_back(position);
            }
        }

    }

    if (!tilePlaced) {
        cout << "Unable to place tile at that position." << endl;
    }

}


//Adding scores
void Game::addScores(Player player, int scoreToAdd) {
    player.addScore(scoreToAdd);
}

void Game::startGameFromLoad() {
    mainGameLoop();
}

void Game::mainGameLoop() {
    cout << "Let's Play!" << endl;

    bool gameOver = false;
    while (!gameOver && !std::cin.eof()) {
        // loop through each player, handle their turns.
        // may have to split some logic into functions.
        for (Player *player: players) {

            /*TODO: HANDLE PLAYER ACTIONS HERE;
             * E.g.:
             *    - handle one of [place tile,
             *      replace tile,
             *      pass, quit, save]
             *    - for place use a while loop.
            */

            handlePlayerTurn(player);

            //TODO: CHECK SCORES AND PASSES AT THE END OF THE PLAYER FUNCTIONS (Assuming they don't quit)


        }
    }
}
//
//int Game::scoreCheck(int direction, string initialCoord) {
//    int totalTurnScore = 0;
//
//    //setting start and end coords
//    setStartEndCoord(initialCoord, direction);
//
//    if (direction == 1) {
//        totalTurnScore += calcHori(initialCoord);
//        totalTurnScore += loopVerti(startCoord, endCoord)
//    } else {
//        totalTurnScore += calcVerti(initialCoord);
//        totalTurnScore += loopHori(startCoord, endCoord)
//    }
//
//    return totalTurnScore;
//}

//void Game::setStartEndCoord(string base, int direction) {
//    string curr = base;
//    //TODO: figure out a way to cycle left and right/ up and down
//
//    if (direction == 1) {
//        while (curr.left != NULL) {
//            curr = curr.left;
//        }
//        startCoord = curr;
//
//        //checking right side
//        curr = base;
//        while (base.right != NULL) {
//            curr = curr.right;
//        }
//        endCoord = curr;
//    } else {
//        while (curr.up != NULL) {
//            curr = curr.up;
//        }
//        startCoord = curr;
//
//        //checking right side
//        curr = base;
//        while (base.down != NULL) {
//            curr = curr.down;
//        }
//        endCoord = curr;
//    }
//}

//int Game::calcHori(string base) {
//    int tempScore = 0;
//    string curr = base;
//
//    //checking left side
//    while (curr.left != null) {
//        curr = curr.left;
//        tempScore += curr.tile.score;
//    }
//
//    //checking right side
//    curr = base;
//    while (base.right != null) {
//        curr = curr.right;
//        tempScore += curr.tile.score;
//    }
//
//    return tempScore;
//}
//
//int Game::calcVerti(string base) {
//    int tempScore = 0;
//    string curr = base;
//
//    //checking upwards
//    while (curr.up != null) {
//        curr = curr.up;
//        tempScore += curr.tile.score;
//    }
//
//    //checking downwards
//    curr = base;
//    while (base.down != null) {
//        curr = curr.down;
//        tempScore += curr.tile.score;
//    }
//
//    return tempScore;
//}
//
//int Game::loopHori(string startCoord, string endCoord) {
//    int tempScore = 0;
//
//    //creating a list of coordinates to loop through
//    string col = startCoord.letter;
//    vector<string> coordList;
//    for (int i = startCoord.num, i<startCoord.num - endCoord.num, i++) {
//        string tempCoord = col + i;
//        coordList.push_back(tempCoord);
//    }
//
//    //looping through the list
//    for (auto &currCoord: coordList) {
//        if (currCoord.up is
//        not Null || currCoord.down
//        is
//        not Null){
//            tempScore += calcVerti(currCoord);
//        }
//    }
//
//    return tempScore;
//}
//
//int Game::loopVerti(string startCoord, string endCoord) {
//    int tempScore = 0;
//
//    //creating a list of coordinates to loop through
//    string row = startCoord.num;
//    vector<string> coordList;
//    for (int i = startCoord.letter.val, i<
//            startCoord.letter.val - endCoord.letter.val, i++) {
//        string tempCoord = i.toString + row;
//        coordList.push_back(tempCoord);
//    }
//
//    //looping through the list
//    for (auto &currCoord: coordList) {
//        if (currCoord.left is
//        not Null || currCoord.right
//        is
//        not Null){
//            tempScore += calcVerti(currCoord);
//        }
//    }
//
//    return tempScore;
//}