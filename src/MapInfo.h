#ifndef __TANKGAME_MAPINFO__
#define __TANKGAME_MAPINFO__

#include <string>
#include <vector>

struct MapInfo {
  std::vector<std::string> charMap;
  int tankHP, tankAmmo, tankRange;
  int bridgeHP, blockHP;
  std::string bridgeIDs;
  std::string blockIDs;
  std::string headquarterIDs;
  std::string playerIDs;
  char landID, waterID;
};

#endif