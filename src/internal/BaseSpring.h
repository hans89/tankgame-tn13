#ifndef __TANKGAME_BASESPRING__
#define __TANKGAME_BASESPRING__

#include "BaseMapObject.h"
#include "../ISpring.h"
#include "../IGameInfo.h"

class BaseSpring : public BaseMapObject, public ISpring {
protected:
  char _mapID;
  const int _HPIncrease;
public:
  #pragma region IMapObjectImplementation
  
  char getMapID() const;

  int getHPIncrease() const;
  #pragma endregion

  #pragma region ModelPreservedInterfaces 

  void regen(IGameInfo* gameInfo) const;

  BaseSpring(char mapID, int hp, const std::pair<int,int>& pos, int hpInc)
     : BaseMapObject(hp, pos), _mapID(mapID), _HPIncrease(hpInc) {}

  #pragma endregion
};

#endif