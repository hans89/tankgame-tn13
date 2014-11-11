#ifndef __TANKMAP_ISPRING__
#define __TANKMAP_ISPRING__


#include "IMapObject.h"

class ISpring : virtual public IMapObject {
protected:
  // prevent deletion through this interface
  virtual ~ISpring() {}
public:
  virtual int getHPIncrease() const = 0;
};

#endif