
#include <sstream>
#include "TileManager.h"

#ifdef DEBUG
#include <iostream>	
#endif


TileManager::TileManager(AppConfig* config) : 
	tileImage(config->getConfig("tile").c_str()) {
	
	tileSize = Utils::parseInt(config->getConfig("tile_size"));

	string tileResource = config->getConfig("tile_list");

	vector<string> tiles = Utils::split(tileResource, ',');

	#ifdef DEBUG
		for (int i = 0; i < tiles.size(); i++)
			cout << tiles[i] << ' ';
	#endif

	for (int i = 0; i < tiles.size(); i++) {
		pair<int,int> offset = Utils::parseIntPair(config->getConfig(tiles[i]));
		addTileMap(tiles[i], offset.first, offset.second);				
	}
}

CImg<unsigned char>& TileManager::getTile(string t) {
	return tilesMap[t];
}

int TileManager::getTileSize() {
	return tileSize;
}

void TileManager::addTileMap(string t, int offsetX, int offsetY) {
	int startX = offsetX * tileSize,
			startY = offsetY * tileSize,
			endX = startX + tileSize - 1,
			endY = startY + tileSize - 1;

	tilesMap.insert(pair<string, CImg<unsigned char> >(
					t, 
					tileImage.get_crop(startX, startY, endX, endY)
	));
}

int TileManager::tileSize = 16;