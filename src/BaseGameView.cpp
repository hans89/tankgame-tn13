#include "BaseGameView.h"

using namespace std;

pair<int,int> BaseGameView::calculateOffset(int x, int y) {
  int tileSize = _tileManager->getTileSize();

  return pair<int,int>(x  * tileSize + _displayOffset.first, 
                        y  * tileSize + _displayOffset.second);
}

void BaseGameView::blendTiles(int x, int y) {
  
  const string& tileLayers 
      = _model->getBaseMap()->getLayersAt(x, y);

  if (tileLayers.size() > 0) {

    pair<int,int> offs = calculateOffset(x, y);
    // draw background with first tile
    _displayImg->draw_image(
        offs.first, offs.second, 0, 0,
        _tileManager->getTile(_c2StrTileMap[tileLayers[0]])
    );

    // draw the rest with alpha layer (get_channel(3))
    for (int i = 1; i < tileLayers.size(); i++) {
      CImg<unsigned char>& tile = _tileManager->getTile(_c2StrTileMap[tileLayers[i]]);
      _displayImg->draw_image(
        offs.first, offs.second, 0, 0,
        tile, tile.get_channel(3), 1, 255
      );
    }
  }
}

void BaseGameView::prepareCharToStringTileMap() {
  const MapInfo& mapInfo = _model->getBaseMap()->getMapInfo();


  _c2StrTileMap.insert(pair<char,string>(mapInfo.landID, "LAND"));
  _c2StrTileMap.insert(pair<char,string>(mapInfo.waterID, "WATER"));

  int i;
  for (i = 0; i < mapInfo.bridgeIDs.size(); i++) {
    _c2StrTileMap.insert(pair<char,string>(mapInfo.bridgeIDs[i], 
                                  string("BRIDGE.") + mapInfo.bridgeIDs[i]));    
  }
  
  for (i = 0; i < mapInfo.blockIDs.size(); i++) {
    _c2StrTileMap.insert(pair<char,string>(mapInfo.blockIDs[i], 
                                  string("BLOCK.") + mapInfo.blockIDs[i]));    
  }

  for (i = 0; i < mapInfo.headquarterIDs.size(); i++) {
    _c2StrTileMap.insert(pair<char,string>(mapInfo.headquarterIDs[i], 
                                  string("HEAD.") + mapInfo.headquarterIDs[i]));    
  }

  for (i = 0; i < mapInfo.playerIDs.size(); i++) {
    _c2StrTileMap.insert(pair<char,string>(mapInfo.playerIDs[i], 
                                  string("TANK.") + mapInfo.playerIDs[i]));    
  }
}

BaseGameView::BaseGameView(TileManager* tileManager, const BaseGameModel* model) 
  : _tileManager(tileManager), _model(model),
    _displayOffset(32, 32) {
    this->prepareCharToStringTileMap();

    _displayImg = new CImg<unsigned char>(
                model->getMap()->getWidth() * tileManager->getTileSize() + _displayOffset.first, 
                model->getMap()->getHeight() * tileManager->getTileSize() + _displayOffset.second,
                1, 3, 0); 
}

BaseGameView::~BaseGameView() {
  delete _displayImg;
}

const CImg<unsigned char>* BaseGameView::getDisplayImage() const {
  return _displayImg;
}

CImgDisplay* BaseGameView::getDisplay() const {
  return _display;
}

void BaseGameView::setDisplay(CImgDisplay* display) {
  _display = display;
}

#define __STRING_LEN__ 3
void BaseGameView::initDisplay() {
  if (_model == NULL || _tileManager == NULL)
    return;

  int width = _model->getMap()->getWidth();
  int height = _model->getMap()->getHeight();

  // init border
  int tileSize = _tileManager->getTileSize();
  char str[__STRING_LEN__];
  unsigned char white[] = {255, 255, 255};

  for (int j = tileSize + 8, i = 0; i < height; i++, j += tileSize){
    sprintf(str, "%2d", i);
    _displayImg->draw_text(8, j, str, white);
  }

  for (int j = tileSize + 8, i = 0; i < width; i++, j += tileSize){
    sprintf(str, "%2d", i);
    _displayImg->draw_text(j, 8, str, white);
  }

  // init main map
  // blend _tileMap
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

void BaseGameView::addFire(int x, int y, string fire) {
  blendTiles(x, y);

  // draw fire
  pair<int,int> offs = calculateOffset(x, y);

  CImg<unsigned char>& tile = _tileManager->getTile(fire);
  _displayImg->draw_image(
    offs.first, offs.second, 0, 0,
    tile, tile.get_channel(3), 1, 255
  );
}

void BaseGameView::removeFire(int x, int y) {
  blendTiles(x, y);
}