//
// Created by Sao on 13/05/2022.
//

#include "utils.h"

#include <iostream>

vector<string> splitPlaceCommand(string place_cmd) {
    regex tileExpr("[\\s][A-Z][\\s]");
    regex posExpr("[A-Z]([0-9]+)");
    vector<string> tokens;

    // extract the tile token, e.g. "A", "C"... etc.
    regex_string_iterator tileIterator(place_cmd.begin(), place_cmd.end(), tileExpr);
    string tileToken = tileIterator->str();
    // remove whitespaces
    tileToken.erase(remove_if(tileToken.begin(), tileToken.end(),
                              ::isspace), tileToken.end());

    // extract the position token, e.g. "A1", "B0"
    regex_string_iterator posIterator(place_cmd.begin(), place_cmd.end(), posExpr);
    string posToken = posIterator->str();


    if(!tileToken.empty() && !posToken.empty()) {
        tokens.insert(tokens.end(), {tileToken, posToken});
    }

    return tokens;
}

std::array<int, 2> convertPosTokenToInt(string position) {
    regex posExpr("[A-Z]([0-9]+)");
    int row = -1;
    int col = -1;

    // macthing <row><col> e.g.: B10
    if(std::regex_match(position, posExpr)) {

        try {
            row = position[0] - 'A';

            string colStr = position.substr(1);

            col = stoi(colStr);
        } catch (std::exception &err) {
            // reset row and col to -1 as the position string is of invalid format
            row = -1;
            col = -1;
        }
    }

    return {row, col};
}

string convertRowColToPosToken(int row, int col) {
    string position;
    char rowLetter = 'A' +  row;
    char colChar = '0' + col;

    position.append({rowLetter, colChar});

    return position;
}
