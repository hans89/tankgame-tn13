#ifndef __TANKGAME_BASEMAP__
#define __TANKGAME_BASEMAP__ 


#include <vector>

#include "IMap.h"
#include "MapInfo.h"
#include "BaseMapObject.h"

class BaseMap : public IMap {
private:
  MapInfo _mapInfo;
public:
  #pragma region IMapImplementation
  int getWidth() const;
  int getHeight() const;

  char operator()(int x, int y) const;

  bool isEmptySpace(int x, int y) const;
  bool isWater(int x, int y) const;
  bool isBlock(int x, int y) const;
  bool isBridge(int x, int y) const;
  bool isTank(int x, int y, char playerId) const;
  bool isHeadquarter(int x, int y, char playerId) const;
  bool isHeadquarter(int x, int y) const;

  #pragma endregion

  #pragma region ModelPreservedInterfaces
  char& operator()(int x, int y);

  void remove(BaseMapObject* obj);

  void move(BaseMapObject* obj, const std::pair<int,int>& newPos);

  BaseMap(const MapInfo& mapInfo);
  #pragma endregion
};

#endif