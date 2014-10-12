#ifndef __TANKGAME_IPLAYERINFO__
#define __TANKGAME_IPLAYERINFO__ 

#include <list>

#include "Command.h"

/**
 * Interface to access player's information
 */
class IPlayerInfo
{
protected:
  // prevent deletion through this interface
  virtual ~IPlayerInfo() {}
public:
  // return the ID as a character
  // this character presents the player's assets on the map
  virtual char getPlayerMapID() const = 0;

  // get list of running tanks of the player
  virtual std::list<ITank*> getAliveTanks() const = 0;

  // get list of dead tanks of the player
  virtual std::list<ITank*> getDeadTanks() const = 0;

  // get the position of the headquarter of the player
  virtual std::pair<int, int> getHeadquarterPosition() const = 0;

  // get the last move by the player
  // virtual Command getLastMove() const = 0;

  // is this player playable
  virtual bool isPlayable() const = 0;
};
#endif