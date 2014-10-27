#ifndef __TANKGAME_BASEPLAYERINFO__
#define __TANKGAME_BASEPLAYERINFO__

#include "IPlayerInfo.h"
#include "BaseTank.h"
#include <vector>



class BasePlayerInfo : public IPlayerInfo {
public:
  #pragma region IPlayerInfoImplementation
  char getPlayerMapID() const;

  std::list<ITank*> getAliveTanks() const;
  std::list<ITank*> getDeadTanks() const;
  std::pair<int, int> getHeadquarterPosition() const;

  // Command getLastMove() const {
  //   return _lastMove;
  // }

  bool isPlayable() const;

  #pragma endregion

  #pragma region ModelPreservedInterface
  bool isOwnerOf(const BaseTank* tank) const;

  bool removeTank(BaseTank* tank);

  bool getHit(BaseTank* tank);
  bool getHit(const std::pair<int,int>& pos, BaseTank*& tank);

  // void updateLastMove(const Command& cmd) {
  //   _lastMove = cmd;
  // }

  void addTank(int hp, int ammo, int range, std::pair<int,int> pos);

  BasePlayerInfo(char id, const std::pair<int,int>& head);
  ~BasePlayerInfo();
  #pragma endregion

private:
  char _mapID;
  Command _lastMove;
  std::list<ITank*> _aliveTanks;
  std::list<ITank*> _deadTanks;
  std::pair<int, int> _headquarterPosition;
  std::vector<BaseTank*> _baseTanks;
};

#endif