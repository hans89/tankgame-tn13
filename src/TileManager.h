#ifndef __TANKGAME_TILE_MANAGER__
#define __TANKGAME_TILE_MANAGER__

#include <string>
#include <map>

#include "include/CImg.h"
#include "include/utils.h"
#include "AppConfig.h"


using namespace std;
using namespace cimg_library;

class TileManager {
public:
	TileManager(AppConfig* config);

	CImg<unsigned char>& getTile(string t);

	CImg<unsigned char>& getCharTile(char t);

	static int getTileSize();

private:
	static int tileSize;
	CImg<unsigned char>	tileImage;
	map<string, CImg<unsigned char> > tilesMap;
  map<char, string> charStringTileMap;
	
	void addTileMap(string t, int offsetX, int offsetY);

};


#endif