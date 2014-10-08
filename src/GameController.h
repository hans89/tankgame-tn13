#ifndef __TANKGAME_GAMECONTROLLER__
#define __TANKGAME_GAMECONTROLLER__

#include "Command.h"

class GameController {
private:
  GameView* _view;
  GameModel* _model;
  CImgDisplay& _mainDisplay;
  vector<IPlayer*> _players;
  int _currentPlayerTurn;

  bool _autoMode;
  bool _ended;
  bool _ending;

public:
  GameController(GameView* v, GameModel* m, CImgDisplay& dis, autoM = true) 
    : _view(v), _model(m), _mainDisplay(dis),
      _autoMode(autoM), _ended(false), _ending(false),
      _currentPlayerTurn(0) {}

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
  
    IPlayer* currentPlayer = _model->getPlayer(currentPlayerTurn++);
    // reduce down:
    currentPlayerTurn %= _players.size();

    Command nextMove = currentPlayer->nextMove();

    if (_model->isValidMove(currentPlayer, nextMove)) {
      vector<pair<int,int>> changes = _model->applyMove(currentPlayer, nextMove);

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

  void updateDisplay() {
    _view->updateDisplay(_mainDisplay);
  }
};
#endif