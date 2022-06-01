//
// Created by Sao Kanneh on 13/5/22.
//

#ifndef APTA_A2_COMMONREGEXPATTERNS_H
#define APTA_A2_COMMONREGEXPATTERNS_H

#include <regex>

using std::regex;
using std::regex_match;

class CommonRegexPatterns {
  public:
    static CommonRegexPatterns getInstance();

    regex getPlaceTileExpr();
    regex getPlacePosRegex();


  private:
    CommonRegexPatterns();




};


#endif //APTA_A2_COMMONREGEXPATTERNS_H
