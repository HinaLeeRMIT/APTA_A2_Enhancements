
#include "Node.h"

Node::Node(Tile* tile, Node* next, Node* prev)
{
   this->tile = tile;
   this->next = next;
   this->prev = prev;
}

Node::Node(Tile *tile, Node *next)
{
    this->tile = tile;
    this->next = next;
    this->prev = nullptr;
}

Node::Node(Node& other) {
   this->tile = other.tile;
   this->prev = other.prev;
   this->next = other.next;
}

Node::~Node()
{
   delete tile;
   if (next != nullptr) {
      delete next;
   }
}

bool Node::isEqual(Node* otherNode) {
   return otherNode->tile->equals(tile);
}
