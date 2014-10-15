#ifndef __TANKGAME_BASEGAMEVIEW__
#define __TANKGAME_BASEGAMEVIEW__ 

#include <vector>
#include <string>
#include "include/CImg.h"
#include "TileManager.h"
#include "BaseGameModel.h"

using namespace cimg_library;

class BaseGameView {
public:

  BaseGameView(TileManager* tileManager, const BaseGameModel* model);

  const CImg<unsigned char>* getDisplayImage() const;

  CImgDisplay* getDisplay() const;

  void setDisplay(CImg<unsigned char>* image, CImgDisplay* display);

  void initDisplay();
  void display();

  void update(const std::vector<std::pair<int,int> >& changes);

  void addFire(int offsetX, int offsetY, string fire);
  void removeFire(int offsetX, int offsetY);

protected:
  CImg<unsigned char>* _displayImg;
  CImgDisplay* _display;

  const BaseGameModel* _model;
  TileManager* _tileManager;

  void blendTiles(int x, int y);

  void prepareCharToStringTileMap();
  map<char, string> c2StrTileMap;
};

#endif