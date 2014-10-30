#include "ConcurrentGameController.h"

ConcurrentGameController::ConcurrentGameController()
        : GameController(), turnCount(0) {}

// the controller makes the next turn
bool ConcurrentGameController::nextTurn() {
  // moves
  // tryMove will try to order (serialize) 2 moves
  CommandInfo move1, move2;

  move1.commander = _players[0]->getPlayerInfo();
  move1.originalCommand = _players[0]->nextMove();

  move2.commander = _players[1]->getPlayerInfo();
  move2.originalCommand = _players[1]->nextMove();

  bool dependent = false;
  pair<CommandInfo*, CommandInfo*> realMoves
    = _model->tryMove(move1, move2, dependent);

  vector<pair<int,int> > changes;
  
  // in case of both moves are firing, we can't serialize it 
  // and will have special calculation

  std::cout << "-- Turn " << ++turnCount << " ---" << endl;
  if (dependent) {
    // special calculation for both firing
    // we are not sure of the order, so we are forced to resolve it concurrently

    logMove(*realMoves.first);
    logMove(*realMoves.second);

    animateMove(realMoves.first->executedCommand);
    animateMove(realMoves.second->executedCommand);

    changes = _model->applyMove(*(realMoves.first),*(realMoves.second));
    _view->update(changes);  
  } else {
    // moves have been re-ordered by the tryMove method
    logMove(*realMoves.first);
    
    animateMove(realMoves.first->executedCommand);
    changes = _model->applyMove(realMoves.first->commander, 
                                realMoves.first->executedCommand);    
    _view->update(changes);  

    logMove(*realMoves.second);

    animateMove(realMoves.second->executedCommand);
    changes = _model->applyMove(realMoves.second->commander,
                                realMoves.second->executedCommand);
    _view->update(changes);
  }
  
  _ending = _model->isEndGame();
  // cout << nextMoveMessage << std::endl;
  return true;
}


void ConcurrentGameController::logMove(const CommandInfo& cmd, 
                                        ostream& out) const {
  string nextMoveMessage = cmd.commander->getPlayerMapID() 
          + string(": ") + cmd.originalCommand.toString();

  if (cmd.executedCommand.getActionType() == Command::SKIP)
    nextMoveMessage += " SKIP!!!";

  out << nextMoveMessage << endl;
}