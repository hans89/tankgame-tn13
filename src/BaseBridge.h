#ifndef __TANKGAME_BASEBRIGE__
#define __TANKGAME_BASEBRIGE__

#include <utility>
#include "BaseMapObject.h"
#include "IBridge.h"

using namespace std;

class BaseBridge : public BaseMapObject, public IBridge {
private:
  static int _mapID;
  static int _defaultHP;
public:
  #pragma region IMapObjectImplementation
  
  char getMapID() const {
    return getID();
  }

  #pragma endregion

  #pragma region ModelPreservedInterfaces 

  BaseBridge(const pair<int,int>& pos) : BaseMapObject(_defaultHP, pos) {}

  static int getID() {
    return _mapID;
  }

  static int getDefaultHP() {
    return _defaultHP;
  }

  #pragma endregion
};

int BaseBridge::_mapID = 0;
int BaseBridge::_defaultHP = 0;

#endif