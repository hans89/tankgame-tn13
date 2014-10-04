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
	CImg<unsigned char> *displayImg;
	TileManager *tileManager;

	vector<vector<vector<string> > > tileMap;
public:

	void loadMap(const char * file) {
		ifstream inStream(file);

		string line;

		while (getline(inStream, line).good()) {
			charMap.push_back(line);

			vector<vector<string> > tileLine;
			for (int i = 0; i < line.size(); i++) {
				vector<string> tileLayers;

				if (line[i] == '0' || line[i] == ' ') {
				} else if (line[i] == '-' || line[i] == '|' 
					|| line[i] == 'T' || line[i] == 'S') {
					tileLayers.push_back(tileManager->getCharTileName('0'));
				} else if (line[i] == 'a' || line[i] == 'A' 
					|| line[i] == 'b' || line[i] == 'B'
					|| line[i] == 'U' || line[i] == 'V' || line[i] == 'W' 
					|| line[i] == 'X' || line[i] == 'Y' || line[i] == 'Z') {
					tileLayers.push_back(tileManager->getCharTileName(' '));
				}

				tileLayers.push_back(tileManager->getCharTileName(line[i]));
				tileLine.push_back(tileLayers);
			}

			tileMap.push_back(tileLine);

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
		int tileSize = TileManager::getTileSize();
		for (int j = 0; j < tileMap.size(); j++) {
			for (int i = 0; i < tileMap[j].size(); i++) {
				blendTiles(
					i * tileSize,
					j * tileSize,
					displayImg,
					tileMap[j][i]
				);
			}
		}
	}

	void addFire(int offsetX, int offsetY, string fire) {
		tileMap[offsetY][offsetX].push_back(fire);
		updateTile(offsetX, offsetY);
	}

	void removeFire(int offsetX, int offsetY) {
		int size = tileMap[offsetY][offsetX].size();

		tileMap[offsetY][offsetX].pop_back();
		updateTile(offsetX, offsetY);
		
	}

private:
	void blendTiles(int x, int y, 
				CImg<unsigned char> *displayImg, 
				const vector<string>& tileNames) {

		if (tileNames.size() > 0) {

			// draw background with first tile
			displayImg->draw_image(
					x, y, 0, 0,
					tileManager->getTile(tileNames[0])
			);

			// draw the rest with alpha layer (get_channel(3))
			for (int i = 1; i < tileNames.size(); i++) {
				CImg<unsigned char>& tile = tileManager->getTile(tileNames[i]);
				displayImg->draw_image(
					x, y, 0, 0,
 					tile, tile.get_channel(3), 1, 255
 				);
			}
		}
	}

	void updateTile(int offsetX, int offsetY) {
		int tileSize = TileManager::getTileSize();
		blendTiles(
					offsetX * tileSize,
					offsetY * tileSize,
					displayImg,
					tileMap[offsetY][offsetX]
				);
	}


};

#endif