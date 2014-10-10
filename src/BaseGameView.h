#ifndef __TANKGAME_BASEGAMEVIEW__
#define __TANKGAME_BASEGAMEVIEW__ 

class BaseGameView {
public:
  void setDisplay(CImg<unsigned char>* image, CImgDisplay* display) {
    _view->setDisplay(image, display);
  }
};

#endif