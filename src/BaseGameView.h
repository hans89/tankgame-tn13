#ifndef __TANKGAME_BASEGAMEVIEW__
#define __TANKGAME_BASEGAMEVIEW__ 

#include <vector>
#include <string>
#include "include/CImg.h"
#include "TileManager.h"
#include "BaseGameModel.h"

using namespace cimg_library;

class BaseGameView {
protected:
  CImg<unsigned char>* _displayImg;
  CImgDisplay* _display;

  std::vector<std::vector<std::vector<std::string> > > _tileMap;

  const BaseGameModel* _model;
  TileManager* _tileManager;

  void blendTiles(int x, int y, const std::vector<std::string>& tileNames);

public:

  BaseGameView(TileManager* tileManager, const BaseGameModel* model);

  const CImg<unsigned char>* getDisplayImage() const;

  const CImgDisplay* getDisplay() const;

  void setDisplay(CImg<unsigned char>* image, CImgDisplay* display);

  void initDisplay();

  void display();

  void update(const std::vector<std::pair<int,int> >& changes);
};

#endif