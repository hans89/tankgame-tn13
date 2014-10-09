#ifndef __TANKGAME_BASEBLOCK__
#define __TANKGAME_BASEBLOCK__

#include <utility>
#include "BaseMapObject.h"
#include "IBlock.h"

using namespace std;

class BaseBlock : public BaseMapObject, public IBlock {
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

  BaseBlock(const pair<int,int>& pos) : BaseMapObject(_defaultHP, pos) {}

  static int getID() {
    return _mapID;
  }

  static int getDefaultHP() {
    return _defaultHP;
  }

  #pragma endregion
};

int BaseBlock::_mapID = 0;
int BaseBlock::_defaultHP = 0;

#endif