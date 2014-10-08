#include "IGameInfo.h"

#include <list>
#include <vector>
#include <map>

#define STARTID 'A'

using namespace std;
class BaseGameModel : IGameInfo {
private:
  BaseMap* _map;
  vector<pair<int,int> > _headquarters;
  int _defaultHP, _defaultAmmo, _defaultRange;
  char _newPlayerID;
  const char _startID;
  
  vector<BasePlayerInfo*> _playersInfo;
  vector<IPlayerInfo*> _iPlayersInfo;
  map<char, BasePlayerInfo*> _playersInfoIDMap;

public:

  #pragma region IGameInfoImplementation
  // get the map
  IMap* getMap() const {
    return _map;
  }

  // get list of current players information
  vector<IPlayerInfo*> getPlayersInfo() const {
    return _iPlayersInfo;
  }
  #pragma endregion

  #pragma region ControllerImplementation 
 
  IPlayer* registerPlayer(IPlayer* newPlayer) {
    char id = _newPlayerID;
    int idNo = teamID - _startID;

    if (idNo < headquarters.size())
    {
      _newPlayerID++;

      BasePlayerInfo* newBaseInfo 
        = new BasePlayerInfo(id, headquarters[idNo]);

      // save for later uses
      _playersInfo.push_back(newBaseInfo);
      _iPlayersInfo.push_back(newBaseInfo);
      _playersInfoIDMap.insert(pair<char, BasePlayerInfo*>(id, newBaseInfo));

      // add tanks
      int w = _map->getWidth(), h = _map->getHeight();
      for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
          // there is a tank here for id
          if ((*map)(i,j) == id) {
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
    ITank* tank = move.receivingObject;

    switch (move.actionType) {
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
            || !_map->isWalkable(e.first, e.second))
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
    BaseTank* tank = (BaseTank*)(move.receivingObject);

    switch (move.actionType) {
      case Command::SURRENDER: {
        list<ITank*> aliveTanks = playerInfo->getAliveTanks();

        changes.resize(aliveTanks.size());

        for (list<ITank*>::iterator it = aliveTanks.begin(); 
              it != aliveTanks.end(); ++it) {

          _map->removeTank(tank);

          playerInfo->removeTank(*it);
          changes.push_back((it*)->getPosition());
        }
      }
        break;

      case Command::FIRE: {

      }
         
      case Command::MOVE: {
        pair<int,int> s = tank->getPosition(),
                      e = move.getTargetPosition();

        _map->moveTank(tank, e);

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
      _startID(startId), _newPlayerID(startId) {}

  ~BaseGameModel() {
    delete _map;
    for (int i = 0; i < _playersInfo.size(); i++) {
      delete _playersInfo[i];
    }
  }
  #pragma endregion;
};