
#include "MainMenu.h"

#include <iostream>

#define EXIT_SUCCESS    0

int main(void) {

   MainMenu* menu = new MainMenu();

   menu->startMenu();

   return EXIT_SUCCESS;
}
