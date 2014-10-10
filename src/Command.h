#ifndef __TANKGAME_COMMAND__
#define __TANKGAME_COMMAND__

#include <utility>
#include "IMap.h"

class ITank;

/**
 * This struct defines the command the player want to make on the next move
 * Player must define which tank will do what, only one tank and one action
 * each command.
 * The framework will check if the command is valid, meaning that the appointed
 * tank is alive, and can conduct the action.
 * In case the player want to skip this turn, assign the actionType as SKIP.
 * In case the player want to surrender, assign the actionType as SURRENDER.
 */
struct Command {
public:
  enum Action {
    MOVE,
    FIRE,
    SKIP,
    SURRENDER
  };

private:
  ITank* receivingObject;
  Action actionType;
  std::pair<int, int> targetPosition;
  
public:

  Command() : 
    receivingObject(NULL), actionType(SKIP), targetPosition(IMap::npos) {}

  Command(ITank* receivObj, int targetX, int targetY) : 
    receivingObject(receivObj), 
    targetPosition(std::pair<int,int>(targetX, targetY)) {}

  std::pair<int, int> getTargetPosition() const {
    return targetPosition;
  }

  ITank* getReceivingObject() const {
    return receivingObject;
  }

  Action getActionType() const {
    return actionType;
  }
};

#endif