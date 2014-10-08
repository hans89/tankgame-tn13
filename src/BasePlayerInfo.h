#ifndef __TANKGAME_BASEPLAYERINFO__
#define __TANKGAME_BASEPLAYERINFO__ value

#include "IPlayerInfo.h"
#include <algorithm>

using namespace std;
class BasePlayerInfo : public IPlayerInfo {
private:
  char _mapID;
  Command _lastMove;
  list<ITank*> _aliveTanks;
  list<ITank*> _deadTanks;
  pair<int, int> _headquarterPosition;
  vector<BaseTank*> _baseTanks;

public:
  #pragma region IPlayerInfoImplementation
  char getPlayerMapID() const {
    return _mapID;
  }

  list<ITank*> getAliveTanks() const {
    return _aliveTanks;
  }

  list<ITank*> getDeadTanks() const {
    return _deadTanks;
  }

  pair<int, int> getHeadquarterPosition() const {
    return _headquarterPosition;
  }

  Command getLastMove() const {
    return _lastMove;
  }

  bool isPlayable() const {
    return !_aliveTanks.empty();
  }

  #pragma endregion

  #pragma region ControllerImplementation
  bool isOwnerOf(const BaseTank* tank) const {
    return this == tank->getOwner();
  }

  bool removeTank(BaseTank* tank) {
    list<ITank*>::iterator it 
      = find (_aliveTanks.begin(), _aliveTanks.end(), tank);
    if (it != _aliveTanks.end()) {
      tank->kill();
      _deadTanks.push_back(tank);
      _aliveTanks.erase(it);
      return true;
    }
    return false;
  }

  bool getHit(BaseTank* tank) {
    list<ITank*>::iterator it 
      = find (_aliveTanks.begin(), _aliveTanks.end(), tank);

    if (it != _aliveTanks.end()) {
      tank->decreaseHP();
      if (!tank->isAlive()) {
        _deadTanks.push_back(tank);
        _aliveTanks.erase(it);
      }
      return true;
    }
    return false;
  }

  void updateLastMove(const Command& cmd) {
    _lastMove = cmd;
  }

  void addTank(int hp, int ammo, int range, pair<int,int> pos) {
    BaseTank* newTank = new BaseTank(hp, ammo, range, pos, this);

    _baseTanks.push_back(newTank);
    _aliveTanks.push_back(newTank);
  }

  BasePlayerInfo(char id, pair<int,int> head) :
    _mapID(id), _headquarterPosition(head) {}

  ~BasePlayerInfo() {
    for (int i = 0; i < _baseTanks.size(); i++)
      delete _baseTanks[i];
  }

  #pragma endregion
};

#endif