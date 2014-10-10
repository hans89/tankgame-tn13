#ifndef __TANKGAME_IMAPOBJECT__
#define __TANKGAME_IMAPOBJECT__

#include <utility>

class IMapObject {
protected:
  virtual ~IMapObject() {}
public:
  // return the HP in range [0, MAXHP]
  virtual int getHP() const = 0;

  // return the current position, if isOnMap() == false, return (-1,-1)
  virtual std::pair<int, int> getPosition() const = 0;

  // return if the Object in on map
  virtual bool isOnMap() const = 0;

  // return the char ID of this object on the map
  virtual char getMapID() const = 0;
};
#endif