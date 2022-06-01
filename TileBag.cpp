#include "TileBag.h"

using std::ifstream;
using std::string;


TileBag::TileBag() {
    bag = new LinkedList();
}

TileBag::~TileBag() {
    delete this->bag;
}

void TileBag::addTile(Tile* tile) {
    bag->addBack(tile);
}

Tile* TileBag::drawTile() {
    return bag->shift();
}

void TileBag::buildBag() {
    ifstream scrabbleTilesFile("ScrabbleTiles.txt");

    for(string line; std::getline(scrabbleTilesFile, line); ) {
        Letter letter = line[0];
        Value value = std::stoi(line.substr(1));
        Tile *tile = new Tile(letter, value);

        bag->addBack(tile);
    }

    shuffle();
}

string TileBag::getBagString() {
    string bagStr = "";

    Node* current = bag->getHead();


    while(current != nullptr) {
        Tile* tile = current->tile;

        bagStr += tile->toString() + ", ";
        current = current->next;
    }

    return bagStr;
}

void TileBag::shuffle() {
    std::vector<int> indices;

    for (int i = 0; i < bag->getLength(); i++)
    {
        indices.push_back(i);
    }

    std::shuffle(indices.begin(), indices.end(), std::random_device());


    LinkedList* newBag = new LinkedList();

    for (int i = 0; i < bag->getLength(); i++)
    {
        int idx = indices[i];
        newBag->addBack(bag->get(idx));
    }

    std::cout << std::endl;

    delete bag;
    bag = newBag;
}

int TileBag::getSize() {
    return bag->getLength();
}

LinkedList* TileBag::getBag() {
    return bag;
}

void TileBag::setBag(LinkedList* list) {
    this->bag = list;
}

std::ifstream &operator>>(std::ifstream &inputFile, TileBag &tileBag) {
    LinkedList* list = new LinkedList();

    inputFile >> *list;

    tileBag.setBag(list);

    return inputFile;
}

std::ofstream &operator<<(std::ofstream &outputFile, TileBag &tileBag) {
    LinkedList* bag = tileBag.getBag();
    outputFile << *bag;

    return outputFile;
}

std::ostream &operator<<(std::ostream &out, TileBag &tileBag) {
    LinkedList* bag = tileBag.getBag();
    out << *bag << endl;

    return out;
}