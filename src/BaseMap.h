#ifndef __TANKGAME_BASEMAP__
#define __TANKGAME_BASEMAP__ 

#include <string>
#include <vector>

#include "IMap.h"
#include "BaseMapObject.h"

class BaseMap : public IMap {
private:
  std::vector<std::string> charMap;
public:
  #pragma region IMapImplementation
  
  int getWidth() const;

  int getHeight() const;

  char operator()(int x, int y) const;

  // return if the cell is on land, empty and can be occupied
  bool isEmptySpace(int x, int y) const;
  #pragma endregion

  #pragma region ModelPreservedInterfaces
  char& operator()(int x, int y);

  void remove(BaseMapObject* obj);

  void move(BaseMapObject* obj, const std::pair<int,int>& newPos);

  BaseMap(const std::vector<std::string>& cMap);
  #pragma endregion
};

#endif