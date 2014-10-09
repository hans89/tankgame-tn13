#ifndef __TANKGAME_BASETANK__
#define __TANKGAME_BASETANK__ 

#include "ITank.h"
#include "BaseMapObject.h"

class BaseTank : public BaseMapObject, public ITank {
protected:
  int _ammo, _range;
  IPlayerInfo* _ownerInfo;
public:

  #pragma region IMapObjectImplementation
  // return the char ID of this object on the map
  char getMapID() const {
    return _ownerInfo->getPlayerMapID();
  }
  #pragma endregion

  #pragma region ITankImplementation
  
  int getAmmoNumber() const {
    return _ammo;
  }

  int getRange() const {
    return _range;
  }

  bool isAlive() const {
    return _HP != 0;
  }

  IPlayerInfo* getOwner() const {
    return _ownerInfo;
  }

  #pragma endregion

  #pragma region ModelPreservedInterface

  void decreaseAmmo() {
    if (_ammo > 0)
      _ammo--;
  }
  
  BaseTank(int hp, int ammo, int range, const pair<int,int>& pos, IPlayerInfo* owner)
    : BaseMapObject(hp, pos), _ammo(ammo), _range(range), _ownerInfo(owner) {}
    
  #pragma endregion
};


#endif