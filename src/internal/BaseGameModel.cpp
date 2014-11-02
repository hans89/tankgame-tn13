#include "BaseGameModel.h"

#include "../include/utils.h"
#include <queue>

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

PlayerEndGameInfo BaseGameModel::getPlayerEndGameInfo(IPlayerInfo* player) const {
  
  // calculate distance map, from headquarter to all the map positions
  const int dx[] = { -1, 1, 0, 0 };
  const int dy[] = { 0, 0, -1, 1 };

  vector<vector<int> > dis;

  dis.resize(_map->getWidth());
  for (int i = 0; i < dis.size(); ++i)
    dis[i].resize(_map->getHeight(), -1);

  queue<pair<int, int> > q;
  pair<int, int> start = player->getHeadquarterPosition();

  dis[start.first][start.second] = 0;
  q.push(start);

  while (!q.empty())
  {
    pair<int, int> u = q.front();
    q.pop();

    for (int i = 0; i < 4; ++i)
    {
      pair<int, int> v(u.first + dx[i], u.second + dy[i]);
      
      if (!_map->isOnMap(v))
        continue;
      
      //if this position is not available, ignore this
      if (dis[v.first][v.second] != -1)
        continue;
  
      char id = (*_map)(v.first, v.second);

      if (!_map->isEmptySpace(id) 
        && !_map->isEmptyBridge(id)
        && this->getPlayerByID(id) != player)
        //if this object is an obstacle or enemy's tank, ignore this
          continue;

      dis[v.first][v.second] = dis[u.first][u.second] + 1;
      q.push(v);
    }
  }

  // calculate end-game info
  PlayerEndGameInfo egInfo;

  list<ITank*> alive = player->getAliveTanks();

  for (list<ITank*>::iterator it = alive.begin(); it != alive.end(); ++it)
  {
    pair<int, int> pos = (*it)->getPosition();
    if (dis[pos.first][pos.second] == -1)
      continue;

    egInfo.totalDistance += dis[pos.first][pos.second];
    egInfo.totalHP += (*it)->getHP();
    ++egInfo.totalTanks;
  }
  return egInfo;
}


int BaseGameModel::getGameFinalResult(const vector<PlayerEndGameInfo>& egInfos) const {
  int max = 0;
  for (int i = 0; i < egInfos.size(); i++) {
    if (egInfos[i].totalHP > egInfos[max].totalHP)
      max = i;

    if (egInfos[i].totalHP == egInfos[max].totalHP 
        && egInfos[i].totalDistance < egInfos[max].totalDistance)
      max = i;
  }

  if (max == 0 && egInfos.size() > 1
      && egInfos[0].totalHP == egInfos[1].totalHP
      && egInfos[0].totalDistance == egInfos[1].totalDistance)
    return -1;

  return max;
}

#pragma endregion

#pragma region ModelPreservedInterfaces 

bool BaseGameModel::isEndGame() const {
  //TODO
  return _currentTurn >= _mapInfo.maxStep
    //&& _totalAmmo <= 0
  ;
}

const BaseMap* BaseGameModel::getBaseMap() const {
  return _map;
}

int BaseGameModel::getMaximumNumberOfTurn() const {
  return _mapInfo.maxStep;
}


