
#ifndef ASSIGN2_NODE_H
#define ASSIGN2_NODE_H

#include "Tile.h"

class Node {
public:

   Node(Tile* tile, Node* next, Node* prev);
   Node(Tile* tile, Node* next);
   Node(Node& other);
   ~Node();

   bool isEqual(Node* otherNode);

   Tile*    tile;
   Node*    next;
   Node*    prev;
};

#endif // ASSIGN2_NODE_H
