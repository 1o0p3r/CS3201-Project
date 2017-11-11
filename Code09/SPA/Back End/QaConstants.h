#pragma once

#include <numeric>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <math.h>

using namespace std;

const string DELIMITER = ",";
const string WHITESPACE = " ";
const string SYNONYM = "synonym";
const string WILDCARD = "wildcard";
const string WILDCARD_SYMBOL = "_";
const int ARGONE = 0;
const int ARGTWO = 1;
const int TABLELOC = 0;
const int SYNVECLOC = 1;
const int SYNENTITY = 2;
const int SYNNAMELOC = 3;
const int SYNATRREFLOC = 4;
const int SYNPOS = 1;
const int VECINTERSECTION = 0;
const int TTMINDEX = 1;
const int STRINDEX = 2;
const int SAMETABLE = 0;
const int TWO_DISJOINT_TABLE = 1;
const int VECTRESULT = 1;
const int BOOLRESULT = 0;
const int NOSYNENTRY = -1;
const string SUCH_THAT = "suchThat";
const string EMPTY_STRING = "";
const int GETNORMALCLAUSE = 0;
const int GETHARDCLAUSE = 1;
const int LOWEST_RANK = floor(sqrt(INT_MAX)/2); //some random number