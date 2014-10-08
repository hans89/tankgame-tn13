#ifndef __TANKGAME_IGAMEINFO__
#define __TANKGAME_IGAMEINFO__

#include "IMap.h"
#include "IPlayer.h"

class IGameInfo {
protected:
	// prevent deletion through this interface
	virtual ~IGameInfo() {};
public:
	// get the map
	virtual IMap* getMap() const = 0;

	// get list of current players infomation
	virtual vector<IPlayerInfo*> getPlayersInfo() const = 0;
}; 
#endif