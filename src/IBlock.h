#ifndef __TANKMAP_IBLOCK__
#define __TANKMAP_IBLOCK__


#include "IMapObject.h"

class IBlock : virtual public IMapObject {
protected:
  // prevent deletion through this interface
  virtual ~IBlock() {}
public:
};

#endif