#ifndef __TANKGAME_IGAMEINFO__
#define __TANKGAME_IGAMEINFO__

#include <vector>
#include <list>

#include "IMap.h"
#include "IPlayer.h"
#include "IBlock.h"
#include "IBridge.h"


class IGameInfo {
protected:
	// prevent deletion through this interface
	virtual ~IGameInfo() {};
public:
	// get the map
	virtual IMap* getMap() const = 0;

  // get the available bridges on map
  virtual std::list<IBridge*> getOnMapBridges() const = 0;

  // get the available blocks on map
  virtual std::list<IBlock*> getOnMapBlocks() const = 0;

	// get list of current players infomation
	virtual std::vector<IPlayerInfo*> getPlayersInfo() const = 0;

  // get PlayerInfo from Map ID. Return NULL if not found
  virtual IPlayerInfo* getPlayerByID(char id) const = 0;

  virtual int getMaximumNumberOfTurn() const = 0;
}; 
#endif