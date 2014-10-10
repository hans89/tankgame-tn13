#include "utils.h"

vector<string>& Utils::split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
      if (!item.empty())
          elems.push_back(item);
    }
    return elems;
}


vector<string> Utils::split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

/**
 * Parse (int, int) string into pair<int,int>
 */
pair<int, int> Utils::parseIntPair(string intPair) {

  // replace with space
  for (int i = 0; i < intPair.size(); i++) {
    if (intPair[i] == '(' || intPair[i] == ')' || intPair[i] == ',')
      intPair[i] = ' ';
  }

  istringstream iss(intPair);

  int f = 0, s = 0;

  iss >> f >> s;

  return pair<int,int>(f,s);
}

int Utils::parseInt(string intNum) {
  return atoi(intNum.c_str());
}