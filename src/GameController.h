#ifndef __TANKGAME_GAMECONTROLLER__
#define __TANKGAME_GAMECONTROLLER__

#include "IController.h"
#include "Command.h"
#include "MapLoader.h"
#include "BaseGameModel.h"
#include "BaseGameView.h"

class GameController : public IController {
protected:
  // prevent copy
  GameController(const GameController& g);
  GameController& operator=(const GameController& g);

public:
  GameController();
  #pragma region IControllerImplementation
  void setDisplay(CImgDisplay* display);
  bool registerPlayer(IPlayer* player);

  std::string getConfig(std::string key) const;

  bool start();
  bool finish();
  void toggleMode();
  bool isInAutoMode() const;
  bool isEnding() const;
  bool isEnded() const;
  bool nextTurn();
  void updateDisplay();

  virtual ~GameController();
  #pragma endregion

  #pragma region InternalPreservedInterfaces
  void setConfig(AppConfig* config);
  void setTileManager(TileManager* tileManager);

  void createGameModel();
  void createGameView();

  void animateMove(const Command& move);

protected:
  BaseGameView* _view;
  BaseGameModel* _model;
  
  std::vector<IPlayer*> _players;
  int _currentPlayerTurn;

  #pragma region IController Data
  AppConfig* _appConfig;
  TileManager* _tileManager;
  int _delayTime;

  bool _autoMode;
  bool _ended;
  bool _ending;

  #pragma endregion
};
#endif