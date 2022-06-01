#include "LinkedList.h"

LinkedList::LinkedList() {
   head = nullptr;
   tail = nullptr;
   length = 0;
}

LinkedList::~LinkedList() {
   delete head;
}

/**
 * @brief adds tile to the front of the list. creates new head if head is null, else sets current head as the next node of the new head. 
 * 
 * @param tile the tile to be inserted to the front
 */
void LinkedList::addFront(Tile* tile) {
   Node* newHead = new Node(tile, head);
   if(head == nullptr) {
      head = newHead;
      tail = newHead;
   } else {
      head->prev = newHead;
      head = newHead;
   }

   length++;
}

void LinkedList::addBack(Tile* tile) {
   Node* newTail = new Node(tile, nullptr);

   if(head == nullptr) {
      head = newTail;
      tail = newTail;
   } else {
      tail->next = newTail;
      newTail->prev = tail;
      tail = newTail;
   }

   length++;
}

Tile* LinkedList::get(int index) {
   Tile* toReturn;

   if(index == 0) {
      toReturn = getHeadTile();
   } else if (index == length - 1) {
      toReturn = getTailTile();
   } else if (index >= length || index < 0) {
      toReturn = nullptr;
   } else {
      Node* current = head;

      for(int i = 0; i < index; i++) {
         current = current->next;
      }

      toReturn = new Tile(*current->tile);
   }

   return toReturn;
}

Node* LinkedList::getHead() {
   return new Node(*head);
}

Node *LinkedList::getTail() {
    return new Node(*tail);
}

Tile *LinkedList::getHeadTile()
{
    Tile *toReturn = nullptr;

    if (head != nullptr)
    {
        toReturn = new Tile(*head->tile);
    }

    return toReturn;
}

Tile *LinkedList::getTailTile()
{
    Tile *toReturn = nullptr;

    if (tail != nullptr)
    {
        toReturn = new Tile(*tail->tile);
    }

    return toReturn;
}

/**
 * @brief traverses the list to find if the list contains the given tile. returns false if the list is empty
 * 
 * @param tile the tile that we're searching for
 * @return true if the tile is in the list
 * @return false if the tile is NOT in the list
 */
bool LinkedList::contains(Tile* tile) {
   Node* current = head;

   bool contains = false;

   // traverse the list until the node is found
   while(current != nullptr && current->next != nullptr && !contains) {
      contains = current->tile->equals(tile);
      current = current->next;
   }

   return contains;
}

Tile *LinkedList::shift() {
   Tile* toReturn = nullptr; 

   if(head != nullptr) {
      Node* nextHead = head->next;
      toReturn = new Tile(*head->tile);

      if(nextHead != nullptr) {

         // removing the next reference so it doesn't get freed
         head->next = nullptr;
         delete head;
         // set the list head to the next node
         nextHead->prev = nullptr;
         head = nextHead;
      } else {
         // list is now empty, we should delete the head
         delete head;
      }

      length--;
   }

   return toReturn;
}

Tile* LinkedList::pop() {
   Tile* toReturn = nullptr;

   if(tail != nullptr) {
      Node* newTail = tail->prev;
      toReturn = new Tile(*tail->tile);

      // ideally should always be true, but we check anyway to avoid a segfault
      if(newTail != nullptr) {
          delete tail;
          newTail->next = nullptr;
          tail = newTail;
      } 

      length--;
   }

   return toReturn;
}

void LinkedList::remove(int index) {
   if (index == 0) {
      this->removeHead();
   } else if (index == length - 1) {
      this->removeTail();
   } else if ( !(index < 0 || index >= length)) {
      // index is within bounds

      // traverse till we find the node
      Node* current = head;
      for(int i = 0; i < index; i++) {
         current = current->next;
      }

      Node* prev = current->prev;
      Node* next = current->next;

   
      prev->next = next;
      next->prev = prev;

      // remove next ref to avoid double delete
      current->next = nullptr;
      delete current;
      length--;
   }
}

void LinkedList::remove(Tile *toRemove) {
    Node* current = head;
    bool tileFound = false;
    int index = 0;

    while(current != nullptr && !tileFound ) {
        if(current->tile->equals(toRemove)) {
            tileFound = true;
        } else {
            index++;
            current = current->next;
        }
    }

    remove(index);
}

void LinkedList::removeHead() {
    if (head != nullptr) {
        Node *nextHead = head->next;
        if (nextHead != nullptr)
        {
            nextHead->prev = nullptr;
            head->next = nullptr;
            delete head;
            head = nextHead;
        }
        else
        {
            head = nullptr;
        }
        length--;
    }
}

void LinkedList::removeTail() {
   if(tail != nullptr) {
      Node* nextTail = tail->prev;

      if(nextTail->prev != nullptr) {
         nextTail->next = nullptr;
         delete tail;
         tail = nextTail;
      }

      length--;
   }
}

int LinkedList::getLength() {
   return length;
}

std::string LinkedList::getListString() {
    Node *current = head;
    std::string listStr = "";

    for(int i = 0; i < length; i++) {
        Tile *tile = current->tile;
        listStr += tile->toString();

        if (i != length -1)
        {
            listStr += ",";
        }

        current = current->next;
    }

    return listStr;
}

std::ifstream &operator>>(std::ifstream &inFile, LinkedList &linkedList) {
   // "A-1" / <letter>-<value>

   string tiles = "";
   std::getline(inFile, tiles);

   regex expr("[A-Z]-(\\d+)");

   regex_string_iterator it(tiles.begin(), tiles.end(), expr);

   // iterate through each tile of format <letter>-<value>
   while(it != regex_string_iterator()) {
      string tileStr = it->str();
      // tile at index 0 = <letter>
      Letter letter = tileStr.at(0);
      // tile at index 2 = <value>
      Value value = tileStr.at(2) - '0';

      Tile* tile = new Tile(letter, value);

      linkedList.addBack(tile);
      ++it;
   }

   return inFile;
}

    std::ofstream &
    operator<<(std::ofstream &outputFile, LinkedList &linkedList)
{
   std::string listStr = linkedList.getListString();

   outputFile << listStr << std::endl;

   return outputFile;
}

std::ostream &operator<<(std::ostream &out, LinkedList &linkedList) {
    std::string listStr = linkedList.getListString();

    out << listStr << std::endl;

    return out;
}
