#include "Game.h"

#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::exception;
using std::regex;
using std::regex_match;

/*
*default constructor
*/
Game::Game() {
    bag = new TileBag();
    bag->buildBag();
    gameBoard = new GameBoard();
    setValidWords();
}

/*
*Overloaded constructor for loading game file
*/
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

    setValidWords();
}

/*
*Destructor
*/
Game::~Game() {
    delete bag;

    for (Player *player: players) {
        delete player;
    }
}

/*
*Function to make players. 
*Returns a boolean to confirm that players are made
*/
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

/*
*Function to load in valid words from a file. 
*Used to check against words players will place.
*/
void Game::setValidWords(){
    //setting list of valid words to check against
    ifstream inFile("validWords");

    //checking if file loaded properly
    if(!inFile.fail()){
        while(inFile.peek()!=EOF){
            string validWord = "";
            getline(inFile, validWord);
            validWords.push_back(validWord);
        }
    }
}

/*
*Default game start
*/
void Game::startGame(int numPlayers) {
    bool playersMade = makePlayers(numPlayers);
    if (playersMade) {
        cout << endl;
        cout << "Let's Play!" << endl;
        cout << "If you have any trouble, feel free to type 'help'!" << endl;
        mainGameLoop();
    }
}

/*
*Alternative game start from load file
*Checks if game ends, if so, print announcement and quit application.
*/
void Game::startGameFromLoad() {
    //first finish current cycle of turns, in case load game doesnt start from player 1
    bool turnCycleDone = false;
    while(!gameEnd && !quitCommand && !std::cin.eof() && !turnCycleDone){
        for (int i = loadedCurrPlayer; i < int(players.size()) && !gameEnd; i++){
            Player *player = players[i];
            handlePlayerTurn(player, i);

            //checking if game ends
            checkGameEnd();
            if(gameEnd){
                endGameAnnouncement();
            }
        }
        turnCycleDone = true;
    }

    //Once cycle of turns is done, continue main game loop from player one
    mainGameLoop();
}

/*
*Main game loop.
*Iterates through each player and executes handlePlayerTurn() on each.
*Checks if game ends, if so, print announcement and quit application.
*/
void Game::mainGameLoop() {
    while (!gameEnd && !quitCommand && !std::cin.eof()) {
        // loop through each player, handle their turns.
        for (int i = 0; i < int(players.size()) && !gameEnd; i++){
            Player *player = players[i];
            handlePlayerTurn(player, i);

            //checking if game ends
            checkGameEnd();
            if(gameEnd){
                endGameAnnouncement();
            }
        }
    }
}

/*
*Function to check if game ends. If any game end condiditon is met, return true, else return false.
*Cond 1. Game will end is bag is empty
*Cond 2. Game will end if player has no tiles in hand
*Cond 3. game will end if player has passed 2x in a row
*/
void Game::checkGameEnd(){  
    //Cond 1 check
    if(bag->getSize() == 0){
        gameEnd = true;
    }   

    //Cond 2 and 3 check
    for (int i = 0; i < int(players.size()) && !gameEnd; i++){
            Player *player = players[i];
            if (player->getHandSize() == 0){
                gameEnd = true;
            }
            if(player->getPassCount() == 2){
                gameEnd = true;
            }    
        }
}

/*
*Announcement for game end.
*Prints out scores for each player.
*Prints out winner(s).
*/
void Game::endGameAnnouncement(){
    //integer for tracking the highest score
    int highestScore = 0;

    //list for all the winners
    vector<int> winners;

    //Printing out the scores for each player
    cout << endl;
    cout << "Game Over" << endl;
    for (int i = 0; i < int(players.size()); i++){
            Player *player = players[i];
            cout << "Score for " << player->getName() << ": " << player->getScore() << endl;
            if(player->getScore() > highestScore){
                highestScore = player->getScore();
            }
    }

    //Finding the winners and appending to the vector
    for (int i = 0; i < int(players.size()); i++){
            Player *player = players[i];
            if(player->getScore() == highestScore){
                winners.push_back(i);
            }
    }

    //Printing out all winners
    cout << "Player ";
    cout << players.at(winners.at(0))->getName(); 
    for(int i = 1; i < int(winners.size()); i++){
        if (i < int(winners.size())){
            cout << " and ";
            cout << players.at(winners.at(i))->getName();                              
        }
    }
    cout << " won!"<< endl;
    cout << endl;
}

