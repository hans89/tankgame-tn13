#ifndef __TANKGAME_BASEGAMEMODEL__
#define __TANKGAME_BASEGAMEMODEL__ 

#include <list>
#include <vector>
#include <map>
#include <cstdlib>

#include "IGameInfo.h"
#include "BaseMap.h"
#include "BasePlayerInfo.h"
#include "BaseTank.h"
#include "BaseBlock.h"
#include "BaseBridge.h"

using namespace std;

class BaseGameModel : public IGameInfo {
protected:
  BaseMap* _map;
  vector<BaseBlock*> _blocks;
  vector<BaseBridge*> _bridges;

  const MapInfo _mapInfo;
  
  map<char, pair<int,int> > _headquarters;
  int _nextRegisterPlayer;

  vector<BasePlayerInfo*> _playersInfo;
  map<char, BasePlayerInfo*> _playersInfoIDMap;

  // maintaining public interfaces to internal objects
  // to support IGameInfo interfaces
  vector<IPlayerInfo*> _iPlayersInfo;
  list<IBlock*> _onMapBlocks;
  list<IBridge*> _onMapBridges;

public:

  #pragma region IGameInfoImplementation
  IMap* getMap() const;

  list<IBridge*> getOnMapBridges() const;
  list<IBlock*> getOnMapBlocks() const;

  vector<IPlayerInfo*> getPlayersInfo() const;
  IPlayerInfo* getPlayerByID(char id) const;
  #pragma endregion

  #pragma region ModelPreservedInterfaces 
  bool isEndGame();

  const BaseMap* getBaseMap() const;

  IPlayer* registerPlayer(IPlayer* newPlayer);

  bool isValidMove(IPlayer* player, const Command& move);

  vector<pair<int, int> > applyMove(IPlayer* player, const Command& move);

  BaseGameModel(const MapInfo& info);

  ~BaseGameModel();
  #pragma endregion;
};

#endif