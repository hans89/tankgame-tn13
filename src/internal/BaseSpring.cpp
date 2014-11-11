#include "BaseSpring.h"
#include "BaseTank.h"

char BaseSpring::getMapID() const {
  return _mapID;
}

int BaseSpring::getHPIncrease() const {
  return _HPIncrease;
}

void BaseSpring::regen(IGameInfo* gameInfo) const {

  if (this->isOnMap()) {
    const char dx[] = {0, 0, -1, 1};
    const char dy[] = {-1, 1, 0, 0};

    std::pair<int,int> curPos = this->getPosition();
    IMap* bMap = gameInfo->getMap();

    for (int i = 0; i < sizeof(dx); i++) {
      int nX = curPos.first + dx[i];
      int nY = curPos.second + dy[i];

      BaseTank* tank = NULL;
      if ((tank = (BaseTank*) gameInfo->getTank(nX, nY)) != NULL) {
        tank->increaseHP(this->getHPIncrease());
      }
    }
  }
}