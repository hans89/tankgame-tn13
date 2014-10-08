#ifndef __TANKGAME_GAMECONTROLLER__
#define __TANKGAME_GAMECONTROLLER__

#include "Command.h"

class GameController {
private:
  GameView* _view;
  GameModel* _model;
  list<IPlayer*> _players;

  bool _autoMode;
  bool _ended;
  bool _ending;

  int _currentPlayerTurn;

public:
  GameController(GameView* v, GameModel* m, autoM = true) 
    : _view(v), _model(m), _autoMode(autoM), 
      _ended(false), _ending(false) {}

  bool registerPlayer(IPlayer* player) {
    IPlayer* newPlayer = _model->registerPlayer(player);

    if (newPlayer != NULL) {
      _players.push_back(newPlayer);
      return true;
    }

    return false;
  }

  // the controller makes the next turn
  bool nextTurn() {
    int size = _model->getPlayers().size();

    IPlayer* currentPlayer = _model->getPlayer(currentPlayerTurn++ %= size);

    Command nextMove = currentPlayer->onNextMove();

    if (_model->isValidMove(nextMove)) {
      pair<int,int> changes = _model->applyMove(nextMove);

      _view->update(changes);

      _ending = _model->isEndGame();

      return true;
    } else {
      return false;
    }
  }

  bool start() {
    for (list<IPlayer*>::iterator it = _players.begin(); it != _players.end(); ++it) {
      (*it)->onStart();
    }
    
    return true;
  }

  bool finish() {
    for (list<IPlayer*>::iterator it = _players.begin(); it != _players.end(); ++it) {
      (*it)->onFinish();
    }
    _ended = true;
  }

  bool toggleMode() {
    _autoMode = !_autoMode;
  }

  bool isInAutoMode() {
    return _autoMode;
  }

  bool isEnding() {
    return _ending;
  }

  bool isEnded() {
    return _ended;
  }

  void updateDisplay(CImgDisplay& main_disp) {
    _view->updateDisplay(main_disp);
  }
};
#endif