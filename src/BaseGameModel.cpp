#include "IGameInfo.h"

#include <list>
#include <vector>

#define STARTID 'A'

using namespace std;
class BaseGameModel : IGameInfo {
private:
  BaseMap* _map;
  vector<pair<int,int> > _headquarters;
  int _defaultHP, _defaultAmmo;
  char _newPlayerID;
  const char _startID;
  
  vector<BasePlayerInfo*> _playersInfo;
  vector<IPlayerInfo*> _iPlayersInfo;

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
    char teamID = _newPlayerID;
    int teamNo = teamID - _startID;

    if (teamNo < headquarters.size())
    {
      _newPlayerID++;

      BasePlayerInfo* newBaseInfo 
        = new BasePlayerInfo(teamID, headquarters[teamNo]);

      // save for later uses
      _playersInfo.push_back(newBaseInfo);
      _iPlayersInfo.push_back(newBaseInfo);

      // add tanks
      int w = _map->getWidth(), h = _map->getHeight();
      for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
          // there is a tank here for teamID
          if ((*map)(i,j) == teamID) {
            newBaseInfo->addTank(_defaultHP, _defaultAmmo, pair<int,int>(i,j));
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

  BaseGameModel(const vector<string>& charMap, 
                const vector<pair<int,int> >& heads,
                int defHP, int defAmmo, char startId = STARTID) 
    : _map(new BaseMap(charMap)), 
      _headquarters(heads),
      _defaultHP(defHP), _defaultAmmo(defAmmo),
      _startID(startId), _newPlayerID(startId) {}

  ~BaseGameModel() {
    delete _map;
    for (int i = 0; i < _playersInfo.size(); i++) {
      delete _playersInfo[i];
    }
  }
  #pragma endregion;
};