IPlayer* BaseGameModel::registerPlayer(IPlayer* newPlayer) {
  if (_nextRegisterPlayer < _mapInfo.playerIDs.size()) {
    
    char id = _mapInfo.playerIDs[_nextRegisterPlayer];

    BasePlayerInfo* newBaseInfo 
		= new BasePlayerInfo(id, _headquarters[_mapInfo.headquarterIDs[_nextRegisterPlayer++]]);

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
          _totalAmmo += _mapInfo.tankAmmo;
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

bool BaseGameModel::isValidMove(IPlayerInfo* playerInfo, const Command& move) const {
  ITank* tank = move.getReceivingObject();

  switch (move.getActionType()) {
    case Command::SURRENDER:
    case Command::SKIP:
      return true;

    case Command::FIRE: {
      if (tank == NULL || tank->getOwner() != playerInfo || !tank->isAlive())
        return false;
      pair<int, int> e = move.getTargetPosition();

      if (!_map->isOnMap(e))
        return false;

      pair<int, int> s = tank->getPosition();
      int r = tank->getRange();
      int distance = Utils::manhattanDistance(s, e);
      return  distance <= r && distance > 0 && tank->hasAmmo();
    }
       
    case Command::MOVE: {
      pair<int, int> e = move.getTargetPosition();

      if (!_map->isOnMap(e))
        return false;
      
      if (tank == NULL || tank->getOwner() != playerInfo || !tank->isAlive()
          || !(_map->isEmptySpace(e.first, e.second) || _map->isBridge(e.first, e.second)))
        return false;

      pair<int, int> s = tank->getPosition();

      return Utils::manhattanDistance(s, e) == 1;
    }
  }

  return false;
}

vector<pair<int, int> > 
BaseGameModel::applyMove(IPlayerInfo* player, const Command& move) {

  vector<pair<int, int> > changes;

  BasePlayerInfo* playerInfo = (BasePlayerInfo*)player;
  BaseTank* tank = (BaseTank*)(move.getReceivingObject());

  switch (move.getActionType()) {
    case Command::SURRENDER: {
      list<ITank*> aliveTanks = playerInfo->getAliveTanks();

      for (list<ITank*>::iterator it = aliveTanks.begin(); 
            it != aliveTanks.end(); ++it) {

        _map->remove(tank);

        playerInfo->removeTank((BaseTank*)*it);
        changes.push_back((*it)->getPosition());
      }
    }
      break;

    case Command::FIRE: {
      tank->decreaseAmmo();
      _totalAmmo--;

      pair<int,int> pos = move.getTargetPosition();
      int i = pos.first,
          j = pos.second;

      if (_map->isEmptySpace(i, j) || _map->isHeadquarter(i,j)
            || _map->isWater(i, j)) {
        // skipp
      } 

      else if (_map->isBlock(i, j)) {
        // find the block
        list<IBlock*>::iterator it;

        for (it = _onMapBlocks.begin(); it != _onMapBlocks.end(); ++it) {
          if ((*it)->getPosition() == pos)
            break;
        }

        if (it != _onMapBlocks.end()) {
          BaseBlock* baseBlock = (BaseBlock*)(*it);
          baseBlock->decreaseHP();
          // destroy block 
          if (baseBlock->getHP() == 0) {
            _map->remove(baseBlock);
            changes.push_back(pos); 
            _onMapBlocks.erase(it);
          } 
        }
      } 

      else if (_map->isBridge(i, j)) {
        // find the block
        list<IBridge*>::iterator it;

        for (it = _onMapBridges.begin(); it != _onMapBridges.end(); ++it) {
          if ((*it)->getPosition() == pos)
            break;
        }

        if (it != _onMapBridges.end()) {
          BaseBridge* baseBridge = (BaseBridge*)(*it);
          baseBridge->decreaseHP();
          // destroy bridge 
          if (baseBridge->getHP() == 0) {
            _map->remove(baseBridge);
            changes.push_back(pos); 
            _onMapBridges.erase(it);
          } 
        }
      } 

      else if (_map->isTank(i, j)) {
        BasePlayerInfo* playerInfo = _playersInfoIDMap[(*_map)(i,j)];

        BaseTank* baseTank = NULL;

        if (playerInfo->getHit(pos, baseTank) && baseTank != NULL) {
          _map->remove(baseTank);
          changes.push_back(pos); 
        }
      }
      break;
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

bool BaseGameModel::isPossibleMove(IPlayerInfo* playerInfo, const Command& move) const {
  
  ITank* tank = move.getReceivingObject();

  switch (move.getActionType()) {
    case Command::SURRENDER:
    case Command::SKIP:
      return true;

    case Command::FIRE: {
      if (tank == NULL || tank->getOwner() != playerInfo || !tank->isAlive())
        return false;
      pair<int, int> e = move.getTargetPosition();

      if (!_map->isOnMap(e))
        return false;

      pair<int, int> s = tank->getPosition();

      int distance = Utils::manhattanDistance(s, e);

      int r = tank->getRange();

      return distance <= r && distance > 0 && tank->hasAmmo();
    }
       
    case Command::MOVE: {
      pair<int, int> e = move.getTargetPosition();

      if (!_map->isOnMap(e))
        return false;
      
      if (tank == NULL || tank->getOwner() != playerInfo || !tank->isAlive())
        return false;

      if (_map->isBlock(e.first, e.second) || _map->isWater(e.first, e.second)
          || _map->isTank(e.first, e.second, playerInfo->getPlayerMapID())) 
        return false;

      pair<int, int> s = tank->getPosition();

      return Utils::manhattanDistance(s, e) == 1;
    }
  }

  return false;
}

pair<CommandInfo*, CommandInfo*> 
BaseGameModel::tryMove(CommandInfo& move1, CommandInfo& move2, bool& dependent) {

  bool possible1 = isPossibleMove(move1.commander, move1.originalCommand);
  bool possible2 = isPossibleMove(move2.commander, move2.originalCommand);
  
  dependent = false;
  // executedCommand's of CommandInfo's are 
  // approved by default

  move1.executedCommand = move1.originalCommand;
  move2.executedCommand = move2.originalCommand;


  if (!possible1 && possible2) {
      // player1 can't move, player2 move
    if (isValidMove(move2.commander, move2.executedCommand)) {
      // approve move2
      // skip move1
      move1.executedCommand = Command();
      return pair<CommandInfo*, CommandInfo*>(&move1, &move2);
    }
  }

  if (possible1 && !possible2) {
    // player2 can't move, player1 move
    if (isValidMove(move1.commander, move1.executedCommand)) {
      // approve move1
      // skip move2
      move2.executedCommand = Command();
      return pair<CommandInfo*, CommandInfo*>(&move1, &move2);
    }
  }

  if (possible1 && possible2) {
    Command::Action t1 = move1.originalCommand.getActionType(),
                  t2 = move2.originalCommand.getActionType();

    
    pair<int,int> e1 = move1.originalCommand.getTargetPosition();
    pair<int,int> e2 = move2.originalCommand.getTargetPosition();

    pair<int,int> s1;
    pair<int,int> s2;

    if (t1 == Command::MOVE || t1 == Command::FIRE)
        s1 = move1.originalCommand.getReceivingObject()->getPosition();

    if (t2 == Command::MOVE || t2 == Command::FIRE)
        s2 = move2.originalCommand.getReceivingObject()->getPosition();

    if (t1 == Command::MOVE && t2 == Command::MOVE) {
      // can't share the same place
      // can't switch place
      if ((e1 == e2) || (s1 == e2 && s2 == e1)) {
        move1.executedCommand = Command();
        move2.executedCommand = Command();
        return pair<CommandInfo*, CommandInfo*>(&move1, &move2);
      }

      // 1 move into old place of 2 => 2 go first
      // approve both
      if (e1 == s2) {
        if (!isValidMove(move2.commander, move2.executedCommand)) {
          move1.executedCommand = Command();
          move2.executedCommand = Command();
        }
        return pair<CommandInfo*, CommandInfo*>(&move2, &move1);
      }

      // 2 move into old place of 1 => 1 go first
      if (e2 == s1) {
        if (!isValidMove(move1.commander, move1.executedCommand)) {
          move1.executedCommand = Command();
          move2.executedCommand = Command();
        }
        return pair<CommandInfo*, CommandInfo*>(&move1, &move2);
      }
    }

    if (t1 == Command::FIRE && t2 == Command::MOVE) {
      // 2 move into 1 fire => 2 move first
      // 2 move out of 1 fire => 2 move first

      if (!isValidMove(move2.commander, move2.executedCommand))
        move2.executedCommand = Command();

      if ((e2 == e1) || (s2 == e1))
        return pair<CommandInfo*, CommandInfo*>(&move2, &move1);
    }

    if (t2 == Command::FIRE && t1 == Command::MOVE) {
      // 1 move into 2 fire => 1 move first
      // 1 move out of 2 fire => 1 move first

      if (!isValidMove(move1.commander, move1.executedCommand))
        move1.executedCommand = Command();

      if ((e1 == e2) || (s1 == e2))
        return pair<CommandInfo*, CommandInfo*>(&move1, &move2);
    }

    if (t1 == Command::FIRE && t2 == Command::FIRE && e1 == s2 && e2 == s1) {
      dependent = true;
    }
    // order is unimportant/unhandled
    if (!isValidMove(move1.commander, move1.executedCommand))
      move1.executedCommand = Command();
    if (!isValidMove(move2.commander, move2.executedCommand))
      move2.executedCommand = Command();
    return pair<CommandInfo*, CommandInfo*>(&move1, &move2);
  }

  
  // both impossible, skip all
  move1.executedCommand = Command();
  move2.executedCommand = Command();
  return pair<CommandInfo*, CommandInfo*>(&move1, &move2);

}

vector<pair<int, int> > 
BaseGameModel::applyMove(const CommandInfo& move1, const CommandInfo& move2) {

  vector<pair<int, int> > changes;
  Command::Action t1 = move1.originalCommand.getActionType(),
                  t2 = move2.originalCommand.getActionType();

  if (t1 == Command::FIRE && t2 == Command::FIRE) {

    pair<int,int> s1 = move1.originalCommand.getReceivingObject()->getPosition();
    pair<int,int> e1 = move1.originalCommand.getTargetPosition();
    pair<int,int> s2 = move2.originalCommand.getReceivingObject()->getPosition();
    pair<int,int> e2 = move2.originalCommand.getTargetPosition();

    if (e1 == s2 && e2 == s1) {
      BasePlayerInfo* playerInfo1 = (BasePlayerInfo*)move1.commander;
      BaseTank* tank1 = (BaseTank*)(move1.executedCommand.getReceivingObject());

      BasePlayerInfo* playerInfo2 = (BasePlayerInfo*)move2.commander;
      BaseTank* tank2 = (BaseTank*)(move2.executedCommand.getReceivingObject());

      tank1->decreaseAmmo();
      tank2->decreaseAmmo();
      _totalAmmo -= 2;

      pair<int,int> pos = tank1->getPosition();
      if (playerInfo1->getHit(tank1)) {
        _map->remove(tank1);
        changes.push_back(pos); 
      }

      pos = tank2->getPosition();
      if (playerInfo2->getHit(tank2)) {
        _map->remove(tank2);
        changes.push_back(pos);
      }
    }
  }

  return changes;
}


BaseGameModel::BaseGameModel(const MapInfo& info) 
  : _mapInfo(info),
    _nextRegisterPlayer(0),
    _currentTurn(0),
    _totalAmmo(0) {

  _map = new BaseMap(_mapInfo);
  
  // set up the bridges, blocks and headquarters
  int w = _map->getWidth(), h = _map->getHeight();

  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      char c = (*_map)(i,j);
      
      if (_map->isBlock(i, j)) {
        BaseBlock* newBlock 
            = new BaseBlock(c, _mapInfo.blockHP, pair<int,int>(i,j));
        _blocks.push_back(newBlock);
        _onMapBlocks.push_back(newBlock);
      } 

      else if (_map->isBridge(i, j)) {
        BaseBridge* newBridge 
            = new BaseBridge(c, _mapInfo.bridgeHP, pair<int,int>(i,j));
        _bridges.push_back(newBridge);
        _onMapBridges.push_back(newBridge);
      }

      else if (_map->isHeadquarter(i, j)) {
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

void BaseGameModel::nextTurnCount() {
  _currentTurn++;
}

int BaseGameModel::getCurrentTurnCount() const {
  return _currentTurn;
}
#pragma endregion;