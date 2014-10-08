#ifndef __TANKGAME_BASETANK__
#define __TANKGAME_BASETANK__ 

#include "ITank.h"

class BaseTank : ITank {
private:
  int _HP, _ammo, _range;
  pair<int, int> _pos;
  IPlayerInfo* _ownerInfo;
public:

  #pragma region ITankImplementation
  
  int getHP() const {
    return _HP;
  }
  
  int getAmmoNumber() const {
    return _ammo;
  }

  int getRange() const {
    return _range;
  }

  pair<int, int> getPosition() const {
    if (isAlive())
      return _pos;
    return nopos;
  }

  bool isAlive() const {
    return _HP != 0;
  }

  IPlayerInfo* getOwner() const {
    return _ownerInfo;
  }

  #pragma endregion

  #pragma region ControllerImplementation
  void decreaseHP() {
    if (_HP > 0)
      _HP--;
  }

  void decreaseAmmo() {
    if (_ammo > 0)
      _ammo--;
  }

  void kill() {
    _HP = 0;
  }

  void move(const pair<int, int>& newPost) {
    _pos = newPost;
  }
  
  BaseTank(int hp, int ammo, int range, pair<int,int> pos, IPlayerInfo* owner)
    : _HP(hp), _ammo(ammo), _range(range), _pos(pos), _ownerInfo(owner) {}
    
  #pragma endregion
};


#endif