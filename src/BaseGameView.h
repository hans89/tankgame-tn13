#ifndef __TANKGAME_BASEGAMEVIEW__
#define __TANKGAME_BASEGAMEVIEW__ 

#include "include/CImg.h"

using namespace cimg_library;

class BaseGameView {
protected:
	CImg<unsigned char>* _displayImg;
	CImgDisplay* _display;

  vector<vector<vector<string> > > _tileMap;

  const BaseGameModel* _model;
  TileManager* _tileManager;

  void blendTiles(int x, int y, const vector<string>& tileNames) {

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

public:

	BaseGameView(const TileManager* tileManager, const BaseGameModel* model) 
		: _tileManager(tileManager), _model(model) {}

	const CImg<unsigned char>* getDisplayImage() const {
    return _displayImg;
  }

  const CImgDisplay* getDisplay() const {
  	return _display;
  }

  void setDisplay(CImg<unsigned char>* image, CImgDisplay* display) {
    _displayImg = image;
    _display = display;
  }

  void setModel(const BaseGameModel* model) {
  	_model = model;
  }

  void initDisplay() {
  	if (_model == NULL || _tileManager == NULL)
  		return;

  	// set up _tileMap by _model

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

  void display() {
  	_displayImg->display(*_display);
  }

  void update(const vector<pair<int,int> >& changes) {
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
};

#endif