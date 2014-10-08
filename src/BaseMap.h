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

  bool isWalkable(int x, int y) const {

  }

  #pragma endregion

  #pragma region ControllerImplementation
  char& operator()(int x, int y) {
    return charMap[y][x];
  }

  void removeTank(BaseTank* tank) {
    pair<int,int> curPost = tank->getPosition();

    (*this)(curPost.first, curPost.second) = IMap::LAND;
  }

  void moveTank(BaseTank* tank, const pair<int,int>& newPost) {
    pair<int,int> curPost = tank->getPosition();
    tank->move(newPost);

    (*this)(curPost.first, curPost.second) = IMap::LAND;
    (*this)(newPost.first, newPost.second) = tank->getOwner()->getPlayerMapID();
  }

  BaseMap(const vector<string>& cMap) : charMap(cMap) {}
  #pragma endregion
};

#endif