#ifndef __TANKGAME_IBRIDGE__
#define __TANKGAME_IBRIDGE__

#include "IMapObject.h"

class IBridge : virtual public IMapObject {
protected:
  // prevent deletion through this interface
  virtual ~IBridge() {}
public:

};

#endif