#ifndef __TANKGAME_MAPLOADER__
#define __TANKGAME_MAPLOADER__ 

#include <string>
#include <vector>

#include "BaseMap.h"

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

class MapLoader {
public:
  static void loadMap(const std::string& mapFilePath, MapInfo& info);
};

#endif