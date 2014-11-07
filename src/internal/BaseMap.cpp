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

bool BaseMap::isOnMap(const std::pair<int,int>& pos) const {
  return (pos.first >= 0 && pos.first < getWidth()
          && pos.second >= 0 && pos.second < getHeight());
}

// return the cell value as char at (x,y)
// (0,0) is at the left-top corner
char BaseMap::operator()(int x, int y) const {
  const std::string& layer = _layeredMap[y][x];
  return layer[layer.size()-1];
}

bool BaseMap::isEmptySpace(int x, int y) const {
  return isOnMap(std::pair<int,int>(x,y)) && isEmptySpace((*this)(x,y));
}

bool BaseMap::isEmptySpace(char id) const {
  return id == _mapInfo.landID;
}

bool BaseMap::isWater(int x, int y) const {
  return isOnMap(std::pair<int,int>(x,y)) && isWater((*this)(x,y));
}

bool BaseMap::isWater(char id) const {
  return id == _mapInfo.waterID;
}

bool BaseMap::isBlock(int x, int y) const {
  return isOnMap(std::pair<int,int>(x,y)) && isBlock((*this)(x,y));
}

bool BaseMap::isBlock(char id) const {
  return _mapInfo.blockIDs.find(id) != std::string::npos;
}

bool BaseMap::isBridge(int x, int y) const {
  return isOnMap(std::pair<int,int>(x,y)) && isBridge((*this)(x,y));
}

bool BaseMap::isBridge(char id) const {
  return _mapInfo.bridgeIDs.find(id) != std::string::npos;
}

bool BaseMap::isTank(int x, int y, char playerId) const {
  char c;
  return isOnMap(std::pair<int,int>(x,y)) && (c = (*this)(x,y)) == playerId
      && isTank(c);
}

bool BaseMap::isTank(int x, int y) const {
  return isOnMap(std::pair<int,int>(x,y)) && isTank((*this)(x,y));
}

bool BaseMap::isTank(char id) const {
  return _mapInfo.playerIDs.find(id) != std::string::npos;
}

bool BaseMap::isHeadquarter(int x, int y, char playerId) const {

  int playerIDindex = _mapInfo.playerIDs.find(playerId);

  if (playerIDindex == std::string::npos)
    return false;

  return isOnMap(std::pair<int,int>(x,y)) 
      && _mapInfo.headquarterIDs[playerIDindex] == (*this)(x,y);
}

bool BaseMap::isHeadquarter(int x, int y) const {
  return isOnMap(std::pair<int,int>(x,y)) && isHeadquarter((*this)(x,y));
}

bool BaseMap::isHeadquarter(char id) const {
 return _mapInfo.headquarterIDs.find(id) != std::string::npos; 
}


#pragma endregion

#pragma region ControllerImplementation
char& BaseMap::operator()(int x, int y) {
  std::string& layer = _layeredMap[y][x];
  return layer[layer.size()-1];
}

void BaseMap::popLastLayerAt(int x, int y) {
  std::string& layer = _layeredMap[y][x];
  layer.erase(layer.size()-1);
}

void BaseMap::pushLayerAt(int x, int y, char c) {
  _layeredMap[y][x] += c;
}

const std::string& BaseMap::getLayersAt(int x, int y) const {
  return _layeredMap[y][x];
}

void BaseMap::remove(BaseMapObject* obj) {
  std::pair<int,int> lastPos = obj->getPosition();

  this->popLastLayerAt(lastPos.first, lastPos.second);
     
  obj->removeFromMap();
}

void BaseMap::move(BaseMapObject* obj, const std::pair<int,int>& newPos) {
  std::pair<int,int> lastPos = obj->getPosition();
  
  char topLayer = (*this)(lastPos.first, lastPos.second);

  this->pushLayerAt(newPos.first, newPos.second, topLayer);
  this->popLastLayerAt(lastPos.first, lastPos.second);
  
  obj->move(newPos);
}

const MapInfo& BaseMap::getMapInfo() const {
  return _mapInfo;
}

BaseMap::BaseMap(const MapInfo& mapInfo) : _mapInfo(mapInfo) {
  // set up layers
  int w = this->getWidth(),
        h = this->getHeight();

    _layeredMap.resize(h);

    for (int j = 0; j < h; j++) {
      _layeredMap[j].resize(w);

      for (int i = 0; i < w; i++) {

        char c = _mapInfo.charMap[j][i];
        std::string& currentLayers = _layeredMap[j][i];
        currentLayers = "";
        // add the non-top layers
        if (isEmptySpace(c) || isWater(c)) {
          // is land or water
        } else if (isBlock(c) || isTank(c) || isHeadquarter(c)) {
          currentLayers += _mapInfo.landID;
        } else if (isBridge(c)) {
          currentLayers += _mapInfo.waterID;
        }
        // at the top layer
        currentLayers += c;
      }
    }
}
#pragma endregion