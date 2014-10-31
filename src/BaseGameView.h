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
  ~BaseGameView();

  const CImg<unsigned char>* getDisplayImage() const;

  CImgDisplay* getDisplay() const;

  void setDisplay(CImgDisplay* display);

  void initDisplay();
  void display();

  void updateInfo();
  void update(const std::vector<std::pair<int,int> >& changes);

  void addFire(int offsetX, int offsetY, string fire);
  void removeFire(int offsetX, int offsetY);

protected:
  
  const BaseGameModel* _model;
  TileManager* _tileManager;
  CImgDisplay* _display;
  CImg<unsigned char>* _displayImg;
  pair<int,int> _displayOffset;
  int _backgroundInfoWidth;
  
  void blendTiles(int x, int y);

  map<char, string> _c2StrTileMap;
  void prepareCharToStringTileMap();


  pair<int,int> calculateOffset(int x, int y);
  int _infoOffsetX;
  int _infoOffsetY;
  int _lineSpacing;
};

#endif