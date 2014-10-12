#include "BaseGameView.h"

using namespace std;

void BaseGameView::blendTiles(int x, int y, const vector<string>& tileNames) {
  int tileSize = _tileManager->getTileSize();

  if (tileNames.size() > 0) {

    // draw background with first tile
    _displayImg->draw_image(
        x * tileSize, y * tileSize, 0, 0,
        _tileManager->getTile(tileNames[0])
    );

    // draw the rest with alpha layer (get_channel(3))
    for (int i = 1; i < tileNames.size(); i++) {
      CImg<unsigned char>& tile = _tileManager->getTile(tileNames[i]);
      _displayImg->draw_image(
        x  * tileSize, y  * tileSize, 0, 0,
        tile, tile.get_channel(3), 1, 255
      );
    }
  }
}

void BaseGameView::translateTile(int i, int j) {
  vector<string>& tileLayers = _tileMap[j][i];

  const BaseMap* baseMap = _model->getBaseMap();
  char c = (*baseMap)(i,j);
  tileLayers.clear();

  if (baseMap->isEmptySpace(i, j)) {
    tileLayers.push_back("LAND");
  } else if (baseMap->isWater(i, j)) {
    tileLayers.push_back("WATER");
  } else if (baseMap->isBlock(i, j)) {
    tileLayers.push_back("LAND");
    tileLayers.push_back(string("BLOCK.") + c);
  } else if (baseMap->isBridge(i, j)) {
    tileLayers.push_back("WATER");
    tileLayers.push_back(string("BRIDGE.") + c);
  } else if (baseMap->isTank(i, j, c)) {
    tileLayers.push_back("LAND");
    tileLayers.push_back(string("TANK.") + c);
  } else if (baseMap->isHeadquarter(i,j)) {
    tileLayers.push_back("LAND");
    tileLayers.push_back(string("HEAD.") + c);
  }
}

BaseGameView::BaseGameView(TileManager* tileManager, const BaseGameModel* model) 
  : _tileManager(tileManager), _model(model) {

  if (_model != NULL) {

    // set up _tileMap by _model

    const BaseMap* baseMap = _model->getBaseMap();
    int w = baseMap->getWidth(),
        h = baseMap->getHeight();

    _tileMap.resize(h);

    for (int j = 0; j < h; j++) {
      _tileMap[j].resize(w);

      for (int i = 0; i < w; i++) {

        char c = (*baseMap)(i,j);

        if (baseMap->isEmptySpace(i, j)) {
          _tileMap[j][i].push_back("LAND");
        } else if (baseMap->isWater(i, j)) {
          _tileMap[j][i].push_back("WATER");
        } else if (baseMap->isBlock(i, j)) {
          _tileMap[j][i].push_back("LAND");
          _tileMap[j][i].push_back(string("BLOCK.") + c);
        } else if (baseMap->isBridge(i, j)) {
          _tileMap[j][i].push_back("WATER");
          _tileMap[j][i].push_back(string("BRIDGE.") + c);
        } else if (baseMap->isTank(i, j, c)) {
          _tileMap[j][i].push_back("LAND");
          _tileMap[j][i].push_back(string("TANK.") + c);
        } else if (baseMap->isHeadquarter(i,j)) {
          _tileMap[j][i].push_back("LAND");
          _tileMap[j][i].push_back(string("HEAD.") + c);
        }
      }
    }
  }
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
      blendTiles(i, j, _tileMap[j][i]);
    }
  }
}

void BaseGameView::display() {
  _displayImg->display(*_display);
}

void BaseGameView::update(const vector<pair<int,int> >& changes) {
  for (int i = 0; i < changes.size(); i++) {
    pair<int,int> pos = changes[i];
    translateTile(pos.first, pos.second);
    blendTiles(pos.first, pos.second, _tileMap[pos.second][pos.first]);
  }
}


void BaseGameView::addFire(int offsetX, int offsetY, string fire) {
  _tileMap[offsetY][offsetX].push_back(fire);
  blendTiles(offsetX, offsetY, _tileMap[offsetY][offsetX]);
}

void BaseGameView::removeFire(int offsetX, int offsetY) {
  _tileMap[offsetY][offsetX].pop_back();
  blendTiles(offsetX, offsetY, _tileMap[offsetY][offsetX]);
}