
#ifndef ASSIGN2_PLAYERHAND_H
#define ASSIGN2_PLAYERHAND_H

#include "Node.h"
#include "Tile.h"
#include "LinkedList.h"
#include <iostream>

class PlayerHand {
public:

   PlayerHand();
   ~PlayerHand();

   //Generate first hand
   void generateHand(LinkedList* deckList);

   //Display hand
   void displayHand();

   //Pick up a new tile
   Tile* newTile(LinkedList* deckList);

   //Add tile to hand
   void addTile(Tile* tileToAdd);

   /**
    * removes the first instance of tile with provided letter
    * and returns a COPY of said tile.
    * @param letter
    * @return
    */
   Tile* getTileFromLetter(char letter);

   //Remove tile from hand
   void deleteTile(Tile* tileToDelete);

   bool contains(Tile* tile);

   bool containsLetter(char letter);

   LinkedList* getInternalHandList();
   void setInternalList(LinkedList* list);

private:
   LinkedList* hand;
};

std::ifstream& operator >> (std::ifstream& inFile,PlayerHand &hand);

std::ofstream &operator<<(std::ofstream &outputFile, PlayerHand &hand);

std::ostream &operator<<(std::ostream &out, PlayerHand &hand);

#endif // ASSIGN2_PLAYERHAND_H
