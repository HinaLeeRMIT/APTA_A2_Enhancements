#ifndef ASSIGN2_Game_H
#define ASSIGN2_Game_H

#include "Player.h"
#include "TileBag.h"
#include "GameBoard.h"
#include "utils.h"

#include <string>
#include <iostream>
#include <vector>
#include <exception>
#include <regex>
#include <list>

class Game {
  public:

    Game();
    Game(ifstream& gameFile);
    ~Game();

    //Function to make players.
    bool makePlayers(int numPlayers);

    //Function to load in valid words from a file
    void setValidWords();

    //Start game based off numPlayers or loading a game in
    void startGame(int numPlayers);
    void startGameFromLoad();

    //Main game loop. Iterates through each player and executes handlePlayerTurn() on each.
    void mainGameLoop();

    //Checking if the game is over, and if so printing out the announcement for winner
    void checkGameEnd();
    void endGameAnnouncement();

    //Function for handling each players turn. 
    void handlePlayerTurn(Player *player, int playerIndex);

    //Help menu function. 
    void helpMenu();

    //Function to draw new tiles or reimburse tiles to player after valid or invalid turns.
    void drawNewTiles(Player *player);
    void reimburseTiles(Player *player, LinkedList *tempHand);

    //Functions to check if placed word is valid or not
    bool validPlacementCheck(Player *player);
    bool checkHoriWord(int currCol, int finalCol, int row);
    bool checkVertiWord(int currCol, int finalCol, int row);

    //Function to add score to player
    int calcPlacedScore(LinkedList *tempHand);

    //Function to empty the gameboard of tiles placed this turn.
    void emptyPlacedTiles();

    //Function to place all tiles placed this turn.
    void placeTiles(LinkedList *tempHand);

    //Extract placement location from a user input.
    string extractPlacementLocation(string input);

    //Handles player placing a tile.
    void handlePlaceTile(LinkedList *tempHand, Player *player, string placement);

    //Functions to check placement is valid. 
    bool checkValidPlacement(LinkedList *tempHand, Player *player, char tileLetter, string position);
    bool checkTempPositionNotTaken(string position);
    bool checkValidOrientation(string position);


    //Saving game to external file
    void saveGame(int currPlayer, string filename);

    //quitting the game
    void quitGame();

  private:
    TileBag *bag;
    GameBoard *gameBoard;
    vector<Player *> players;
    vector<string> validWords;
    bool gameEnd = false;
    bool quitCommand = false;
    vector<string> placementHistory;
    string rowColCheck = "";
    int loadedCurrPlayer = 0;
};

#endif // ASSIGN2_GAME_H
