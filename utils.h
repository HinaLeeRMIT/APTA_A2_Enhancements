//
// Created by sheku on 11/05/2022.
//

#ifndef APTA_A2_UTILS_H
#define APTA_A2_UTILS_H

#include <vector>
#include <string>
#include <regex>
#include <algorithm>
#include <array>
#include <exception>

using std::vector;
using std::string;
using std::regex;
using std::regex_iterator;
using std::remove_if;

typedef regex_iterator<string::iterator> regex_string_iterator;

/**
 * splits place command and stores tile letter at index 0, grid position at index 1
 * NOTE: this function does not validate the place command.
 *
 * @param place_cmd - the place command of expected format "place [tile] at [position]"
 * @return vector of string with index 0 being the letter, and 1 being the grid position
 */
vector<string> splitPlaceCommand(string place_cmd);

/**
 * @params position - the position token of format [row_letter][col_num]
 * @return int array of [row,col]; row and col will be -1 if position is invalid
 */
std::array<int,2> convertPosTokenToInt(std::basic_string<char> position);

string convertRowColToPosToken(int row, int col);


#endif //APTA_A2_UTILS_H
