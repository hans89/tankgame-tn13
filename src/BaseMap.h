#ifndef __TANKGAME_BASEMAP__
#define __TANKGAME_BASEMAP__ 

#include <string>

#include "IMap.h"
#include "BaseTank.h"

class BaseMap : public IMap {
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

  bool isEmptySpace(int x, int y) const {

  }

  #pragma endregion

  #pragma region ControllerImplementation
  char& operator()(int x, int y) {
    return charMap[y][x];
  }

  void remove(BaseMapObject* obj) {
    pair<int,int> lastPos = obj->getPosition();

    obj->removeFromMap();

    (*this)(lastPos.first, lastPos.second) = IMap::LAND;
  }

  void move(BaseMapObject* obj, const pair<int,int>& newPos) {
    pair<int,int> lastPos = obj->getPosition();
    obj->move(newPos);

    (*this)(lastPos.first, lastPos.second) = IMap::LAND;
    (*this)(newPos.first, newPos.second) = obj->getMapID();
  }

  BaseMap(const vector<string>& cMap) : charMap(cMap) {}
  #pragma endregion
};

#endif