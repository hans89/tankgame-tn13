#ifndef __TANKGAME_MAPINFO__
#define __TANKGAME_MAPINFO__

#include <string>
#include <vector>
#include <map>

struct TankInfo {
  int HP, Ammo, Range;

  TankInfo(int hp = 0, int a = 0, int r = 0) :
    HP(hp), Ammo(a), Range(r) {}
};

struct MapInfo {
  std::vector<std::string> charMap;

  TankInfo defaultTank;
  std::map<std::pair<int,int>, TankInfo> tankMap;

  int bridgeHP, blockHP, headHP;

  std::string bridgeIDs;
  std::string blockIDs;
  std::string headquarterIDs;
  std::string playerIDs;

  char landID, waterID, springID;
  int maxStep;
};

#endif