#ifndef __TANKGAME_BASEHEADQUARTER__
#define __TANKGAME_BASEHEADQUARTER__

#include <utility>
#include "BaseMapObject.h"
#include "../IHeadquarter.h"

class BaseHeadquarter : public BaseMapObject, public IHeadquarter {
protected:
  char _mapID;
public:
  #pragma region IMapObjectImplementation
  
  char getMapID() const {
    return _mapID;
  }

  #pragma endregion

  #pragma region ModelPreservedInterfaces 

  BaseHeadquarter(char mapID, int hp, const std::pair<int,int>& pos)
     : BaseMapObject(hp, pos), _mapID(mapID) {}

  #pragma endregion
};

#endif