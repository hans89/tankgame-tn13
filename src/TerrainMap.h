#ifndef __TERRAIN_MAP__
#define __TERRAIN_MAP__

#include <vector>
#include <string>
#include <fstream>
#include <map>

#include "TileManager.h"
#include "include/CImg.h"

using namespace std;

class TerrainMap {
private:
	int width, height;
	vector<string> charMap;
	map<char, TileManager::Tile> charToTileMapping;
	CImg<unsigned char> *displayImg;
	TileManager *tileManager;

	void addCharTileMapping(char c, TileManager::Tile t) {
		charToTileMapping.insert(pair<char, TileManager::Tile>(c, t));
	}
public:
	TerrainMap() {

		addCharTileMapping('%', TileManager::WATER);
		addCharTileMapping(' ', TileManager::GRASS);
		addCharTileMapping('W', TileManager::WALL);
		addCharTileMapping('B', TileManager::BRIDGE_L);
		addCharTileMapping('b', TileManager::BRIDGE_U);
	}

	void loadMap(const char * file) {
		ifstream inStream(file);

		string line;

		while (getline(inStream, line).good()) {
			charMap.push_back(line);
			//cout << line << endl;
		}

		height = charMap.size();
		if (height > 0) {
			width = charMap[0].size();
		}
	}

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

	void setDisplayView(CImg<unsigned char>* dImg) {
		displayImg = dImg;
	}

	void setTileManager(TileManager* tMan) {
		tileManager = tMan;
	}

	void initView() {
		for (int j = 0; j < charMap.size(); j++) {
			for (int i = 0; i < charMap[j].size(); i++) {
				displayImg->draw_image(
					i * TileManager::TileSize,
					j * TileManager::TileSize,
					0, 0,
					tileManager->getTile(charToTileMapping[charMap[j][i]])
				);
			}
		}
	}
};

#endif