/*
*Function for handling each players turn. 
*Uses regex to determine players desired execution.
*Players can: Place a tile, confirm placement, replace a tile, pass turn, request help meny, save game and quit game from here.  
*Loops until a valid turn is executed.
*/
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

        //list of regular expressions used to execute turns
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
        if (regex_match(input, placeTileExpr)) {  //place tile execution
            string placement = extractPlacementLocation(input);
            handlePlaceTile(tempHand, player, placement);
        } else if (regex_match(input, placeDoneExpr)) {  //place Done execution
            //Checks whether the placement is valid or not
            bool validPlacement = validPlacementCheck(player); 
            if (validPlacement){ //If placement is valid, place tiles on board, draw new tiles, add score to player and end turn.
                turnEnd = true;
                player->resetPassCount();
                drawNewTiles(player);
                placeTiles(tempHand);
                player->addScore(calcPlacedScore(tempHand));
            }else{ //If placement is not valid, return tiles used to players hand and inform player of their hand again.
                reimburseTiles(player, tempHand);
                emptyPlacedTiles();
                cout << endl;
                cout << "Your current hand is: " << *player->getHand() << endl;
            }

            //resetting temporary variables to restart turn loop
            delete tempHand;
            tempHand = new LinkedList;
            placementHistory.clear();
            rowColCheck = "";

        } else if(regex_match(input, replaceTileExpr)){ //replace tile execution
            //getting the char and setting to a char
            string stringTileLetter = input.substr(8);
            char tileLetter = stringTileLetter[0];

            Tile *tileToReplace = bag->drawTile();
            if(player->hasTileLetter(tileLetter)){ //If player has the tile, replace the tile from the players hand. Print out new tile recieved and updated hand.
                player->replaceTile(player->getTileFromLetter(tileLetter), tileToReplace);
                cout << endl;
                cout<< "The new tile you recieved is " << tileToReplace->letter << "-" << tileToReplace->value << endl;
                cout << "\nYour new hand is\n" << *player->getHand();
                turnEnd = true;
                delete tempHand;
                placementHistory.clear();
                rowColCheck = "";
            }
            else{ //If player does not have the tile, print error message
                cout << "You don't have this tile to replace!" << endl;
            }
        } else if(regex_match(input, passTurnExpr)){ //pass turn execution
            cout << "Turn passed!\n";
            player->addPassCount();
            turnEnd = true;
            delete tempHand;
            placementHistory.clear();
            rowColCheck = "";
        } else if(regex_match(input, helpExpr)){ //help execution
            helpMenu();
        } else if(regex_match(input, saveGameExpr)){ //save game execution
            string filename = input.substr(4);
            saveGame(playerIndex, filename);
        } else if(regex_match(input, quitGameExpr)){ //quit game execution
            turnEnd = true;
            quitCommand = true;
        } else if (cin.eof()) { //EOF execution
            turnEnd = true;
        } else {    //Invalid input execution
            cout << "Invalid action provided. Please try again" << endl;
        }
    }
}

/*
*Help menu function. 
*Prints out useful commands for the user.
*/
void Game::helpMenu(){
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
}

/*
*Function to draw a new tile.
*While a player does not have minimum hand size of 7, or bag isnt empty, keep drawing tiles.
*/
void Game::drawNewTiles(Player *player){
    while(player->getHandSize() < 7 || bag->getSize() == 0){
        Tile *newTile = bag->drawTile();
        player->addTile(newTile);
    }
}

/*
*Reimburse tiles.
*Uses tempHand to restore played tiles to a player when a turn was invalid. 
*/
void Game::reimburseTiles(Player *player, LinkedList *tempHand){
    for(int i = 0; i < tempHand->getLength(); i++){
        player->addTile(tempHand->get(i));
    }
}

/*
*Function to determine whether the placement of tiles is valid
*/
bool Game::validPlacementCheck(Player *player){
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

    //check if word exists inside of validword check
    
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

/*
*Checks the horizontal word from where the user placed tiles.
*If the placement is valid, return true.
*If invalid, return false.
*/
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
                foundNull = true;
            }
        }

    return foundNull;
}

/*
*Checks  the vertical word from where the user placed tiles.
*If the placement is valid, return true.
*If invalid, return false.
*/
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
                foundNull = true;
            }
        }

    return foundNull;
}

