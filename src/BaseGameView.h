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

  void blendTiles(int x, int y, const vector<string>& tileNames);

public:

  BaseGameView(TileManager* tileManager, const BaseGameModel* model);

  const CImg<unsigned char>* getDisplayImage() const;

  const CImgDisplay* getDisplay() const;

  void setDisplay(CImg<unsigned char>* image, CImgDisplay* display);

  void setModel(const BaseGameModel* model);

  void initDisplay();

  void display();

  void update(const vector<pair<int,int> >& changes);
};

#endif