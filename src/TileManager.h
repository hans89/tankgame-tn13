#ifndef __TILE_MANAGER__
#define __TILE_MANAGER__

#include <string>
#include <map>
#include <iostream>
#include <sstream>

#include "include/CImg.h"
#include "include/utils.h"
#include "AppConfig.h"


using namespace std;
using namespace cimg_library;

class TileManager {
public:
	
	TileManager(string tileRes) : 
			tileResourceFile(tileRes),
			tileImage(tileRes.c_str()) {
			
			string tileSizeStr = AppConfig::getConfig("tile_size");
			istringstream iss(tileSizeStr);
			iss >> tileSize;

			string tileResource = AppConfig::getConfig("tile_list");

			vector<string> tiles = Utils::split(tileResource, ',');

			for (int i = 0; i < tiles.size(); i++) {
				pair<int,int> offset = Utils::parseIntPair(AppConfig::getConfig(tiles[i]));
				addTileMap(tiles[i], offset.first, offset.second);				
			}
	}

	CImg<unsigned char>& getTile(string t) {
		return tilesMap[t];
	}

	CImg<unsigned char>& getCharTile(char t) {
		return tilesMap[CHAR_TILE_PREFIX + t];
	}

	string getCharTileName(char t) {
		return CHAR_TILE_PREFIX + t;
	}
	static int getTileSize() {
		return tileSize;
	}

private:
	static int tileSize;
	string tileResourceFile;
	static string const CHAR_TILE_PREFIX;
	CImg<unsigned char>	tileImage;
	map<string, CImg<unsigned char> > tilesMap;
	

	void addTileMap(string t, int offsetX, int offsetY) {
		int startX = offsetX * tileSize,
				startY = offsetY * tileSize,
				endX = startX + tileSize - 1,
				endY = startY + tileSize - 1;

		tilesMap.insert(pair<string, CImg<unsigned char> >(
						t, 
						tileImage.get_crop(startX, startY, endX, endY)
		));
	}

};

string const TileManager::CHAR_TILE_PREFIX = "CHAR.";
int TileManager::tileSize = 16;

#endif