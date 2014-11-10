#ifndef __TANKGAME_BASETANK__
#define __TANKGAME_BASETANK__ 

#include "../ITank.h"
#include "../IPlayerInfo.h"
#include "BaseMapObject.h"

class BaseTank : public BaseMapObject, public ITank {
public:
  #pragma region IMapObjectImplementation

  char getMapID() const;
  #pragma endregion

  #pragma region ITankImplementation
  
  int getAmmoNumber() const;
  int getRange() const;
  int getMaxHP() const;
  bool isAlive() const;
  bool hasAmmo() const;
  IPlayerInfo* getOwner() const;

  #pragma endregion

  #pragma region ModelPreservedInterface

  void decreaseAmmo();
  void increaseHP(int amount = 1); 
  BaseTank(int hp, int ammo, int range, 
      const std::pair<int,int>& pos, IPlayerInfo* owner);
    
  #pragma endregion

protected:
  int _ammo, _range;
  const int _maxHP;
  IPlayerInfo* _ownerInfo;
};
#endif