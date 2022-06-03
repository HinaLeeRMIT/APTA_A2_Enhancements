#include "PlayerHand.h"

#include <iostream>

PlayerHand::PlayerHand() {
   hand = new LinkedList();
}

PlayerHand::~PlayerHand() {
    delete hand;
}

void PlayerHand::generateHand(LinkedList* deckList) {
    for (int i = 0; i < 7; i++){
        hand->addBack(deckList->getHeadTile());
        deckList->removeHead();
    }    
}

void PlayerHand::displayHand(){
    for(int i = 0; i < hand->getLength(); i++){
        if (i != 0){
            std::cout << ", ";
        }

        Tile* currTile = hand->get(i);

        std::cout << currTile->letter << "-" << currTile->value;
    }
    std::cout << std::endl;
}

Tile* PlayerHand::newTile(LinkedList* deckList) {
    Tile* newCard = deckList->getHeadTile();
    deckList->removeHead();

    return newCard;
}

void PlayerHand::addTile(Tile* tileToAdd) {
    hand->addBack(tileToAdd);
}

Tile* PlayerHand::getTileFromLetter(char letter) {
    bool tileFound = false;
    Node* current = hand->getHead();
    Tile* toReturn = nullptr;

    while(current != nullptr && !tileFound) {
        if(current->tile->letter == letter) {
            toReturn = new Tile(*current->tile);
            tileFound = true;
        } else {
            current = current->next;
        }

    }

    return toReturn;
}

void PlayerHand::deleteTile(Tile* tileToDelete) {
    bool tileDeleted = false;
    for(int i = 0; i < hand->getLength() && !tileDeleted; i++){
        if(hand->get(i)->equals(tileToDelete)) {
            hand->remove(i);
            tileDeleted = true;
        }
    }
}

bool PlayerHand::contains(Tile* tile){
    return hand->contains(tile);
}

bool PlayerHand::containsLetter(char letter) {
    bool hasLetter = false;

    for(int i = 0; i < hand->getLength() && !hasLetter; i++){
        if(hand->get(i)->letter == letter) {
            hasLetter = true;
        }
    }

    return hasLetter;
}

int PlayerHand::getLength(){
    return hand->getLength();
}

LinkedList* PlayerHand::getInternalHandList() {
    return hand;
}

void PlayerHand::setInternalList(LinkedList* list) {
    this->hand = list;
}

std::ifstream &operator>>(std::ifstream& inFile,PlayerHand &hand) {
    LinkedList* linkedList = new LinkedList();

    inFile >> *linkedList;

    hand.setInternalList(linkedList);
    return inFile;
}

std::ofstream &operator<<(std::ofstream &outputFile, PlayerHand &hand) {
    outputFile << *hand.getInternalHandList();

    return  outputFile;
}

std::ostream &operator<<(std::ostream &out, PlayerHand &hand)
{
    out << *hand.getInternalHandList();

    return out;
}
