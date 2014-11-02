#include "BaseGameView.h"

#define _DRAW_STRLEN_ 256
char __drawString[_DRAW_STRLEN_];
unsigned char 
  __cWhite[] = {255, 255, 255}
, __cBlack[] = {0, 0, 0}
, __cRed[] = {255, 0, 0}
, __cGreen[] = {0, 255, 0}
, __cBlue[] = {0, 0, 255}
, __cPink[] = {255, 192, 203}
, __cYellow[] = {255, 255, 0}
, __cOrangeRed[] = {255, 69, 0}
, __cMagenta2[] = {238, 0, 238}
, __cTurquoise1[] = {0, 245, 255}
;

const CImgList<unsigned char>& __bigFont 
    = CImgList<unsigned char>::font(23);

const CImgList<unsigned char>& __smallFont 
    = CImgList<unsigned char>::font(15);


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

BaseGameView::BaseGameView(
      TileManager* tileManager, 
      const BaseGameModel* model,
      const IController* controller) 
  : _tileManager(tileManager), _model(model), _controller(controller),
    _displayOffset(32, 32),
    _backgroundInfoWidth(300),
    _backgroundInfoHeight(600),
    _infoOffsetY(90),
    _lineSpacing(24) {
    this->prepareCharToStringTileMap();


    int w = model->getMap()->getWidth() * tileManager->getTileSize()
             + _displayOffset.first + _backgroundInfoWidth;

    int h = model->getMap()->getHeight() * tileManager->getTileSize()
             + _displayOffset.second;

    h = h > _backgroundInfoHeight ? h : _backgroundInfoHeight;

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
    sprintf(__drawString, "%2d", i);
    _displayImg->draw_text(8, j, __drawString, __cWhite);
  }

  for (int j = tileSize + 8, i = 0; i < width; i++, j += tileSize){
    sprintf(__drawString, "%2d", i);
    _displayImg->draw_text(j, 8, __drawString, __cWhite);
  }

  sprintf(__drawString, 
    "Press R/r to switch between\nauto-run mode and manual mode.");
  _displayImg->draw_text(_infoOffsetX + 8, 8, __drawString, 
                          __cYellow, __cBlack, 1.0, __smallFont);

  sprintf(__drawString, "In manual mode, press SPACE to step.");
  _displayImg->draw_text(_infoOffsetX + 8, 8 + 32, __drawString, 
                          __cYellow, __cBlack, 1.0, __smallFont);

  sprintf(__drawString, "Maximum turn: %5d",
          _model->getBaseMap()->getMapInfo().maxStep);

  _displayImg->draw_text(_infoOffsetX + 8, 8 + 64, __drawString, 
                          __cYellow, __cBlack, 1.0, __smallFont);

  updateInfo();

  // init main map
  // blend _tileMap
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      blendTiles(i, j);
    }
  }
}

void BaseGameView::updateInfo(bool endgame) {
  
  int tileSize = _tileManager->getTileSize();
  int mapHeight = _model->getMap()->getHeight() * tileSize;
  
  int currentY = _infoOffsetY;
  int currentX = _infoOffsetX + 8;

  _displayImg->draw_rectangle(_infoOffsetX, _infoOffsetY, 
          _infoOffsetX + _backgroundInfoWidth,
          mapHeight + _displayOffset.second,
          __cBlack);

  if (_controller->isInAutoMode())
    sprintf(__drawString, "Current mode: AUTO");
  else 
    sprintf(__drawString, "Current mode: MANUAL");
  
  _displayImg->draw_text(currentX, currentY, __drawString,
                          __cPink, __cBlack, 1.0, __smallFont);
  currentY += _lineSpacing;

  sprintf(__drawString, "Current turn: %5d", _model->getCurrentTurnCount());
  _displayImg->draw_text(currentX, currentY, __drawString,
                          __cTurquoise1, __cBlack, 1.0, __smallFont);
  currentY += _lineSpacing;

  vector<IPlayerInfo*> playerInfos = _model->getPlayersInfo();
  IPlayerInfo* cur;
  for (int i = 0; i < playerInfos.size(); i++) {
    cur = playerInfos[i];
    pair<int,int> pos = cur->getHeadquarterPosition();

    sprintf(__drawString, "Team %c - Head: (%d, %d)", cur->getPlayerMapID(), 
            pos.first, pos.second);

    _displayImg->draw_text(currentX, currentY, __drawString, 
                            __cGreen, __cBlack, 1.0, __bigFont);
    currentY += _lineSpacing;

    sprintf(__drawString, "Pos\t\tHP\tAmmo\tRange");
    _displayImg->draw_text(currentX, currentY, __drawString, 
                          __cPink, __cBlack, 1.0, __smallFont);
    currentY += _lineSpacing;

    list<ITank*> tanks = cur->getAliveTanks();
    for (list<ITank*>::iterator it = tanks.begin(); it != tanks.end(); ++it) {
      ITank* tank = *it;

      pos = tank->getPosition();

      sprintf(__drawString, "(%2d, %2d)\t%2d\t%4d\t%5d",
            pos.first, pos.second, 
            tank->getHP(), tank->getAmmoNumber(),
            tank->getRange());

      _displayImg->draw_text(currentX, currentY, __drawString, 
                              __cWhite, __cBlack, 1.0, __smallFont);
      currentY += _lineSpacing;

    }
  }

  if (endgame) {
    sprintf(__drawString, "--- END GAME ---");

    _displayImg->draw_text(currentX, currentY, __drawString, 
                            __cPink, __cBlack, 1.0, __bigFont);
    currentY += _lineSpacing;

    vector<PlayerEndGameInfo> egInfos(playerInfos.size());

    for (int i = 0; i < playerInfos.size(); i++) {
      cur = playerInfos[i];
      egInfos[i] = _model->getPlayerEndGameInfo(cur);

      sprintf(__drawString, "Team %c",cur->getPlayerMapID());
      _displayImg->draw_text(currentX, currentY, __drawString, 
                            __cGreen, __cBlack, 1.0, __smallFont);
      currentY += _lineSpacing;

      sprintf(__drawString, "Tanks: %2d Tot.HP: %3d Tot.Dist.: %3d",
            egInfos[i].totalTanks,
            egInfos[i].totalHP,
            egInfos[i].totalDistance);
      _displayImg->draw_text(currentX, currentY, __drawString, 
                            __cGreen, __cBlack, 1.0, __smallFont);
      currentY += _lineSpacing;
    }

    int gameResult = _model->getGameFinalResult(egInfos);

    if (gameResult > 0)
      sprintf(__drawString, "WINNER IS %c!!!",
            playerInfos[gameResult]->getPlayerMapID());
    else
      sprintf(__drawString, "): TIE GAME :(");

    _displayImg->draw_text(currentX, currentY, __drawString, 
                            __cPink, __cBlack, 1.0, __bigFont);
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