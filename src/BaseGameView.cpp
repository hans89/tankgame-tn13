#include "BaseGameView.h"

using namespace std;

void BaseGameView::blendTiles(int x, int y) {
  int tileSize = _tileManager->getTileSize();

  const string& tileLayers 
      = _model->getBaseMap()->getLayersAt(x, y);

  if (tileLayers.size() > 0) {

    // draw background with first tile
    _displayImg->draw_image(
        x * tileSize, y * tileSize, 0, 0,
        _tileManager->getTile(c2StrTileMap[tileLayers[0]])
    );

    // draw the rest with alpha layer (get_channel(3))
    for (int i = 1; i < tileLayers.size(); i++) {
      CImg<unsigned char>& tile = _tileManager->getTile(c2StrTileMap[tileLayers[i]]);
      _displayImg->draw_image(
        x  * tileSize, y  * tileSize, 0, 0,
        tile, tile.get_channel(3), 1, 255
      );
    }
  }
}

void BaseGameView::prepareCharToStringTileMap() {
  const MapInfo& mapInfo = _model->getBaseMap()->getMapInfo();


  c2StrTileMap.insert(pair<char,string>(mapInfo.landID, "LAND"));
  c2StrTileMap.insert(pair<char,string>(mapInfo.waterID, "WATER"));

  int i;
  for (i = 0; i < mapInfo.bridgeIDs.size(); i++) {
    c2StrTileMap.insert(pair<char,string>(mapInfo.bridgeIDs[i], 
                                  string("BRIDGE.") + mapInfo.bridgeIDs[i]));    
  }
  
  for (i = 0; i < mapInfo.blockIDs.size(); i++) {
    c2StrTileMap.insert(pair<char,string>(mapInfo.blockIDs[i], 
                                  string("BLOCK.") + mapInfo.blockIDs[i]));    
  }

  for (i = 0; i < mapInfo.headquarterIDs.size(); i++) {
    c2StrTileMap.insert(pair<char,string>(mapInfo.headquarterIDs[i], 
                                  string("HEAD.") + mapInfo.headquarterIDs[i]));    
  }

  for (i = 0; i < mapInfo.playerIDs.size(); i++) {
    c2StrTileMap.insert(pair<char,string>(mapInfo.playerIDs[i], 
                                  string("TANK.") + mapInfo.playerIDs[i]));    
  }
}

BaseGameView::BaseGameView(TileManager* tileManager, const BaseGameModel* model) 
  : _tileManager(tileManager), _model(model) {
    this->prepareCharToStringTileMap();
  }

const CImg<unsigned char>* BaseGameView::getDisplayImage() const {
  return _displayImg;
}

CImgDisplay* BaseGameView::getDisplay() const {
  return _display;
}

void BaseGameView::setDisplay(CImg<unsigned char>* image, CImgDisplay* display) {
  _displayImg = image;
  _display = display;
}


void BaseGameView::initDisplay() {
  if (_model == NULL || _tileManager == NULL)
    return;

  // blend _tileMap
  int width = _model->getMap()->getWidth();
  int height = _model->getMap()->getHeight();
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      blendTiles(i, j);
    }
  }
}

void BaseGameView::display() {
  _displayImg->display(*_display);
}

void BaseGameView::update(const vector<pair<int,int> >& changes) {
  for (int i = 0; i < changes.size(); i++) {
    pair<int,int> pos = changes[i];
    blendTiles(pos.first, pos.second);
  }
}

void BaseGameView::addFire(int offsetX, int offsetY, string fire) {
  blendTiles(offsetX, offsetY);
  int tileSize = _tileManager->getTileSize();
  CImg<unsigned char>& tile = _tileManager->getTile(fire);
  _displayImg->draw_image(
    offsetX  * tileSize, offsetY  * tileSize, 0, 0,
    tile, tile.get_channel(3), 1, 255
  );
}

void BaseGameView::removeFire(int offsetX, int offsetY) {
  blendTiles(offsetX, offsetY);
}