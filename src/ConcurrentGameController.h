#ifndef __TANKGAME_CONCURRENT_GAMECONTROLLER
#define __TANKGAME_CONCURRENT_GAMECONTROLLER

#include "GameController.h"

#include <iostream>
class ConcurrentGameController : public GameController {
public:
  #pragma region IControllerImplementation
  bool nextTurn();
  #pragma endregion
  void logMove(const CommandInfo& cmd, ostream& out = std::cout) const;
};

#endif