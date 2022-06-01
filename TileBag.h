#ifndef ASSIGN2_TILEBAG_H
#define ASSIGN2_TILEBAG_H

#include "LinkedList.h"
#include "Tile.h"

#include <random>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::ofstream;
using std::ifstream;


class TileBag {
    public:
    TileBag();
    ~TileBag();

    // adds the tile to the back of the bag
    void addTile(Tile* tile);

    // returns a COPY of the front-most tile
    Tile* drawTile();

    void buildBag();

    std::string getBagString();

    void shuffle();

    int getSize();

    LinkedList* getBag();
    void setBag(LinkedList* list);

  private:
    LinkedList* bag;

};

std::ifstream& operator >> (std::ifstream& inputFile,TileBag& tileBag);

std::ofstream& operator << (std::ofstream& outputFile, TileBag& tileBag);

std::ostream& operator << (std::ostream& out,  TileBag& tileBag);

#endif