#ifndef __TANKGAME_MAPLOADER__
#define __TANKGAME_MAPLOADER__ 

#include <string>
#include <vector>

#include "MapInfo.h"
#include "BaseMap.h"


class MapLoader {
public:
  static void loadMap(const std::string& mapFilePath, MapInfo& info);
};

#endif