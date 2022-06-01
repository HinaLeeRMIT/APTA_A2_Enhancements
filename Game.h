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

    //setting a list of valid words into vector to be checked against
    void setValidWords();

    bool makePlayers(int numPlayers);

    bool handlePlayerTurn(Player *player, int playerIndex);

    /**
     * Handles the "place" action
     * by a player and updates the player's score accordingly
     * @param player - the player making the "place" action
     * @param input - the VALID place command by the user
     * @return
     */
    string handlePlace(Player *player, string input, string tempWord, vector<Tile*> *placedTiles,
                vector<string> *positions);

    void handlePlaceTile(Player *player, string input, vector<Tile*> *placedTiles,
                vector<string> *positions);
    bool handleValidWord(string tempWord);
};

#endif // ASSIGN2_GAME_H
