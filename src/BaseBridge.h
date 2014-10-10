#ifndef __TANKGAME_BASEBRIGE__
#define __TANKGAME_BASEBRIGE__

#include <utility>
#include "BaseMapObject.h"
#include "IBridge.h"

class BaseBridge : public BaseMapObject, public IBridge {
protected:
  char _mapID;
public:
  #pragma region IMapObjectImplementation
  
  char getMapID() const {
    return _mapID;
  }

  #pragma endregion

  #pragma region ModelPreservedInterfaces 

  BaseBridge(char id, int hp, const std::pair<int,int>& pos)
     : BaseMapObject(hp, pos), _mapID(id) {}


  #pragma endregion
};

#endif