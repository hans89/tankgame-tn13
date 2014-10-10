#ifndef __TANKGAME_ITANK__
#define __TANKGAME_ITANK__

#include "IMapObject.h"
#include "IPlayerInfo.h"

class ITank : virtual public IMapObject {
protected:
  // prevent deletion through this interface
  virtual ~ITank() {}
public:
  // return the amount of ammo left, in range [0, MAXAMMO]
  virtual int getAmmoNumber() const = 0;

  // return the maximum range, in range [0, MAXRANGE]
  virtual int getRange() const = 0;

  // return if HP != 0
  virtual bool isAlive() const = 0;

  // get the owner of this tank
  virtual IPlayerInfo* getOwner() const = 0;
};

#endif