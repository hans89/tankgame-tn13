#ifndef __TANKGAME_BASEBLOCK__
#define __TANKGAME_BASEBLOCK__

#include <utility>
#include "BaseMapObject.h"
#include "IBlock.h"

class BaseBlock : public BaseMapObject, public IBlock {
protected:
  char _mapID;
public:
  #pragma region IMapObjectImplementation
  
  char getMapID() const {
    return _mapID;
  }

  #pragma endregion

  #pragma region ModelPreservedInterfaces 

  BaseBlock(char id, int hp, const std::pair<int,int>& pos) 
    : BaseMapObject(hp, pos), _mapID(id) {}

  #pragma endregion
};

#endif