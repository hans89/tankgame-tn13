#ifndef __TANKGAME_BASEMAP__
#define __TANKGAME_BASEMAP__ 


#include <vector>

#include "IMap.h"
#include "MapInfo.h"
#include "BaseMapObject.h"

class BaseMap : public IMap {
public:
  #pragma region IMapImplementation
  int getWidth() const;
  int getHeight() const;
  bool isOnMap(const std::pair<int,int>& pos) const;

  char operator()(int x, int y) const;

  bool isEmptySpace(int x, int y) const;
  bool isEmptySpace(char id) const;
  bool isWater(int x, int y) const;
  bool isWater(char id) const;
  bool isBlock(int x, int y) const;
  bool isBlock(char id) const;
  bool isBridge(int x, int y) const;
  bool isBridge(char id) const;
  bool isTank(int x, int y, char playerId) const;
  bool isTank(int x, int y) const;
  bool isTank(char id) const;
  bool isHeadquarter(int x, int y, char playerId) const;
  bool isHeadquarter(int x, int y) const;
  bool isHeadquarter(char id) const;

  #pragma endregion

  #pragma region ModelPreservedInterfaces
  char& operator()(int x, int y);

  void popLastLayerAt(int x, int y);
  void pushLayerAt(int x, int y, char c);
  const std::string& getLayersAt(int x, int y) const;

  void remove(BaseMapObject* obj);
  void move(BaseMapObject* obj, const std::pair<int,int>& newPos);

  const MapInfo& getMapInfo() const;
  
  BaseMap(const MapInfo& mapInfo);
  #pragma endregion

private:
  const MapInfo _mapInfo;
  std::vector<std::vector<std::string> > _layeredMap;
};

#endif