#ifndef __TANKGAME_MAPLOADER__
#define __TANKGAME_MAPLOADER__ 

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

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
  static void loadMap(const char* mapFilePath, MapInfo& info) {
    std::ifstream inStream(mapFilePath);

    std::istringstream iss;
    std::string line;

    if (!inStream.is_open()) {
      std::cerr << "Can't load map " << mapFilePath << std::endl;
      return;
    }
      
    // line 1: TankHP TankAmmo TankRange
    std::getline(inStream, line);
    iss.str(line);
    iss >> info.tankHP >> info.tankAmmo >> info.tankRange;

    // line 2: BridgeHP BlockHP
    std::getline(inStream, line);
    iss.str(line);
    iss >> info.bridgeHP >> info.blockHP;

    // line 3: BridgeIDs
    std::getline(inStream, line);
    iss.str(line);
    iss >> info.bridgeIDs;

    // line 4: BlockIDs
    std::getline(inStream, line);
    iss.str(line);
    iss >> info.blockIDs;

    // line 5: HeadquarterIDs
    std::getline(inStream, line);
    iss.str(line);
    iss >> info.headquarterIDs;

    // line 6: PlayerIDs
    std::getline(inStream, line);
    iss.str(line);
    iss >> info.playerIDs;

    // line 7: LandID WaterID
    std::getline(inStream, line);
    info.landID = line[0];
    info.waterID = line[1];

    // get the map from the rest
    while (std::getline(inStream, line).good()) {
      if (!line.empty())
        info.charMap.push_back(line);
    }

    inStream.close();
  }
};

#endif