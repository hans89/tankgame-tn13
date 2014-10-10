
#include "BaseMap.h"

#pragma region IMapImplementation
int BaseMap::getWidth() const {
  return charMap[0].size();
}

// return the height (y-coordinate) of this map
int BaseMap::getHeight() const {
  return charMap.size();
}

// return the cell value as char at (x,y)
// (0,0) is at the left-top corner
char BaseMap::operator()(int x, int y) const {
  return charMap[y][x];
}

bool BaseMap::isEmptySpace(int x, int y) const {
  return false;
}
#pragma endregion

#pragma region ControllerImplementation
char& BaseMap::operator()(int x, int y) {
  return charMap[y][x];
}

void BaseMap::remove(BaseMapObject* obj) {
  std::pair<int,int> lastPos = obj->getPosition();

  obj->removeFromMap();

  //(*this)(lastPos.first, lastPos.second) = IMap::LAND;
}

void BaseMap::move(BaseMapObject* obj, const std::pair<int,int>& newPos) {
  std::pair<int,int> lastPos = obj->getPosition();
  obj->move(newPos);

  //(*this)(lastPos.first, lastPos.second) = IMap::LAND;
  (*this)(newPos.first, newPos.second) = obj->getMapID();
}

BaseMap::BaseMap(const std::vector<std::string>& cMap) : charMap(cMap) {}
#pragma endregion