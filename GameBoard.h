//
// Created by sheku on 11/05/2022.
//

#ifndef APTA_A2_GAMEBOARD_H
#define APTA_A2_GAMEBOARD_H

#include "Tile.h"
#include "utils.h"
#include "Node.h"
#include "LinkedList.h"

#include <exception>
#include <vector>
#include <string>
#include <iostream>
#include <array>

#define BOARD_SIZE 15


using std::vector;
using std::array;
using std::string;
using std::exception;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;


typedef vector<Tile *> BoardRow;
typedef vector<BoardRow> BoardGrid;

/**
 * @brief  Handles board logic and operations
 */
class GameBoard {
  public:
    GameBoard();

    ~GameBoard();

    /**
     * places provided tile to the board,
     * @param tile
     * @param position
     * @return
     */
    bool placeTile(Tile *tile, string position);

    bool spaceIsFree(int row, int col);

    bool isValidPosition(int row, int col);

    string generateBoardView();

    BoardGrid getBoardGrid();
    void setBoardGrid(BoardGrid board);

    /**
     * Computes the score of the tiles placed, including any intersecting tiles.
     * @param placedTiles
     * @param positions
     * @return the total value of the placed tiles
     */
    int computeTurnScore(vector<Tile*> placedTiles, vector<string> positions);


  private:
    BoardGrid boardVector;
    

    // total value of the row
    int computeRowScore(int row, int col, vector<Tile*> placedTiles, vector<string> positions);

};

std::ifstream& operator >> (std::ifstream& inFile,GameBoard &gameBoard);

std::ofstream &operator<<(std::ofstream& outFile, GameBoard &gameBoard);

std::ostream &operator<<(std::ostream& out, GameBoard &gameBoard);


#endif //APTA_A2_GAMEBOARD_H
