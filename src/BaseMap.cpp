#include <string>
#include "BaseMap.h"

#pragma region IMapImplementation
int BaseMap::getWidth() const {
  return _mapInfo.charMap[0].size();
}

// return the height (y-coordinate) of this map
int BaseMap::getHeight() const {
  return _mapInfo.charMap.size();
}

// return the cell value as char at (x,y)
// (0,0) is at the left-top corner
char BaseMap::operator()(int x, int y) const {
  return _mapInfo.charMap[y][x];
}

bool BaseMap::isEmptySpace(int x, int y) const {
  return _mapInfo.charMap[y][x] == _mapInfo.landID;
}

bool BaseMap::isWater(int x, int y) const {
  return _mapInfo.charMap[y][x] == _mapInfo.waterID;
}
bool BaseMap::isBlock(int x, int y) const {
  return _mapInfo.blockIDs.find(_mapInfo.charMap[y][x]) != std::string::npos;
}

bool BaseMap::isBridge(int x, int y) const {
  return _mapInfo.bridgeIDs.find(_mapInfo.charMap[y][x]) != std::string::npos;
}

bool BaseMap::isTank(int x, int y, char playerId) const {
  return _mapInfo.charMap[y][x] == playerId 
    && _mapInfo.playerIDs.find(_mapInfo.charMap[y][x]) != std::string::npos;
}

bool BaseMap::isHeadquarter(int x, int y, char playerId) const {

  int playerIDindex = _mapInfo.playerIDs.find(playerId);

  if (playerIDindex == std::string::npos)
    return false;

  return _mapInfo.headquarterIDs[playerIDindex] == _mapInfo.charMap[y][x];
}

#pragma endregion

#pragma region ControllerImplementation
char& BaseMap::operator()(int x, int y) {
  return _mapInfo.charMap[y][x];
}

void BaseMap::remove(BaseMapObject* obj) {
  std::pair<int,int> lastPos = obj->getPosition();

  obj->removeFromMap();

 _mapInfo.charMap[lastPos.second][lastPos.first] = _mapInfo.landID;
}

void BaseMap::move(BaseMapObject* obj, const std::pair<int,int>& newPos) {
  std::pair<int,int> lastPos = obj->getPosition();
  obj->move(newPos);

  _mapInfo.charMap[lastPos.second][lastPos.first] = _mapInfo.landID;
  _mapInfo.charMap[newPos.second][newPos.first] = obj->getMapID();
}

BaseMap::BaseMap(const MapInfo& mapInfo) : _mapInfo(mapInfo) {}
#pragma endregion