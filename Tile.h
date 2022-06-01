
#ifndef ASSIGN2_TILE_H
#define ASSIGN2_TILE_H

#include <string>

// Define a Letter type
typedef char Letter;

// Define a Value type
typedef int Value;

class Tile {
public:
   Tile(Letter letter, Value value);
   Letter letter;
   Value  value;

   bool equals(Tile* tile);
   std::string toString();

   bool operator ==(const Tile& tile) const;
};

#endif // ASSIGN2_TILE_H
