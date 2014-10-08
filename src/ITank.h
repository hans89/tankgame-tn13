#ifndef __TANKGAME_ITANK__
#define __TANKGAME_ITANK__

#include "IPlayerInfo.h"

class ITank {
protected:
  // prevent deletion through this interface
  virtual ~ITank() {}
public:
  // return the HP in range [0, MAXHP]
  virtual int getHP() const = 0;
  
  // return the amount of ammo left, in range [0, MAXAMMO]
  virtual int getAmmoNumber() const = 0;

  // return the maximum range, in range [0, MAXRANGE]
  virtual int getRange() const = 0;

  // return the current position, if isAlive == false, return (-1,-1)
  virtual pair<int, int> getPosition() const = 0;

  // return if HP != 0
  virtual bool isAlive() const = 0;

  // get the owner of this tank
  virtual IPlayerInfo* getOwner() const = 0;
};

#endif