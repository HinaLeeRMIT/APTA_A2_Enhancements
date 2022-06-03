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

    //setting valid words
    setValidWords();
}

Game::Game(ifstream& gameFile) {
    bag = new TileBag();
    gameBoard = new GameBoard();
    int numPlayers = 0;

    gameFile >> numPlayers;
    gameFile.ignore();

    for(int i = 0; i < numPlayers; i++){
        Player *player = new Player();

        gameFile >> *player;
        players.push_back(player);
    }

    gameFile >> *gameBoard;
    gameFile >> *bag;
    gameFile >> loadedCurrPlayer;
    gameFile.ignore();

    //setting valid words
    setValidWords();
}

Game::~Game() {
    delete bag;

    for (Player *player: players) {
        delete player;
    }
}

bool Game::makePlayers(int numPlayers) {
    string input;
    bool playersMade = false;
    int madePlayers = 0;
    regex playerNameExpr("[A-Z]+");

    while (madePlayers < numPlayers && !cin.eof()) {
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

void Game::setValidWords(){
    //setting list of valid words to check against
    ifstream inFile("validWords");

    //checking if file loaded properly
    if(!inFile.fail()){
        //looping while not EOF
        while(inFile.peek()!=EOF){
            string validWord = "";
            getline(inFile, validWord);
            validWords.push_back(validWord);
        }
    }
}

void Game::startGameLoop(int numPlayers) {
    bool playersMade = makePlayers(numPlayers);


    if (playersMade) {
        // Game start
            mainGameLoop();
        }
    }

void Game::startGameFromLoad() {
    cout << endl;
    cout << "Let's Continue playing!" << endl;
    cout << "If you have any trouble, feel free to type 'help'!" << endl;
    //first finish current cycle of turns 
    bool turnCycleDone = false;
    while(!gameEnd && !std::cin.eof() && !turnCycleDone){
        // loop through each player, handle their turns.
        for (int i = loadedCurrPlayer; i < int(players.size()) && !gameEnd; i++){
            Player *player = players[i];
            handlePlayerTurn(player, i);
        }
        turnCycleDone = true;
    }

    while (!gameEnd && !std::cin.eof()) {
        // loop through each player, handle their turns.
        for (int i = 0; i < int(players.size()) && !gameEnd; i++){
            Player *player = players[i];
            handlePlayerTurn(player, i);
        }
    }
    cout << "\nThank you for playing, See you next time!\n";
}

void Game::mainGameLoop() {
    cout << endl;
    cout << "Let's Play!" << endl;
    cout << "If you have any trouble, feel free to type 'help'!" << endl;

    while (!gameEnd && !std::cin.eof()) {
        // loop through each player, handle their turns.
        for (int i = 0; i < int(players.size()) && !gameEnd; i++){
            Player *player = players[i];
            handlePlayerTurn(player, i);
        }
    }
    cout << "\nThank you for playing, See you next time!\n";
}

void Game::handlePlayerTurn(Player *player, int playerIndex) {
    //creating tempHand to store tiles used in case invalid turn, where tiles will be placed back into players hand
    LinkedList *tempHand = new LinkedList;

    string tempWord = "";
    string input;
    cout << endl;
    cout << player->getName() << ", it's your turn!" << endl;

    for (Player *p: players) {
        cout << "Score for " << p->getName() << ": " << p->getScore()
             << endl;
    }

    cout << *gameBoard;

    cout << "\nYour hand is\n" << *player->getHand();

    bool turnEnd = false;

    while (!turnEnd) {
        cout << endl;
        cout << "What would you like to do?" << endl;
        cout << "> ";
        getline(cin, input);

        regex placeTileExpr("^[place]+[ ][A-Z][ ][at]+[ ][A-Z]([0-9]+)");
        regex placeDoneExpr("^[place]+[\\s]([D][one]+)");
        regex replaceTileExpr("replace [A-Z]");
        regex passTurnExpr("pass");
        regex helpExpr("help");
        regex saveGameExpr("save [A-Za-z\\d\\-_.]+");
        regex quitGameExpr("Quit");

        /*
         * This if chain checks the input and executes the relevant action,
         * e.g. "place", "replace", etc.
         */
        if (regex_match(input, placeTileExpr)) {
            string placement = extractPlacementLocation(input);

            handlePlaceTile(tempHand, player, placement);

        } else if (regex_match(input, placeDoneExpr)) {
            bool validPlacement = okPlacementCheck(player); 
            if (validPlacement){
                turnEnd = true;
                drawNewTiles(player);
                placeTiles(tempHand);
                player->addScore(calcPlacedScore(tempHand));
            }else{
                reimburseTiles(player, tempHand);
                emptyPlacedTiles();
                cout << endl;
                cout << "Your current hand is: " << *player->getHand() << endl;
            }

            delete tempHand;
            tempHand = new LinkedList;
            placementHistory.clear();
            rowColCheck = "";

        } else if(regex_match(input, replaceTileExpr)){
            //getting the char and setting to a char
            string stringTileLetter = input.substr(8);
            char tileLetter = stringTileLetter[0];

            Tile *tileToReplace = bag->drawTile();
            //checking if the tile exists, if so replace it and print out new hand
            if(player->hasTileLetter(tileLetter)){
                player->replaceTile(player->getTileFromLetter(tileLetter), tileToReplace);
                cout << endl;
                cout<< "The new tile you recieved is " << tileToReplace->letter << "-" << tileToReplace->value << endl;
                cout << "\nYour new hand is\n" << *player->getHand();
                turnEnd = true;
                delete tempHand;
                placementHistory.clear();
                rowColCheck = "";
            }
            else{
                cout << "You don't have this tile to replace!" << endl;
            }
        } else if(regex_match(input, passTurnExpr)){
            cout << "Turn passed!\n";
            turnEnd = true;
            delete tempHand;
            placementHistory.clear();
            rowColCheck = "";
        } else if(regex_match(input, helpExpr)){
            cout << "List of commands!\n";
            cout << "---------------------------------------------------------" << std::endl;
            cout << "'place [letter] at [Row letter + Col number]'" << endl;
            cout << "       Pends chosen tile to be placed at chosen coordinate." << endl;
            cout << "       Can continuously place tiles until no more tiles left or 'place Done' command entered." << endl;
            cout << "       Example: 'place S at D5'" << endl;
            cout << endl;
            cout << "'place Done'" << endl;
            cout << "       Finishes turn and all pending tiles will be placed if word is valid." << endl;
            cout << "       Example: 'place S at D5'" << endl;
            cout << "                'place A at D6'" << endl;
            cout << "                'place D at D7'" << endl;
            cout << "                'place S at D5'" << endl;
            cout << "                'place Done'" << endl;
            cout << endl;
            cout << "'replace [letter]'" << endl;
            cout << "       Replaces a letter current player chooses if they own that corresponding tile." << endl;
            cout << "They will be given a new tile." << endl;
            cout << "       Example: 'replace A'" << endl;
            cout << endl;
            cout << "'pass'" << endl;
            cout << "       Current player passes turn. Turn will be passed to next player." << endl;
            cout << "       Example: 'pass'" << endl;
            cout << endl;
            cout << "'save [file name]" << endl;
            cout << "       Saved current game as a file at desired file name location." << endl;
            cout << "       If file does not exist, it will create a new file." << endl;
            cout << "       Example: 'save savedGame'" << endl;
            cout << "             or 'save savedGame'" << endl;
            cout << endl;
            cout << "'Quit'" << endl;
            cout << "       Ends the current game. Program will terminate." << endl;
            cout << "       Example: 'Quit'" << endl;
            cout << "---------------------------------------------------------" << std::endl;
            cout << endl;



        } else if(regex_match(input, saveGameExpr)) {
            string filename = input.substr(4);
            saveGame(playerIndex, filename);
        } else if(regex_match(input, quitGameExpr)) {
            turnEnd = true;
            gameEnd = true;
        } else if (cin.eof()) {
            turnEnd = true;
        } else {
            cout << "Invalid action provided. Please try again" << endl;
        }
    }
}

void Game::drawNewTiles(Player *player){
    while(player->getHandSize() < 7 || bag->getSize() == 0){
        Tile *newTile = bag->drawTile();
        player->addTile(newTile);
    }
}

void Game::reimburseTiles(Player *player, LinkedList *tempHand){
    for(int i = 0; i < tempHand->getLength(); i++){
        player->addTile(tempHand->get(i));
    }
}

bool Game::okPlacementCheck(Player *player){
    bool validTurn = false;
    bool horiNull = false;
    bool vertiNull = false;
    vector<int> tempCoordValidation;

    std::array<int, 2> initialPos = convertPosTokenToInt(placementHistory.front());
    std::array<int, 2> finalPos = convertPosTokenToInt(placementHistory.back());

    int initRow = initialPos.at(0);
    int initCol = initialPos.at(1);
    int finalRow = finalPos.at(0);
    int finalCol = finalPos.at(1);

    //checking Hori
    if ((finalCol - initCol) > 1){
        horiNull = checkHoriWord(initCol, finalCol, finalRow);
    }
    //checking Verti
    if ((finalRow - initRow) > 1){
        vertiNull = checkVertiWord(initRow, finalRow, finalCol);
    }
    
    //if word is valid
    if(!horiNull && !vertiNull){
        cout << "Valid word!" << endl;
        //Bingo case, add points here
        if(placementHistory.size() == 7){
            cout <<"BINGO!!! (bonus 50 points)" << endl;
            player->addScore(50);
        }
        validTurn = true;

    }else{
        cout << "Invalid word, please try again!" << endl;
    }

    return validTurn;
}

bool Game::checkHoriWord(int currCol, int finalCol, int row){
    bool foundNull = false;
    vector<int> tempCoordValidation;

        while (currCol < finalCol){
            //adding the coordinates in between placed tiles to check against the board
            bool coordNotInside = true;
            string tempCoordinate = std::to_string(row) + std::to_string(currCol);
            for (int i = 0; i < int(placementHistory.size()); i++){
                std::array<int, 2> currPosArr = convertPosTokenToInt(placementHistory.at(i));
                string currPos = std::to_string(currPosArr.at(0)) + std::to_string(currPosArr.at(1));
                if (currPos == tempCoordinate){
                    coordNotInside = false;
                }
            }

            if(coordNotInside){
                tempCoordValidation.push_back(currCol);
            }
            currCol++;
        }

        //validating in bewtween tiles with the board
        for(int col : tempCoordValidation){
            if (!gameBoard->spaceIsFree(row, col)){
            }else{
                cout << "found error hori" << endl;
                foundNull = true;
            }
        }

    return foundNull;
}

bool Game::checkVertiWord(int currRow, int finalRow, int col){
    bool foundNull = false;
    vector<int> tempCoordValidation;

        while (currRow < finalRow){
            //adding the coordinates in between placed tiles to check against the board
            bool coordNotInside = true;
            string tempCoordinate = std::to_string(currRow) + std::to_string(col);
            for (int i = 0; i < int(placementHistory.size()); i++){
                std::array<int, 2> currPosArr = convertPosTokenToInt(placementHistory.at(i));
                string currPos = std::to_string(currPosArr.at(0)) + std::to_string(currPosArr.at(1));
                if (currPos == tempCoordinate){
                    coordNotInside = false;
                }
            }

            if(coordNotInside){
                tempCoordValidation.push_back(currRow);
            }
            currRow++;
        }

        //validating in bewtween tiles with the board
        for(int row : tempCoordValidation){
            if (!gameBoard->spaceIsFree(row, col)){
            }else{
                cout << "found error verti" << endl;
                foundNull = true;
            }
        }

    return foundNull;
}

int Game::calcPlacedScore(LinkedList *tempHand){
    int scoreToAdd = 0;
    for(int i = 0; i < tempHand->getLength(); i++){
        scoreToAdd += tempHand->get(i)->value;
    }
    return scoreToAdd;
}

int Game::calcScore(){
    int scoreToAdd = 0;
    string tempPosition = placementHistory.at(0);
    std::array<int, 2> rowCol = convertPosTokenToInt(placementHistory[0]);
    int row = rowCol.at(0);
    int col = rowCol.at(1);

    //Checking if there is onyl one placed
    if (int(placementHistory.size()) == 1){
        cout << "calculating single positional score " << endl;

        scoreToAdd += horiScore(row, col);
        scoreToAdd += vertiScore(row, col);
    }
    //otherwise multiple tiles placed
    else{
        char orientation = rowColCheck.at(0);
        
        //if orientation is horizontal for placement
        if (orientation == tempPosition.at(0)){
            cout << "calculating hori positional score " << endl;
            cout << row << " " << col << endl;
            scoreToAdd += horiScore(row, col);

            for(string position : placementHistory){
                std::array<int, 2> rowCol = convertPosTokenToInt(placementHistory[0]);
                char newPositionCol = rowCol.at(1);

                scoreToAdd += vertiScore(row, newPositionCol);
            }
        }

        //if orientation is vertical for placement
        else{
            cout << "calculating verti positional score " << endl;

            scoreToAdd += vertiScore(row, col);

            for(string position : placementHistory){
                std::array<int, 2> rowCol = convertPosTokenToInt(placementHistory[0]);
                char newPositionRow = rowCol.at(0);

                scoreToAdd += horiScore(newPositionRow, col);
            }
        }
        
    }

    cout << "Congratulations! You earned " << scoreToAdd << " points!" << endl;
    return scoreToAdd;
}

int Game::horiScore(int currRow, int col){
    cout << "calculating verti score" << endl;
    int scoreToAdd = 0;

    if(gameBoard->retrieveTile(currRow-1, col) != nullptr && gameBoard->retrieveTile(currRow + 1, col) != nullptr){
        //scan all the way left, until nullptr, then check whole word right
        while(gameBoard->retrieveTile(currRow, col) != nullptr && currRow > 0){
            cout << "currRow " << currRow << endl;
            currRow--;
        }

        while(gameBoard->retrieveTile(currRow, col) != nullptr && currRow < gameBoard->getBoardSize()){
            cout << "adding value for " << gameBoard->retrieveTile(currRow, col)->letter << endl;

            scoreToAdd += gameBoard->retrieveTile(currRow, col)->value;
            currRow++;
        }
    }
    return scoreToAdd;
}

int Game::vertiScore(int row, int currCol){
    cout << "calculating hori score" << endl;
    int scoreToAdd = 0;

    if(gameBoard->retrieveTile(row, currCol - 1) != nullptr && gameBoard->retrieveTile(row, currCol + 1) != nullptr){
        //scan all the way to top, until nullptr, then check whole word down
        while(gameBoard->retrieveTile(row, currCol) != nullptr && currCol > 0){
                    cout << "currRow " << currCol << endl;

            currCol--;
        }

        while(gameBoard->retrieveTile(row, currCol) != nullptr && currCol < gameBoard->getBoardSize()){
            cout << "adding value for " << gameBoard->retrieveTile(row, currCol)->letter << endl;

            scoreToAdd += gameBoard->retrieveTile(row, currCol)->value;
            currCol++;
        }
    }
    return scoreToAdd;
}

void Game::emptyPlacedTiles(){
    for (int i = 0; i < int(placementHistory.size()); i++){
        std::array<int, 2> rowCol = convertPosTokenToInt(placementHistory[i]);
        int row = rowCol.at(0);
        int col = rowCol.at(1);
        gameBoard->placeTile(nullptr, row, col);
    }
}

void Game::placeTiles(LinkedList *tempHand){
    for(int i = 0; i < int(placementHistory.size()); i++){
        std::array<int, 2> rowCol = convertPosTokenToInt(placementHistory.at(i));
        int row = rowCol.at(0);
        int col = rowCol.at(1);
        gameBoard->placeTile(tempHand->get(i), row, col);
    }
}

string Game::extractPlacementLocation(string input){
    regex tileExpr("[\\s][A-Z][\\s]");
    regex posExpr("[A-Z]([0-9]+)");
    string placement = "";

    // extract the tile token, e.g. "A", "C"... etc.
    regex_string_iterator tileIterator(input.begin(), input.end(), tileExpr);
    string tileLetter = tileIterator->str();
    // remove whitespaces
    tileLetter.erase(remove_if(tileLetter.begin(), tileLetter.end(),
                              ::isspace), tileLetter.end());

    // extract the position token, e.g. "A1", "B0"
    regex_string_iterator posIterator(input.begin(), input.end(), posExpr);
    string placementPos = posIterator->str();


    if(!tileLetter.empty() && !placementPos.empty()) {
        placement = tileLetter + placementPos;
    }

    return placement;
}

void Game::handlePlaceTile(LinkedList *tempHand, Player *player, string placement){
    char tileLetter = placement[0];
    string position = placement.substr(1);

    //check for row or col integrity.
    if (tempHand->getLength() == 0) {
        if(!checkValidPlacement(tempHand, player, tileLetter, position)){
            cout << "Invalid placement, please try again!" << endl;
            cout << endl;
            cout << "Your current hand is: " << *player->getHand() << endl;
        }else{placementHistory.push_back(position); rowColCheck = position;}
    }
    else{
        if(!checkValidOrientation(position) || !checkValidPlacement(tempHand, player, tileLetter, position)){
            cout << "Invalid placement, please try again!" << endl;
            cout << "Your current hand is: " << *player->getHand() << endl;
        }else{placementHistory.push_back(position);}
    }
}

bool Game::checkValidPlacement(LinkedList *tempHand, Player *player, char tileLetter, string position){
    bool validPlacement = false;

    std::array<int, 2> rowCol = convertPosTokenToInt(position);
    int row = rowCol.at(0);
    int col = rowCol.at(1);

    //first check player has tile
    if (!player->hasTileLetter(tileLetter)) {
        cout << "You do not have this tile to place!" << endl;
    }else{
        bool tilePositionNotTaken = false;
        tilePositionNotTaken = checkTempPositionNotTaken(position);
        //second check if gameboard placement is valid
        if ((gameBoard->spaceIsFree(row, col)) && (tilePositionNotTaken)) {
            validPlacement = true;
            tempHand->addBack(player->getTileFromLetter(tileLetter));    
            player->removeTile(tileLetter);                  
        }else{
            cout << "This tile placement position is invalid!" << endl;
        }
    }
    return validPlacement;
}

bool Game::checkTempPositionNotTaken(string position){
    bool positionNotTaken = true;
    for(int i = 0; i < int(placementHistory.size()); i++){
        if(position == placementHistory[i]){
            positionNotTaken = false;
        }
    }
    
    return positionNotTaken;
}

bool Game::checkValidOrientation(string position){
    bool validOrientation = false;
    char placementRow = position.at(0);
    char placementCol = position.at(1);
    if (rowColCheck.size() == 2){
        //checking for size 2
        char checkerRow = rowColCheck.at(0);
        char checkerCol = rowColCheck.at(1);
        if (checkerRow == placementRow){
            rowColCheck = placementRow;
            validOrientation = true;
        }else if (checkerCol == placementCol){
            rowColCheck = placementCol;
            validOrientation = true;
        }

    }else{
        //checking for size 1
        char checker = rowColCheck.at(0);
        if ((checker == placementRow) || (checker == placementCol)){
            validOrientation = true;
        }
    }
    return validOrientation;
}

//personal  enhancement version
// void Game::handleFinalPlacement(){
//     bool validHoriWord = false;
//     bool validVertiWord = false;

//     vector<Tile *> horiWord;
//     string finalHoriWord = "";
//     int finalHoriPoints = 0;
//     vector<Tile *> vertiWord;
//     string finalVertiWord = "";
//     int finalVertiPoints = 0;

//     int boardSize = gameBoard->getBoardSize();
//     std::array<int, 2> rowCol = convertPosTokenToInt(placementHistory.at(0));
//     int initRow = rowCol.at(0);
//     int initCol = rowCol.at(1);

//     //checking horizontal final word and validity
//     horiWord = scanHoriWord(initRow, initCol, boardSize);
//     for(int i = 0; i < horiWord.size(); i++){
//         finalHoriWord = finalHoriWord + horiWord.at(i)->letter;
//     }
    
//     //checking vertical final word and validity
//     vertiWord = scanVertiWord(initRow, initCol, boardSize);
//     for(int i = 0; i < horiWord.size(); i++){
//         finalVertiWord = finalVertiWord + vertiWord.at(i)->letter;
//     }

//     if (((finalHoriWord.size() == 1) || handleValidWord(finalHoriWord)) && ((finalVertiWord.size() == 1) || handleValidWord(finalVertiWord))){
//         cout << "VALID WORDS TIME TO IMPLEMENT ADD POINTS";
//     }

//     else{
//         cout << "invalid words!";
//     }
// }

// vector<Tile *> Game::scanHoriWord(int row, int col, int boardSize){
//     vector<Tile *> word;
//     bool foundNull = false;

//     //checking left side
//     while(row != 0 || !foundNull){
//         if (!gameBoard->spaceIsFree(row, col)){
//             row--;
//         }else{
//             foundNull = true;
//         }
//     }

//     //scanning all the way to the right
//     //resetting foundNull
//     foundNull = false;
//     while(row != boardSize || !foundNull){
//         if (!gameBoard->spaceIsFree(row, col)){
//             word.push_back(gameBoard->retrieveTile(row, col));
//             row++;
//         }else{
//             foundNull = true;
//         }
//     }
//     return word;
// }

vector<Tile *> Game::scanHoriWord(int row, int col, int boardSize){
    vector<Tile *> word;
    bool foundNull = false;

    //checking left side
    while(col != 0 || !foundNull){
        if (!gameBoard->spaceIsFree(row, col)){
            col--;
        }else{
            foundNull = true;
        }
    }

    //scanning all the way to the right
    //resetting foundNull
    foundNull = false;
    while(col != boardSize || !foundNull){
        if (!gameBoard->spaceIsFree(row, col)){
            word.push_back(gameBoard->retrieveTile(row, col));
            col++;
        }else{
            foundNull = true;
        }
    }
    return word;
}

// string Game::handlePlace(Player *player, string input, string tempWord, vector<Tile *> *placedTiles,
//                   vector<string> *positions) {
//     bool tilePlaced = false;

//     vector<string> tilePosVec = splitPlaceCommand(input);
//     tempWord = tempWord + tilePosVec.at(0)[0];

//     if (!tilePosVec.empty()) {

//         char tileLetter = tilePosVec.at(0)[0];
//         string position = tilePosVec.at(1);

//         if (player->hasTileLetter(tileLetter)) {
//             auto toPlace = player->getTileFromLetter(tileLetter);

//             if (gameBoard->placeTile(toPlace, position)) {
//                 tilePlaced = true;
//                 placedTiles->push_back(new Tile(*toPlace));
//                 positions->push_back(position);
//             }
//         }
//     }

//     if (!tilePlaced) {
//         cout << "Unable to place tile at that position." << endl;
//     }

//     return tempWord;
// }

bool Game::handleValidWord(string tempWord){
    bool validWord = false;

    cout << endl;
    cout << tempWord << endl;

    if(find(validWords.begin(), validWords.end(), tempWord) != validWords.end()){
        validWord = true;
    }
    else{
        cout << "Input word is not legal!" << endl;
    }

    return validWord;
}

//Adding scores
void Game::addScores(Player player, int scoreToAdd) {
    player.addScore(scoreToAdd);
}

void Game::saveGame(int playerIndex, string filename) {
    //removing the blank space from the start
    filename.erase(0,1);
    //appending .txt if the file isnt specified
    filename = filename + ".txt";

    //adding the game data to the file
    ofstream outfile(filename);
    if (!outfile) {
        cout << "Error saving file";
    } else {
        outfile << players.size() << endl;
        for (Player *player: players) {
            // //Saving each players name, score and hand
            outfile << *player;
        }
        //Saving board state
        outfile << *gameBoard;

        //Saving tile bag state
        outfile << *bag;

        //Saving current players name
        outfile << playerIndex << endl;
        outfile.close();
        cout << "Game saved to " << filename << endl;
    }
}

void Game::quitGame() {
    delete bag;

    for(Player* p: players) {
        delete p;
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