
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"
#include "Tile.h"
#include "utils.h"

#include <iostream>
#include <regex>
#include <fstream>
#include <string>

using std::string;
using std::regex;



class LinkedList {
public:

   LinkedList();
   ~LinkedList();

   void addFront(Tile* tile);
   void addBack(Tile* tile);

   // returns COPY of tile at index
   Tile* get(int index);

   // returns COPY of head Node
   Node* getHead();
   Node* getTail();

   // returns COPY of tile in head
   Tile* getHeadTile();
   // returns COPY of tile in tail
   Tile* getTailTile();

   bool contains(Tile* tile);

   // removes head node, returns a COPY of its Tile
   Tile* shift();

   // removes tail node, returns a COPY of its Tile
   Tile* pop();

   /**
    *
    * @param index - index of the
    */
   void remove(int index);

   /**
    * Deletes the first instance of the provided tile from the linked list
    * Calls remove(int index) internally to manage the linked list.
    * @param toRemove - the tile that
    */
   void remove(Tile* toRemove);

   void removeHead();
   void removeTail();
   int getLength();

   std::string getListString();

private:
   Node* head;
   Node* tail;
   int length;
};

std::ifstream &operator >>(std::ifstream &inFile, LinkedList &linkedList);

// outputs contents of the linked list to an ofstream. includes new line.
std::ofstream &operator<<(std::ofstream &outputFile, LinkedList &linkedList);

std::ostream &operator <<(std::ostream & out, LinkedList &linkedList);

#endif // ASSIGN2_LINKEDLIST_H
