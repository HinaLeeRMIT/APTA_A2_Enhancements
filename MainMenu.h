#ifndef ASSIGN2_MAINMENU_H
#define ASSIGN2_MAINMENU_H

#include "Game.h"
#include "TileBag.h"

#include <iostream>
#include <string>
#include <exception>

using std::cout;
using std::cin;
using std::endl;
using std::exception;
using std::regex;
using std::regex_match;
using std::ofstream;
using std::ifstream;

class MainMenu {
public:

    MainMenu();
    ~MainMenu();

    void startMenu();
    void printMainOptions();

    void startGame(int numPlayers);

    bool loadGame();

    void credits();

private:
    int numPlayers;
    Game* game;
};

#endif // ASSIGN2_MAINMENU_H
