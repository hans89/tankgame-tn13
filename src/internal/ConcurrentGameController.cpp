#include "ConcurrentGameController.h"

// the controller makes the next turn
bool ConcurrentGameController::nextTurn() {

  _model->applyNewTurnAutoEffects();
  // moves
  // tryMove will try to order (serialize) 2 moves
  CommandInfo move1, move2;
  std::cout << "-- Turn " << _model->getCurrentTurnCount() << " ---" << endl;

  move1.commander = _players[0]->getPlayerInfo();
  move1.originalCommand = _players[0]->nextMove();
  cout << "Player " << 0 << endl;
  
  move2.commander = _players[1]->getPlayerInfo();
  move2.originalCommand = _players[1]->nextMove();
  cout << "Player " << 1 << endl;

  bool dependent = false;
  pair<CommandInfo*, CommandInfo*> realMoves
    = _model->tryMove(move1, move2, dependent);

  vector<pair<int,int> > changes;
  
  // in case of both moves are firing, we can't serialize it 
  // and will have special calculation
  _model->nextTurnCount();

  
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

  _view->updateInfo();  
  _ending = _model->isEndGame();

  ((BasePlayerInfo*)move1.commander)->updateLastMove(move1);
  ((BasePlayerInfo*)move2.commander)->updateLastMove(move2);
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