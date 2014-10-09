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

#define STARTID 'A'

class BaseGameModel : public IGameInfo {
protected:
  BaseMap* _map;
  vector<BaseBlock*> _blocks;
  vector<BaseBridge*> _bridges;
  
  int _defaultHP, _defaultAmmo, _defaultRange;
  vector<pair<int,int> > _headquarters;

  char _newPlayerID;
  const char _startID;
  vector<BasePlayerInfo*> _playersInfo;
  map<char, BasePlayerInfo*> _playersInfoIDMap;

  // maintaining public interfaces to internal objects
  // to support IGameInfo interfaces
  vector<IPlayerInfo*> _iPlayersInfo;
  list<IBlock*> _onMapBlocks;
  list<IBridge*> _onMapBridges;

public:

  #pragma region IGameInfoImplementation
  // get the map
  IMap* getMap() const {
    return _map;
  }

  // get the available bridges on map
  list<IBridge*> getOnMapBridges() const {
    return _onMapBridges;
  }

  // get the available blocks on map
  list<IBlock*> getOnMapBlocks() const {
    return _onMapBlocks;
  }

  // get list of current players information
  vector<IPlayerInfo*> getPlayersInfo() const {
    return _iPlayersInfo;
  }

  IPlayerInfo* getPlayerByID(char id) const {
    map<char, BasePlayerInfo*>::const_iterator it = _playersInfoIDMap.find(id);

    if (it != _playersInfoIDMap.end())
      return it->second;
    
    return NULL;
  }

  #pragma endregion

  #pragma region ModelPreservedInterfaces 
 
  IPlayer* registerPlayer(IPlayer* newPlayer) {
    char id = _newPlayerID;
    int idNo = id - _startID;

    if (idNo < _headquarters.size())
    {
      _newPlayerID++;

      BasePlayerInfo* newBaseInfo 
        = new BasePlayerInfo(id, _headquarters[idNo]);

      // save for later uses
      _playersInfo.push_back(newBaseInfo);
      _iPlayersInfo.push_back(newBaseInfo);
      _playersInfoIDMap.insert(pair<char, BasePlayerInfo*>(id, newBaseInfo));

      // add tanks
      int w = _map->getWidth(), h = _map->getHeight();
      for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
          // there is a tank here for id
          if ((*_map)(i,j) == id) {
            newBaseInfo->addTank(_defaultHP, _defaultAmmo, 
                        _defaultRange, pair<int,int>(i,j));
          }
        }
      }

      // bind with players
      newPlayer->onBindPlayerInfo(newBaseInfo);
      newPlayer->onBindGameInfo(this);

      // return the player
      return newPlayer;
    }
    // fail to add, return NULL
    return NULL;
  }

  bool isValidMove(IPlayer* player, const Command& move) {
    IPlayerInfo* playerInfo = player->getPlayerInfo();
    ITank* tank = move.getReceivingObject();

    switch (move.getActionType()) {
      case Command::SURRENDER:
      case Command::SKIP:
        return true;

      case Command::FIRE: {
        if (tank == NULL || tank->getOwner() != playerInfo || !tank->isAlive())
          return false;
        pair<int, int> s = tank->getPosition(),
            e = move.getTargetPosition();
        int r = tank->getRange();

        return ((s.first == e.first) && (abs(s.second-e.second) <= r))
          || ((s.second == e.second) && (abs(s.first-e.first) <= r));
      }
         
      case Command::MOVE: {
        pair<int, int> e = move.getTargetPosition();

        if (tank == NULL || tank->getOwner() != playerInfo || !tank->isAlive()
            || !_map->isEmptySpace(e.first, e.second))
          return false;

        pair<int, int> s = tank->getPosition();

        return abs(s.first - e.first) + abs(s.second-e.second) <= 1;
      }
    }

    return false;
  }

  vector<pair<int, int> > applyMove(IPlayer* player, const Command& move) {

    vector<pair<int, int> > changes;

    BasePlayerInfo* playerInfo = (BasePlayerInfo*)(player->getPlayerInfo());
    BaseTank* tank = (BaseTank*)(move.getReceivingObject());

    switch (move.getActionType()) {
      case Command::SURRENDER: {
        list<ITank*> aliveTanks = playerInfo->getAliveTanks();

        changes.resize(aliveTanks.size());

        for (list<ITank*>::iterator it = aliveTanks.begin(); 
              it != aliveTanks.end(); ++it) {

          _map->remove(tank);

          playerInfo->removeTank((BaseTank*)*it);
          changes.push_back((*it)->getPosition());
        }
      }
        break;

      case Command::FIRE: {

      }
         
      case Command::MOVE: {
        pair<int,int> s = tank->getPosition(),
                      e = move.getTargetPosition();

        _map->move(tank, e);

        changes.push_back(s);
        changes.push_back(e);
      }

      case Command::SKIP:
        break;
    }

    return changes;
  }

  BaseGameModel(const vector<string>& charMap, 
                const vector<pair<int,int> >& heads,
                int defHP, int defAmmo, int defRange, char startId = STARTID) 
    : _map(new BaseMap(charMap)), 
      _headquarters(heads),
      _defaultHP(defHP), _defaultAmmo(defAmmo), _defaultRange(defRange),
      _startID(startId), _newPlayerID(startId) {

    // set up the bridges and blocks
    int w = _map->getWidth(), h = _map->getHeight();
    char blockID = BaseBlock::getID();
    char bridgeID = BaseBridge::getID();

    for (int j = 0; j < h; j++) {
      for (int i = 0; i < w; i++) {
        // there is a tank here for id
        if ((*_map)(i,j) == blockID) {
          BaseBlock* newBlock = new BaseBlock(pair<int,int>(i,j));
          _blocks.push_back(newBlock);
          _onMapBlocks.push_back(newBlock);
        } else if ((*_map)(i,j) == bridgeID) {
          BaseBridge* newBridge = new BaseBridge(pair<int,int>(i,j));
          _bridges.push_back(newBridge);
          _onMapBridges.push_back(newBridge);
        }
      }
    }
  }

  ~BaseGameModel() {
    delete _map;
    int i;
    for (i = 0; i < _playersInfo.size(); i++) {
      delete _playersInfo[i];
    }

    for (i = 0; i < _bridges.size(); i++) {
      delete _bridges[i];
    }

    for (i = 0; i < _blocks.size(); i++) {
      delete _blocks[i];
    }
  }
  #pragma endregion;
};