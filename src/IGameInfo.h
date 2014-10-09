#ifndef __TANKGAME_IGAMEINFO__
#define __TANKGAME_IGAMEINFO__

#include <vector>

#include "IMap.h"
#include "IPlayer.h"
#include "IBlock.h"
#include "IBridge.h"

using namespace std;

class IGameInfo {
protected:
	// prevent deletion through this interface
	virtual ~IGameInfo() {};
public:
	// get the map
	virtual IMap* getMap() const = 0;

  // get the available bridges on map
  virtual list<IBridge*> getOnMapBridges() const = 0;

  // get the available blocks on map
  virtual list<IBlock*> getOnMapBlocks() const = 0;

	// get list of current players infomation
	virtual vector<IPlayerInfo*> getPlayersInfo() const = 0;

  // get PlayerInfo from Map ID. Return NULL if not found
  virtual IPlayerInfo* getPlayerByID(char id) const = 0;
}; 
#endif