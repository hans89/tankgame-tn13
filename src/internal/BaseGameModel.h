#ifndef __TANKGAME_BASEGAMEMODEL__
#define __TANKGAME_BASEGAMEMODEL__ 

#include <list>
#include <vector>
#include <map>
#include <cstdlib>

#include "../IGameInfo.h"
#include "BaseMap.h"
#include "BasePlayerInfo.h"
#include "BaseTank.h"
#include "BaseBlock.h"
#include "BaseBridge.h"
#include "BaseSpring.h"

using namespace std;

class BaseGameModel : public IGameInfo {
public:

  #pragma region IGameInfoImplementation
  IMap* getMap() const;

  list<IBridge*> getOnMapBridges() const;
  list<IBlock*> getOnMapBlocks() const;
  list<ISpring*> getOnMapSprings() const;

  IBridge* getBridge(int x, int y) const;
  IBlock* getBlock(int x, int y) const;
  ISpring* getSpring(int x, int y) const;
  ITank* getTank(int x, int y) const;
  
  vector<IPlayerInfo*> getPlayersInfo() const;
  IPlayerInfo* getPlayerByID(char id) const;
  int getMaximumNumberOfTurn() const;

  // get the player end-game info
  PlayerEndGameInfo getPlayerEndGameInfo(IPlayerInfo* player) const;
  #pragma endregion

  #pragma region ModelPreservedInterfaces 
  // return -1 if ties
  int getGameFinalResult(const vector<PlayerEndGameInfo>& egInfos) const;

  bool isEndGame() const;

  const BaseMap* getBaseMap() const;

  IPlayer* registerPlayer(IPlayer* newPlayer);

  bool isValidMove(IPlayerInfo* playerInfo, const Command& move) const;
  vector<pair<int, int> > 
    applyMove(IPlayerInfo* playerInfo, const Command& move);


  bool isPossibleMove(IPlayerInfo* playerInfo, const Command& move) const;  

  pair<CommandInfo*, CommandInfo*> 
    tryMove(CommandInfo& move1, CommandInfo& move2, bool& dependent);

  vector<pair<int, int> > applyMove(const CommandInfo& move1, 
                                    const CommandInfo& move2);

  void nextTurnCount();
  int getCurrentTurnCount() const; 

  void applyNewTurnAutoEffects();

  BaseGameModel(const MapInfo& info);

  ~BaseGameModel();
  #pragma endregion;

protected:
  BaseMap* _map;
  vector<BaseBlock*> _blocks;
  vector<BaseBridge*> _bridges;
  vector<BaseSpring*> _springs;

  const MapInfo _mapInfo;
  
  map<char, pair<int,int> > _headquarters;
  int _nextRegisterPlayer;

  vector<BasePlayerInfo*> _playersInfo;
  map<char, BasePlayerInfo*> _playersInfoIDMap;
  map<char, BaseHeadquarter*> _headquarterMap;

  // maintaining public interfaces to internal objects
  // to support IGameInfo interfaces
  vector<IPlayerInfo*> _iPlayersInfo;
  list<IBlock*> _onMapBlocks;
  list<IBridge*> _onMapBridges;
  list<ISpring*> _onMapSprings;

  int _currentTurn;
  int _totalAmmo;
};

#endif