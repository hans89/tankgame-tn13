#ifndef __TANKGAME_BASEMAP__
#define __TANKGAME_BASEMAP__ 

#include "IMap.h"

class BaseMap : IMap {
private:
  vector<string> charMap;
public:
  #pragma region IMapImplementation
  int getWidth() const {
    return charMap[0].size();
  }

  // return the height (y-coordinate) of this map
  int getHeight() const {
    return charMap.size();
  }

  // return the cell value as char at (x,y)
  // (0,0) is at the left-top corner
  char operator()(int x, int y) const {
    return charMap[y][x];
  }
  #pragma endregion

  #pragma region ControllerImplementation
  char& operator()(int x, int y) {
    return charMap[y][x];
  }

  BaseMap(const vector<string>& cMap) : charMap(cMap) {}
  #pragma endregion
};

#endif