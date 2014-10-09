#ifndef __TANKGAME_GAMECREATOR__
#define __TANKGAME_GAMECREATOR__

#include "AppConfig.h"
#include "IController.h"

class GameCreator {
public:
  static IController* createGame(const char* configFile);
};

#endif