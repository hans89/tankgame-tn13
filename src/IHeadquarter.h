#ifndef __TANKGAME_IHEADQUARTER__
#define __TANKGAME_IHEADQUARTER__

#include "IMapObject.h"

class IPlayerInfo;

class IHeadquarter : virtual public IMapObject {
protected:
  // prevent deletion through this interface
  virtual ~IHeadquarter() {}
public:

};

#endif