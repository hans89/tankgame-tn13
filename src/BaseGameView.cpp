#include "BaseGameView.h"

unsigned char BaseGameView::_cWhite[3] = {255, 255, 255};
unsigned char BaseGameView::_cBlack[3] = {0, 0, 0};
char BaseGameView::_drawString[_DRAW_STRLEN_];

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
    _displayOffset(32, 32),
    _backgroundInfoWidth(300) {
    this->prepareCharToStringTileMap();


    int w = model->getMap()->getWidth() * tileManager->getTileSize()
             + _displayOffset.first + _backgroundInfoWidth;

    int h = model->getMap()->getHeight() * tileManager->getTileSize()
             + _displayOffset.second;

    _displayImg = new CImg<unsigned char>(w, h, 1, 3, 0); 

    _infoOffsetX = _model->getMap()->getWidth() * _tileManager->getTileSize()
             + _displayOffset.first;
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


void BaseGameView::initDisplay() {
  if (_model == NULL || _tileManager == NULL)
    return;

  int width = _model->getMap()->getWidth();
  int height = _model->getMap()->getHeight();

  // init border
  int tileSize = _tileManager->getTileSize();
  

  for (int j = tileSize + 8, i = 0; i < height; i++, j += tileSize){
    sprintf(_drawString, "%2d", i);
    _displayImg->draw_text(8, j, _drawString, _cWhite);
  }

  for (int j = tileSize + 8, i = 0; i < width; i++, j += tileSize){
    sprintf(_drawString, "%2d", i);
    _displayImg->draw_text(j, 8, _drawString, _cWhite);
  }

  updateInfo();

  // init main map
  // blend _tileMap
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      blendTiles(i, j);
    }
  }
}

void BaseGameView::updateInfo() {
  
  int tileSize = _tileManager->getTileSize();

  sprintf(_drawString, "HelloWorld\nHelloMe");
  
  int currentY = 8;
  int currentX = _infoOffsetX + 8;

  _displayImg->draw_rectangle(_infoOffsetX, 0, 
          _infoOffsetX + _backgroundInfoWidth,
          _model->getMap()->getHeight() * tileSize + _displayOffset.second,
          _cBlack);

  vector<IPlayerInfo*> playerInfos = _model->getPlayersInfo();
  IPlayerInfo* cur;
  for (int i = 0; i < playerInfos.size(); i++) {
    cur = playerInfos[i];
    pair<int,int> pos = cur->getHeadquarterPosition();

    sprintf(_drawString, "Team %c - Head: (%d, %d)", cur->getPlayerMapID(), 
            pos.first, pos.second);

    _displayImg->draw_text(currentX, currentY, _drawString, _cWhite);
    currentY += tileSize;

    sprintf(_drawString, "Alive Tanks\tPos\t\tHP\tAmmo\tRange");
    _displayImg->draw_text(currentX, currentY, _drawString, _cWhite);
    currentY += tileSize;

    list<ITank*> tanks = cur->getAliveTanks();
    for (list<ITank*>::iterator it = tanks.begin(); it != tanks.end(); ++it) {
      ITank* tank = *it;

      pos = tank->getPosition();

      sprintf(_drawString, "\t\t\t\t(%2d, %2d)\t%2d\t%4d\t%5d",
            pos.first, pos.second, 
            tank->getHP(), tank->getAmmoNumber(),
            tank->getRange());

      _displayImg->draw_text(currentX, currentY, _drawString, _cWhite);
      currentY += tileSize;

    }
    
    // sprintf(_drawString, "Team %c - Head: (%d, %d)", cur->getPlayerMapID(), 
    //         head.first, head.second);
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