//
// Created by Sao Kanneh on 13/5/22.
//

#include "CommonRegexPatterns.h"

regex CommonRegexPatterns::getPlaceTileExpr() {
    return std::regex("[\\s][A-Z][\\s]");
}

regex CommonRegexPatterns::getPlacePosRegex() {
    return std::regex("[A-Z]([0-9]+)");
}

CommonRegexPatterns CommonRegexPatterns::getInstance() {
    static CommonRegexPatterns instance;
    return instance;
}

CommonRegexPatterns::CommonRegexPatterns() = default;
