
#include "Tile.h"

Tile::Tile(Letter letter, Value value) {
    this->letter = letter;
    this->value = value;
}

bool Tile::equals(Tile* tile) {
    return tile->letter == letter && tile->value == value;
}

std::string Tile::toString() {
    std::string tileStr = "";
    tileStr.push_back(letter);
    tileStr += "-" + std::to_string(value);

    return tileStr;
}

bool Tile::operator==(const Tile& tile) const {
    return tile.letter == letter && tile.value == value;
}