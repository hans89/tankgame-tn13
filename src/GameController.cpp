#include "GameController.h"

#include <iostream>
using namespace std;

GameController::GameController() :
    _autoMode(true), _ended(false), _ending(false), _currentPlayerTurn(0),
	_model(NULL), _view(NULL), _tileManager(NULL), _appConfig(NULL) {}


GameController::GameController(const GameController& g) {}

GameController& GameController::operator=(const GameController& g) {
  return *this;
}

#pragma region IControllerImplementation
void GameController::setDisplay(CImg<unsigned char>* image, 
        CImgDisplay* display) {
  _view->setDisplay(image, display);
}

int GameController::getDisplayWidth() const {
	return _model->getMap()->getWidth() * _tileManager->getTileSize();
}

int GameController::getDisplayHeight() const {
  return _model->getMap()->getHeight() * _tileManager->getTileSize();
}

string GameController::getConfig(string key) const {
  return _appConfig->getConfig(key);
}

bool GameController::registerPlayer(IPlayer* player) {
  IPlayer* newPlayer = _model->registerPlayer(player);

  if (newPlayer != NULL) {
    _players.push_back(newPlayer);
    return true;
  }

  return false;
}

// the controller makes the next turn
bool GameController::nextTurn() {
  
  // reduce down:
  int totalPlayer = _players.size();
  if (_currentPlayerTurn >= totalPlayer)
      _currentPlayerTurn %= totalPlayer;

  IPlayer* currentPlayer = _players[_currentPlayerTurn++];
  

  Command nextMove = currentPlayer->nextMove();

  string nextMoveMessage = currentPlayer->getPlayerInfo()->getPlayerMapID() 
          + string(": ") + nextMove.toString();

  if (_model->isValidMove(currentPlayer, nextMove)) {
    vector<pair<int,int> > changes = _model->applyMove(currentPlayer, nextMove);

    _view->update(changes);

    _ending = _model->isEndGame();

    cout << nextMoveMessage << std::endl;

    return true;
  } else {

    cout << nextMoveMessage << " INVALID!!! - SKIP" << std::endl;

    return false;
  }
}

bool GameController::start() {
  // init players
  for (int i = 0; i < _players.size(); i++) {
    _players[i]->onStart();
  }

  _currentPlayerTurn = 0;
  // init views
  _view->initDisplay();
  _view->display();
  
  return true;
}

bool GameController::finish() {
  for (int i = 0; i < _players.size(); i++) {
    _players[i]->onFinish();
  }
  _ended = true;
  return _ended;
}

void GameController::toggleMode() {
  _autoMode = !_autoMode;
}

bool GameController::isInAutoMode() const {
  return _autoMode;
}

bool GameController::isEnding() const {
  return _ending;
}

bool GameController::isEnded() const {
  return _ended;
}

void GameController::updateDisplay() {
  _view->display();
}

GameController::~GameController() {
  delete _appConfig;
  delete _tileManager;
  delete _model;
  delete _view;
}

#pragma endregion

#pragma region InternalPreservedInterfaces

void GameController::setConfig(AppConfig* config) {
  _appConfig = config;
}

void GameController::setTileManager(TileManager* tileManager) {
  _tileManager = tileManager;
}

void GameController::createGameModel() {
  if (_model != NULL) {
    delete _model;
    _model = NULL;
  }

  MapInfo mapInfo;

  MapLoader::loadMap(_appConfig->getConfig("map"), mapInfo);

  _model = new BaseGameModel(mapInfo);
}

void GameController::createGameView() {

  if (_view != NULL) {
    delete _view;
    _view = NULL;
  }

  _view = new BaseGameView(_tileManager, _model);
}