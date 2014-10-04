#ifndef __TILE_MANAGER__
#define __TILE_MANAGER__

#include <string>
#include <map>
#include <iostream>

#include "includes/CImg.h"


using namespace std;
using namespace cimg_library;

class TileManager {
public:
	static int const TileSize = 24;

	enum Tile { 
		LAND,
		WATER,
		GRASS,
		BRIDGE_L,
		BRIDGE_U,
		WALL,
		GREEN_TANK_L,
		GREEN_TANK_R,
		GREEN_TANK_U,
		GREEN_TANK_D,
		BLUE_TANK_L,
		BLUE_TANK_R,
		BLUE_TANK_U,
		BLUE_TANK_D,
		FIRE_S,
		FIRE_M
	};

	TileManager() : 
		tileImage(TileResourceFile.c_str()) {
			addTileMap(LAND, 4, 3);
			addTileMap(WATER, 6, 3);
			addTileMap(GRASS, 5, 3);
			addTileMap(BRIDGE_L, 8, 6);
			addTileMap(BRIDGE_U, 2, 4);

			addTileMap(WALL, 10, 1);
			addTileMap(GREEN_TANK_L, 19, 3);
			addTileMap(GREEN_TANK_R, 16, 2);
			addTileMap(GREEN_TANK_U, 14, 1);
			addTileMap(GREEN_TANK_D, 15, 1);
			addTileMap(BLUE_TANK_L, 19, 8);
			// addTileMap(BLUE_TANK_R, , );
			// addTileMap(BLUE_TANK_U, , );
			// addTileMap(BLUE_TANK_D, , );
			addTileMap(FIRE_M, 1, 0);
			addTileMap(FIRE_S, 0, 0);

	}

	CImg<unsigned char>& getTile(Tile t) {
		return tilesMap[t];
	}

private:

	CImg<unsigned char>	tileImage;
	map<Tile, CImg<unsigned char> > tilesMap;

	static string const TileResourceFile;

	void addTileMap(Tile t, int offsetX, int offsetY) {
		int startX = offsetX * TileSize,
				startY = offsetY * TileSize,
				endX = startX + TileSize - 1,
				endY = startY + TileSize - 1;

		tilesMap.insert(pair<Tile, CImg<unsigned char> >(
						t, 
						tileImage.get_crop(startX, startY, endX, endY)
		));
	}

};

string const TileManager::TileResourceFile = "res/tilesx24.png";


#endif