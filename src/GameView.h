#ifndef __TANKGAME_GAMEVIEW__
#define __TANKGAME_GAMEVIEW__

#include "include/CImg.h"


class GameView {

  CImg<unsigned char>* displayImg;
  TileManager* _tileManager;
  vector<vector<vector<string> > > _tileMap;

  void blendTiles(int x, int y, 
        CImg<unsigned char> *displayImg, 
        const vector<string>& tileNames) {

    if (tileNames.size() > 0) {

      // draw background with first tile
      displayImg->draw_image(
          x, y, 0, 0,
          _tileManager->getTile(tileNames[0])
      );

      // draw the rest with alpha layer (get_channel(3))
      for (int i = 1; i < tileNames.size(); i++) {
        CImg<unsigned char>& tile = _tileManager->getTile(tileNames[i]);
        displayImg->draw_image(
          x, y, 0, 0,
          tile, tile.get_channel(3), 1, 255
        );
      }
    }
  }

  void updateTileDisplay(int offsetX, int offsetY) {
    int tileSize = _tileManager->getTileSize();
    blendTiles(
      offsetX * tileSize,
      offsetY * tileSize,
      _displayImg,
      _tileMap[offsetY][offsetX]
    );
  }

public:
  GameView(CImg<unsigned char>* dImg, TileManager* tMan)
    : _displayImg(dImg), _tileManager(tMan) {}

  void setDisplayImage(CImg<unsigned char>* dImg) {
    _displayImg = dImg;
  }

  void setTileManager(TileManager* tMan) {
    _tileManager = tMan;
  }

  void initDisplay(const vector<string>& charMap) {
    
  }

  void updateTile() {}

  CImg<unsigned char>* getDisplayImage() {
    return _displayImg;
  }
};
#endif