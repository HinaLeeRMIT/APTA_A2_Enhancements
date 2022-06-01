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

   void addPlayers(int numPlayers);

   //Start game based off numPlayers
   void startGameLoop(int numPlayers);

    //if player hits score ceiling, return true
    bool runScoreCheck();

    //Start 4 player game
    void addScores(Player player, int scoreToAdd);

    int scoreCheck(int direction, string initialCoord);

    void setStartEndCoord(string initialCoord, int direction);

    int calcHori(string initialCoord);

    int calcVerti(string initialCoord);

    int loopHori(string startCoord, string endCoord);

    int loopVerti(string startCoord, string endCoord);

    //Saving game to external file
    void saveGame(string currPlayerName, string filename);
    void startGameFromLoad();

    void mainGameLoop();

    void quitGame();

  private:
    TileBag *bag;
    GameBoard *gameBoard;
    vector<Player *> players;
    string startCoord;
    string endCoord;

    bool makePlayers(int numPlayers);

    bool handlePlayerTurn(Player *player);

    /**
     * Handles the "place" action
     * by a player and updates the player's score accordingly
     * @param player - the player making the "place" action
     * @param input - the VALID place command by the user
     * @return
     */
    void
    handlePlace(Player *player, string input, vector<Tile*> *placedTiles,
                vector<string> *positions);
};

#endif // ASSIGN2_GAME_H
