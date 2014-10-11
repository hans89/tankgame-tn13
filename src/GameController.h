#ifndef __TANKGAME_GAMECONTROLLER__
#define __TANKGAME_GAMECONTROLLER__

using namespace std;
#include "IController.h"
#include "Command.h"
#include "MapLoader.h"
#include "BaseGameModel.h"
#include "BaseGameView.h"

class GameController : public IController {
private:
  BaseGameView* _view;
  BaseGameModel* _model;
  
  vector<IPlayer*> _players;
  int _currentPlayerTurn;

  #pragma region IController Data
  AppConfig* _appConfig;
  TileManager* _tileManager;

  bool _autoMode;
  bool _ended;
  bool _ending;

  #pragma endregion

protected:
  // prevent copy
  GameController(const GameController& g);
  GameController& operator=(const GameController& g);

public:
  GameController();
  #pragma region IControllerImplementation
  void setDisplay(CImg<unsigned char>* image, CImgDisplay* display);
  bool registerPlayer(IPlayer* player);

  int getMapWidth() const;
  int getMapHeight() const;
  string getConfig(string key) const;

  bool start();
  bool finish();
  void toggleMode();
  bool isInAutoMode() const;
  bool isEnding() const;
  bool isEnded() const;
  bool nextTurn();
  void updateDisplay();

  ~GameController();
  #pragma endregion

  #pragma region InternalPreservedInterfaces
  void setConfig(AppConfig* config);
  void setTileManager(TileManager* tileManager);

  void createGameModel();
  void createGameView();
};
#endif