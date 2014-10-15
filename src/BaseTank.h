#ifndef __TANKGAME_BASETANK__
#define __TANKGAME_BASETANK__ 

#include "ITank.h"
#include "IPlayerInfo.h"
#include "BaseMapObject.h"

class BaseTank : public BaseMapObject, public ITank {
public:
  #pragma region IMapObjectImplementation

  char getMapID() const;
  #pragma endregion

  #pragma region ITankImplementation
  
  int getAmmoNumber() const;
  int getRange() const;
  bool isAlive() const;
  IPlayerInfo* getOwner() const;

  #pragma endregion

  #pragma region ModelPreservedInterface

  void decreaseAmmo();
  
  BaseTank(int hp, int ammo, int range, 
      const std::pair<int,int>& pos, IPlayerInfo* owner);
    
  #pragma endregion

protected:
  int _ammo, _range;
  IPlayerInfo* _ownerInfo;
};
#endif