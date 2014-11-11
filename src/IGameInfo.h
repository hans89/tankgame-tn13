#ifndef __TANKGAME_IGAMEINFO__
#define __TANKGAME_IGAMEINFO__

#include <vector>
#include <list>

#include "IMap.h"
#include "IPlayer.h"
#include "IBlock.h"
#include "IBridge.h"
#include "ISpring.h"
#include "IPlayerInfo.h"


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

  // get the available springs on map
  virtual std::list<ISpring*> getOnMapSprings() const = 0;

  // get the block at the designated position, if any. Return NULL if nothing.
  virtual IBlock* getBlock(int x, int y) const = 0;

  // get the bridge at the designated position, if any. Return NULL if nothing.
  virtual IBridge* getBridge(int x, int y) const = 0;

  // get the tank at the designated position, if any. Return NULL if nothing.
  virtual ITank* getTank(int x, int y) const = 0;

  // get the spring at the designated position, if any. Return NULL if nothing.
  virtual ISpring* getSpring(int x, int y) const = 0;

	// get list of current players infomation
	virtual std::vector<IPlayerInfo*> getPlayersInfo() const = 0;

  // get PlayerInfo from Map ID. Return NULL if not found
  virtual IPlayerInfo* getPlayerByID(char id) const = 0;

  virtual int getMaximumNumberOfTurn() const = 0;

  // get the player end-game info
  virtual PlayerEndGameInfo getPlayerEndGameInfo(IPlayerInfo* player) const = 0;
}; 
#endif