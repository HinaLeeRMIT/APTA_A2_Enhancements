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

    //Start game based off numPlayers
    void startGameLoop(int numPlayers);

    //starting the game from a load file
    void startGameFromLoad();

    //looping main functions for game to run
    void mainGameLoop();

    //quitting the game
    void quitGame();


    //adding scores to players
    void addScores(Player player, int scoreToAdd);

    int scoreCheck(int direction, string initialCoord);

    void setStartEndCoord(string initialCoord, int direction);

    int calcHori(string initialCoord);

    int calcVerti(string initialCoord);

    int loopHori(string startCoord, string endCoord);

    int loopVerti(string startCoord, string endCoord);

    //Saving game to external file
    void saveGame(int currPlayer, string filename);
  private:
    TileBag *bag;
    GameBoard *gameBoard;
    vector<Player *> players;
    string startCoord;
    string endCoord;
    vector<string> validWords;
    bool gameEnd = false;
    vector<string> placementHistory;
    string rowColCheck = "";
    int loadedCurrPlayer = 0;

    //setting a list of valid words into vector to be checked against
    void setValidWords();

    //extracting the tile placement specified by the user
    string extractPlacementLocation(string input);


    bool makePlayers(int numPlayers);

    void handlePlayerTurn(Player *player, int playerIndex);

    /**
     * Handles the "place" action
     * by a player and updates the player's score accordingly
     * @param player - the player making the "place" action
     * @param input - the VALID place command by the user
     * @return
     */

    bool handleValidWord(string tempWord);

    void handlePlaceTile(LinkedList *tempHand, Player *player, string placement);
    void drawNewTiles(Player *player);
    void reimburseTiles(Player *player, LinkedList *tempHand);
    void placeTiles(LinkedList *tempHand);

    bool checkValidOrientation(string position);
    bool checkValidPlacement(LinkedList *tempHand, Player *player, char tileLetter, string position);
    void handleFinalPlacement();
    vector<Tile *> scanHoriWord(int row, int col, int boardSize);
    vector<Tile *> scanVertiWord(int row, int col, int boardSize);

    bool okPlacementCheck(Player *player);
    bool checkHoriWord(int currCol, int finalCol, int row);
    bool checkVertiWord(int currCol, int finalCol, int row);
    int calcPlacedScore(LinkedList *tempHand);
    int calcScore();
    int vertiScore(int row, int col);
    int horiScore(int row, int col);

    bool checkTempPositionNotTaken(string position);

    void emptyPlacedTiles();
};

#endif // ASSIGN2_GAME_H
