
#include <sstream>
#include "TileManager.h"

TileManager::TileManager(AppConfig* config) : 
	tileImage(config->getConfig("tile").c_str()) {
	
	string tileSizeStr = config->getConfig("tile_size");
	istringstream iss(tileSizeStr);
	iss >> tileSize;

	string tileResource = config->getConfig("tile_list");

	vector<string> tiles = Utils::split(tileResource, ',');

	for (int i = 0; i < tiles.size(); i++) {
		pair<int,int> offset = Utils::parseIntPair(config->getConfig(tiles[i]));
		addTileMap(tiles[i], offset.first, offset.second);				
	}
}

CImg<unsigned char>& TileManager::getTile(string t) {
	return tilesMap[t];
}

CImg<unsigned char>& TileManager::getCharTile(char t) {
	return tilesMap[CHAR_TILE_PREFIX + t];
}

string TileManager::getCharTileName(char t) const {
	return CHAR_TILE_PREFIX + t;
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

string const TileManager::CHAR_TILE_PREFIX = "CHAR.";
int TileManager::tileSize = 16;