/*
*Function to calculate the score of the tiles placed by the user this turn.
*returns that value as an int.
*/
int Game::calcPlacedScore(LinkedList *tempHand){
    int scoreToAdd = 0;
    for(int i = 0; i < tempHand->getLength(); i++){
        scoreToAdd += tempHand->get(i)->value;
    }
    return scoreToAdd;
}

/*
*Function to empty the gameboard of tiles placed this turn.
*Only used when turn is invalid to return board to original state.
*/
void Game::emptyPlacedTiles(){
    for (int i = 0; i < int(placementHistory.size()); i++){
        std::array<int, 2> rowCol = convertPosTokenToInt(placementHistory[i]);
        int row = rowCol.at(0);
        int col = rowCol.at(1);
        gameBoard->placeTile(nullptr, row, col);
    }
}


/*
*Function to place all tiles placed this turn.
*Loops through tempHand and places all tiles inside on the game board.
*/
void Game::placeTiles(LinkedList *tempHand){
    for(int i = 0; i < int(placementHistory.size()); i++){
        std::array<int, 2> rowCol = convertPosTokenToInt(placementHistory.at(i));
        int row = rowCol.at(0);
        int col = rowCol.at(1);
        gameBoard->placeTile(tempHand->get(i), row, col);
    }
}

/*
*Extract placement location from a user input.
*Takes in a command based on users input, validated through the regex set.
*converts that input into a usable coordinate for out code.
*/
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

/*
*Handles player placing a tile.
*checks for row and column integrity:
*   - make sure words placed go in a straight line
*   - makes sure there are no gaps between words placed, unless a letter already exists on the board to fill the gap
*/
void Game::handlePlaceTile(LinkedList *tempHand, Player *player, string placement){
    char tileLetter = placement[0];
    string position = placement.substr(1);

    //check for row or col integrity.
    if (tempHand->getLength() == 0) { // if only one tiles is being placed, only check validity of tiles placed against tiles on board
        if(!checkValidPlacement(tempHand, player, tileLetter, position)){
            cout << "Invalid placement, please try again!" << endl;
            cout << endl;
            cout << "Your current hand is: " << *player->getHand() << endl;
        }else{placementHistory.push_back(position); rowColCheck = position;}
    }
    else{ //if more than one tile isbeing placed, check orientationas well as validity of tiles placed against tiles on board
        if(!checkValidOrientation(position) || !checkValidPlacement(tempHand, player, tileLetter, position)){
            cout << "Invalid placement, please try again!" << endl;
            cout << endl;            
            cout << "Your current hand is: " << *player->getHand() << endl;
        }else{placementHistory.push_back(position);}
    }
}

/*
*Function to check placement is valid. 
*   -Checks that player has tile that they wish to play.
*   -Checks that the gameboard space desired is available.
*/
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

/*
*Function to determine if placementHistory doesn't contain the coordinate desired by the player.
*placementHistory: Stores a vector of coordinates of tiles already placed by a player this turn.
*/
bool Game::checkTempPositionNotTaken(string position){
    bool positionNotTaken = true;
    for(int i = 0; i < int(placementHistory.size()); i++){
        if(position == placementHistory[i]){
            positionNotTaken = false;
        }
    }
    
    return positionNotTaken;
}

/*
*Checks that the orientation is valid.
*If it is the first tile being placed by the user, no orientation rulings. 
*   - Set rowColCheck to the coordinate of the first tile placed.
*If it is the 2nd tile placed by the user, make sure that either the row or the column is the same as that of rowColCheck.
*   - Set rowColCheck to the row or column that is equal.
*If there are 2 or more tiles placed, rowColCheck will display either the row or column the player much place tiles within.
*   - enforce that all subsequent tile placements had either the row or col match rowColCheck.
*/
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





/*
*Function to save game.
*Saves game to a .txt file.
*Formats of save file:
*   -Number of players.
*   -For each player.
*       -Players name.
*       -Players score.
*       -Players hand.
*   -Tiles currently placed on the board and their positioning.
*   -Tiles left inside of the tile bag.
*   -The player index of whos turn it currently is.
*/
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

/*
*Function to quit the game.
*Deletes memory allocated to the heap.
*/
void Game::quitGame() {
    delete bag;

    for(Player* p: players) {
        delete p;
    }
}