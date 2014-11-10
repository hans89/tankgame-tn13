#ifndef __TANKGAME_IPLAYERINFO__
#define __TANKGAME_IPLAYERINFO__ 

#include <list>

#include "Command.h"
#include "IHeadquarter.h"

/**
 * Interface to access player's information
 */

struct PlayerEndGameInfo
{
  int totalTanks;
  int totalHP;
  int totalDistance;
  int headquarterHP;

  PlayerEndGameInfo(int t = 0, int h = 0, int d = 0, int headHP = 0) 
    : totalTanks(t), totalHP(h), totalDistance(d), headquarterHP(headHP) {}
};

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

  virtual IHeadquarter* getHeadquarter() const = 0;

  // get the last move by the player
  virtual CommandInfo getLastMove() const = 0;

  // is this player playable
  virtual bool isPlayable() const = 0;
};

#endif