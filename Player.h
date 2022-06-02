#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#include "PlayerHand.h"
#include "TileBag.h"

#include <string>
#include <iostream>

using std::string;


class Player {
public:

    Player();
    Player(string name);
    ~Player();

    //draws 7 tiles from TileBag and adds them to the player's hand
    void setPlayerHand(TileBag* bag);

    PlayerHand* getHand();

    void setHand(PlayerHand* hand);

    //Play x tiles
    void placeTile(Tile* tileToPlay);

    /**
     * removes first tile with corresponding letter from player's hand and returns it.
     * @param letter - letter of the tile to place
     * @return first tile in player hand matching letter parameter
     */
    Tile* placeTileFromLetter(char letter);

    /**
     * removes first corresponding tile from player hand and returns a COPY
     * @param letter
     * @return first tile with matching letter
     */
    Tile* getTileFromLetter(char letter);

    void replaceTile(Tile* toReplace, Tile* frombag);

    //Add Score
    void addScore(int addScore);

    int getScore();

    bool turnsPassedTwice();

    void passTurn();

    bool hasTileLetter(char letter);

    std::string getName();
    void setName(string name);

private:
    std::string name;
    PlayerHand* hand;
    int score;

    int turnsPassed;
};

std::ifstream& operator >> (std::ifstream& inFile,Player &player);

std::ofstream& operator << (std::ofstream& outputFile, Player &player);

#endif // ASSIGN2_PLAYER_H