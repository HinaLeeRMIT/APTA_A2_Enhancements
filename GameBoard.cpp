#include "GameBoard.h"

GameBoard::GameBoard() {

    // build vector with nullptrs
    for (int i = 0; i < BOARD_SIZE; i++) {
        vector<Tile *> row(15);
        for (int i = 0; i < BOARD_SIZE; i++) {
            row.push_back(nullptr);
        }

        boardVector.push_back(row);
    }

}

GameBoard::~GameBoard() {

}

void GameBoard::placeTile(Tile *tile, int row, int col) {
    boardVector.at(row).at(col) = tile;
}

Tile* GameBoard::retrieveTile(int row, int col){
    return boardVector.at(row).at(col);
}

bool GameBoard::spaceIsFree(int row, int col) {
    bool isFree = false;

    if (isValidPosition(row, col)) {
        Tile *tileAtPos = boardVector.at(row).at(col);

        isFree = tileAtPos == nullptr;
    }

    return isFree;
}

bool GameBoard::isValidPosition(int row, int col) {
    bool isValid = false;
    // row and col must fit the bounds
    isValid = (row >= 0 && row <= 14) && (col >= 0 && col <= 14);

    return isValid;
}


BoardGrid GameBoard::getBoardGrid() {
    return boardVector;
}

void GameBoard::setBoardGrid(BoardGrid board) {
    this->boardVector = board;
}

int GameBoard::computeTurnScore(vector<Tile *> placedTiles,
                                vector<string> positions) {
    int computedScore = 0;


    //full word
    for(auto tile : placedTiles) {
        computedScore += tile->value;
    }


    vector<vector<int>> gridPositions;
    for(string position: positions) {
        array<int, 2> rowColArr = convertPosTokenToInt(position);

        int row = rowColArr.at(0);
        int col = rowColArr.at(1);

        gridPositions.push_back({row, col});
    }

    for (vector<int> rowCol: gridPositions) {
        int row = rowCol.at(0);
        int col = rowCol.at(1);

        // sum of intersecting tiles excluding the tile at [rowInt, colInt]
        int rowScore = computeRowScore(row, col, placedTiles, positions);
        int colScore = 0;

        computedScore += rowScore + colScore;
        cout << "computed score: " << computedScore << endl;
    }

    return computedScore;
}

int GameBoard::computeRowScore(int row, int col, vector<Tile*> placedTiles, vector<string> positions) {
    int rowScore = 0;

    vector<Tile*> rowVec = boardVector.at(row);

//    Tile* placedTile = rowVec.at(col);
    Tile* before = nullptr;
    Tile* after = nullptr;

    // bounds checking to avoid exceptions
    if(col > 0 && col < 14) {
        before = rowVec.at(col - 1);
        after = rowVec.at(col + 1);
    }

    string beforePos = convertRowColToPosToken(row, col -1);
    string afterPos = convertRowColToPosToken(row, col + 1);


    bool beforeFromPlayerHand = false;
    bool afterFromPlayerHand = false;

    for(auto pos : positions) {
         if(pos == beforePos) {
             beforeFromPlayerHand = true;
         }

         if(pos == afterPos) {
             afterFromPlayerHand = true;
         }
    }


    // value of all played tiles is calculated outside this function.
    if(!beforeFromPlayerHand && !afterFromPlayerHand) {
        // two adjacent tiles
        if(before != nullptr && after != nullptr) {
            // two adjacent tiles, scan the entire rowVec

            int beforeScore = 0;
            int afterScore  = 0;

            // scan up to the before tile
            for(int i = 0; i < col; i++) {
                Tile* tile = rowVec.at(i);
                if(tile != nullptr) {
                    beforeScore += tile->value;
                } else {
                    beforeScore = 0;
                }
            }

            // scan from the after tile to the first nullptr or end of grid
            bool chainEnded = false;
            for(int i = col +1; i < 15 && !chainEnded; i++) {
                Tile* tile = rowVec.at(i);

                if(tile != nullptr) {
                    afterScore += tile->value;
                } else {
                    chainEnded = true;
                }
            }

            rowScore = beforeScore + afterScore;



        } else if(before != nullptr) {
            // one adjacent tile before the placed tile,
            // scan up to the adjacent tile (not including)
            for(int i =0; i < col; i++) {
                Tile* tile = rowVec.at(i);
                if(tile != nullptr) {
                    rowScore += tile->value;
                } else {
                    // empty tiles break the chain
                    rowScore = 0;
                }
            }
        } else if(after != nullptr) {
            // adjacent tile right of the placed tile, sum value of each
            bool chainEnded = false;
            for(int i = col +1; i < 15 && !chainEnded; i++) {
                Tile* tile = rowVec.at(i);

                if(tile != nullptr) {
                    rowScore += tile->value;
                } else {
                    chainEnded = true;
                }
            }
        }
    }

    return rowScore;
}

int GameBoard::getBoardSize(){
    return BOARD_SIZE;
}

std::ifstream &operator>>(std::ifstream& inFile,GameBoard &gameBoard) {
    BoardGrid grid; 


    // build vector with nullptrs
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        vector<Tile *> row(15);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            row.push_back(nullptr);
        }

        grid.push_back(row);
    }


    string line;
    std::getline(inFile, line);

    regex boardExpr("[A-Z]-[\\d]+@[A-Z][\\d]+");

    regex_string_iterator sit(line.begin(), line.end(), boardExpr);

    while(sit != regex_string_iterator()) {
        string tilePos = sit->str();
        char tileLetter = tilePos[0];
        char tileValue = tilePos[2];
        string position = tilePos.substr(4);

        std::array<int, 2> rowCol = convertPosTokenToInt(position);
        int row = rowCol.at(0);
        int col = rowCol.at(1);

        grid.at(row).at(col) = new Tile(tileLetter, tileValue - '0');
        ++sit;
    }

    gameBoard.setBoardGrid(grid);

    return inFile;
}

std::ofstream &operator << (std::ofstream& outFile,GameBoard &gameBoard) {
    BoardGrid  grid = gameBoard.getBoardGrid();

    for(size_t i = 0; i < grid.size(); i++) {
        for(size_t j = 0; j < grid.at(i).size(); j++) {
            Tile* tile = grid.at(i).at(j);
            if(tile != nullptr) {
                string tileStr = tile->toString();
                char row = 'A' + i;
                outFile << tileStr << '@' << row << j;
                if(i != grid.size() -1  && j != grid.at(i).size() -1) {
                    outFile << ",";
                }
            }
        }
    }

    outFile << endl;

    return outFile;
}

std::ostream &operator<<(std::ostream &out, GameBoard &gameBoard) {
    BoardGrid boardGrid = gameBoard.getBoardGrid();
    cout << "    " << 0 << "   ";
    for (int i = 1; i < 10; i++) {
        out << i << "   ";
    }
    //formatting for double digits
    for (int i = 10; i < BOARD_SIZE; i++) {
        out << i << "  ";
    }
    out << endl;
    out << " ";
    for (int i = 0; i < (BOARD_SIZE * 2 + 1); i++) {
        out << " -";
    }
    out << endl;

    for (int i = 0; i < BOARD_SIZE; i++) {
        BoardRow row = boardGrid.at(i);

        char rowChar = i + 65;
        out << rowChar << " |";

        // loop through each column and display the tile or nothing
        for (int i = 0; i < BOARD_SIZE; i++) {
            Tile *tile = row.at(i);

            if (tile == nullptr) {
                // display the guards for nothing
                out << "   |";
            } else {
                // display tile against guards
                out << " " << tile->letter << " |";
            }
        }

        out << endl;
    }
    return out;
}
