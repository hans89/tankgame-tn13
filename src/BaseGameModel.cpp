#include "BaseGameModel.h"

#pragma region IGameInfoImplementation
// get the map
IMap* BaseGameModel::getMap() const {
  return _map;
}

// get the available bridges on map
list<IBridge*> BaseGameModel::getOnMapBridges() const {
  return _onMapBridges;
}

// get the available blocks on map
list<IBlock*> BaseGameModel::getOnMapBlocks() const {
  return _onMapBlocks;
}

// get list of current players information
vector<IPlayerInfo*> BaseGameModel::getPlayersInfo() const {
  return _iPlayersInfo;
}

IPlayerInfo* BaseGameModel::getPlayerByID(char id) const {
  map<char, BasePlayerInfo*>::const_iterator it = _playersInfoIDMap.find(id);

  if (it != _playersInfoIDMap.end())
    return it->second;
  
  return NULL;
}

#pragma endregion

#pragma region ModelPreservedInterfaces 

bool BaseGameModel::isEndGame() {
  //TODO
  return false;
}

const MapInfo& BaseGameModel::getMapInfo() {
  return _mapInfo;
}

IPlayer* BaseGameModel::registerPlayer(IPlayer* newPlayer) {
  if (_nextRegisterPlayer < _mapInfo.playerIDs.size()) {
    
    char id = _mapInfo.playerIDs[_nextRegisterPlayer++];

    BasePlayerInfo* newBaseInfo 
      = new BasePlayerInfo(id, _headquarters[id]);

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
          newBaseInfo->addTank(_mapInfo.tankHP, _mapInfo.tankAmmo, 
                      _mapInfo.tankRange, pair<int,int>(i,j));
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

bool BaseGameModel::isValidMove(IPlayer* player, const Command& move) {
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

vector<pair<int, int> > BaseGameModel::applyMove(IPlayer* player, 
      const Command& move) {

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

BaseGameModel::BaseGameModel(const MapInfo& info) 
  : _mapInfo(info),
    _nextRegisterPlayer(0) {

  _map = new BaseMap(_mapInfo);
  
  // set up the bridges, blocks and headquarters
  int w = _map->getWidth(), h = _map->getHeight();

  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      char c = (*_map)(i,j);
      
      if (_mapInfo.blockIDs.find(c) != string::npos) {
      // there is a block here

        BaseBlock* newBlock 
            = new BaseBlock(c, _mapInfo.blockHP, pair<int,int>(i,j));
        _blocks.push_back(newBlock);
        _onMapBlocks.push_back(newBlock);
      } else if (_mapInfo.bridgeIDs.find(c) != string::npos) {
      // there is a bridge here

        BaseBridge* newBridge 
            = new BaseBridge(c, _mapInfo.bridgeHP, pair<int,int>(i,j));
        _bridges.push_back(newBridge);
        _onMapBridges.push_back(newBridge);
      } else if (_mapInfo.headquarterIDs.find(c) != string::npos
          && _headquarters.find(c) == _headquarters.end()) {
      // there is a headquarter here

        _headquarters.insert(pair<char, pair<int,int> >(c, pair<int,int>(i,j)));
      }
    }
  }
}

BaseGameModel::~BaseGameModel() {
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