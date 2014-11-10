#include "BaseTank.h"

#pragma region IMapObjectImplementation
// return the char ID of this object on the map
char BaseTank::getMapID() const {
  return _ownerInfo->getPlayerMapID();
}
#pragma endregion

#pragma region ITankImplementation

int BaseTank::getAmmoNumber() const {
  return _ammo;
}

int BaseTank::getRange() const {
  return _range;
}

int BaseTank::getMaxHP() const {
  return _maxHP;
}

bool BaseTank::isAlive() const {
  return _HP > 0;
}

bool BaseTank::hasAmmo() const {
  return _ammo > 0;
}

IPlayerInfo* BaseTank::getOwner() const {
  return _ownerInfo;
}
#pragma endregion

#pragma region ModelPreservedInterface

void BaseTank::increaseHP(int amount) {

  if (_HP < _maxHP) {
    _HP += amount;
    
    if (_HP > _maxHP)
      _HP = _maxHP;
  }
}

void BaseTank::decreaseAmmo() {
  if (_ammo > 0)
    _ammo--;
}

BaseTank::BaseTank(int hp, int ammo, int range, 
    const std::pair<int,int>& pos, IPlayerInfo* owner)
  : BaseMapObject(hp, pos), _ammo(ammo), _range(range),
    _ownerInfo(owner), _maxHP(hp) {}
  
#pragma endregion