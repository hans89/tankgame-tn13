
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
    iss >> info.defaultTank.HP >> info.defaultTank.Ammo >> info.defaultTank.Range;

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

    // line 9: number of custom tanks
    getline(inStream, line);
    iss.str(line);
    int noTank = 0;
    iss >> noTank;

    TankInfo tankInf;
    pair<int,int> pos;

    for (int k = 0; k < noTank; k++) {
        getline(inStream, line);
        iss.str(line);
        iss >> pos.first >> pos.second
            >> tankInf.HP >> tankInf.Ammo >> tankInf.Range;
        info.tankMap.insert(pair<pair<int,int>, TankInfo>(pos, tankInf));
    }

    // get the map from the rest
    while (getline(inStream, line).good()) {
      if (!line.empty())
        info.charMap.push_back(line);
    }

    inStream.close();
}