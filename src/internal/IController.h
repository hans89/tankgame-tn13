#ifndef __TANKGAME_ICONTROLLER__
#define __TANKGAME_ICONTROLLER__ 

#include "../include/CImg.h"
#include "AppConfig.h"
#include "TileManager.h"
#include "../IPlayer.h"

using namespace cimg_library;

class IController {
public:
  // Controller setups
  virtual void setDisplay(CImgDisplay* display) = 0;

  virtual bool registerPlayer(IPlayer* player) = 0;

  virtual string getConfig(string key) const = 0;

  // Game controls
  virtual bool start() = 0;
  virtual bool finish() = 0;
  virtual bool isEnding() const = 0;
  virtual bool isEnded() const = 0;
  virtual bool isInAutoMode() const = 0;
  virtual void toggleMode() = 0;
  virtual bool nextTurn() = 0;
  virtual void updateDisplay() = 0;

  virtual ~IController() {}
};
#endif