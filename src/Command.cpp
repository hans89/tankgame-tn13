#include "Command.h"

#include <sstream>
std::pair<int, int> Command::getTargetPosition() const {
  return targetPosition;
}

ITank* Command::getReceivingObject() const {
  return receivingObject;
}

Command::Action Command::getActionType() const {
  return actionType;
}

std::string Command::toString() const {
  std::ostringstream act;
  switch(actionType) {
  case MOVE:
    act << "MOVE TANK FROM " 
      << "(" << receivingObject->getPosition().first
      << "," << receivingObject->getPosition().second << ") "
      << "TO" 
      << " (" << targetPosition.first << "," << targetPosition.second << ")";
    break;
  case FIRE:
    act << "FIRE (" << targetPosition.first << "," << targetPosition.second << ") "
      << "FROM TANK " 
      << "(" << receivingObject->getPosition().first
      << "," << receivingObject->getPosition().second << ")";
    break;
  case SKIP:
    act << "SKIP";
    break;
  case SURRENDER:
    act << "SURRENDER";
    break;
  }
  return act.str();
}