#include "BaseGameView.h"

using namespace std;

void BaseGameView::blendTiles(int x, int y, const vector<string>& tileNames) {

  if (tileNames.size() > 0) {

    // draw background with first tile
    _displayImg->draw_image(
        x, y, 0, 0,
        _tileManager->getTile(tileNames[0])
    );

    // draw the rest with alpha layer (get_channel(3))
    for (int i = 1; i < tileNames.size(); i++) {
      CImg<unsigned char>& tile = _tileManager->getTile(tileNames[i]);
      _displayImg->draw_image(
        x, y, 0, 0,
        tile, tile.get_channel(3), 1, 255
      );
    }
  }
}


BaseGameView::BaseGameView(TileManager* tileManager, const BaseGameModel* model) 
  : _tileManager(tileManager), _model(model) {

  if (_model != NULL) {

    // set up _tileMap by _model

    const BaseMap* baseMap = _model->getBaseMap();

    for (int j = 0; j < baseMap->getHeight(); j++) {
      vector<vector<string> > tileLine(baseMap->getWidth());

      for (int i = 0; i < baseMap->getWidth(); i++) {

        char c = (*baseMap)(i,j);

        if (baseMap->isEmptySpace(i, j)) {
          tileLine[i].push_back("LAND");
        } else if (baseMap->isWater(i, j)) {
          tileLine[i].push_back("WATER");
        } else if (baseMap->isBlock(i, j)) {
          tileLine[i].push_back("LAND");
          tileLine[i].push_back(string("BLOCK.") + c);
        } else if (baseMap->isBridge(i, j)) {
          tileLine[i].push_back("WATER");
          tileLine[i].push_back(string("BRIDGE.") + c);
        } else if (baseMap->isTank(i, j, c)) {
          tileLine[i].push_back("LAND");
          tileLine[i].push_back(string("TANK.") + c);
        } else if (baseMap->isHeadquarter(i,j)) {
          tileLine[i].push_back("LAND");
          tileLine[i].push_back(string("HEAD.") + c);
        }
      }

      _tileMap.push_back(tileLine);

    }
  }
}

const CImg<unsigned char>* BaseGameView::getDisplayImage() const {
  return _displayImg;
}

const CImgDisplay* BaseGameView::getDisplay() const {
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
  int tileSize = _tileManager->getTileSize();
  for (int j = 0; j < _tileMap.size(); j++) {
    for (int i = 0; i < _tileMap[j].size(); i++) {
      blendTiles(
        i * tileSize,
        j * tileSize,
        _tileMap[j][i]
      );
    }
  }
}

void BaseGameView::display() {
  _displayImg->display(*_display);
}

void BaseGameView::update(const vector<pair<int,int> >& changes) {
  int tileSize = _tileManager->getTileSize();

  for (int i = 0; i < changes.size(); i++) {
    pair<int,int> pos = changes[i];
    blendTiles(
      pos.first * tileSize,
      pos.second * tileSize,
      _tileMap[pos.first][pos.second]
    );
  }
}