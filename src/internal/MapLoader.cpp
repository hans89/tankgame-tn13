
#include <fstream>
#include <sstream>

#include "MapLoader.h"
#include <iostream>
using namespace std;

void MapLoader::loadMap(const string& mapFilePath, MapInfo& info) {
    ifstream inStream(mapFilePath.c_str());

    istringstream iss;
    string line;

    if (!inStream.is_open()) {
      cout << "Can't load map " << mapFilePath << endl;
      return;
    }
      
    // line 1: TankHP TankAmmo TankRange
    getline(inStream, line);
    iss.str(line);
    iss >> info.tankHP >> info.tankAmmo >> info.tankRange;

    // line 2: BridgeHP BlockHP
    getline(inStream, line);
    iss.str(line);
    iss >> info.bridgeHP >> info.blockHP;

    // line 3: BridgeIDs
    getline(inStream, line);
    iss.str(line);
    iss >> info.bridgeIDs;

    // line 4: BlockIDs
    getline(inStream, line);
    iss.str(line);
    iss >> info.blockIDs;

    // line 5: HeadquarterIDs
    getline(inStream, line);
    iss.str(line);
    iss >> info.headquarterIDs;

    // line 6: PlayerIDs
    getline(inStream, line);
    iss.str(line);
    iss >> info.playerIDs;

    // line 7: LandID WaterID
    getline(inStream, line);
    info.landID = line[0];
    info.waterID = line[1];

    // line 8: Maximum no of steps
    getline(inStream, line);
    iss.str(line);
    iss >> info.maxStep;

    // get the map from the rest
    while (getline(inStream, line).good()) {
      if (!line.empty())
        info.charMap.push_back(line);
    }

    inStream.close();
}