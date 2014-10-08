#include "player1.h"
#include "player2.h"

int main2(void) {
  // main game flow
  // 1. load configuration
  AppConfig::loadConfig("app.config");

  TileManager manager(AppConfig::getConfig("tile"));

  // 2. load map
  IMap* gameMap = MapLoader::loadMap(AppConfig::getConfig("map").c_str());

  // 3. create game controller
  IController* gameController = GameCreator::createController(gameMap);

  // 4. load players and bind with controller
  IPlayer* player1 = GameCreator::createPlayer(Player1());
  IPlayer* player2 = GameCreator::createPlayer(Player1());

  gameController->registerPlayer(player1);
  gameController->registerPlayer(player2);

  // 5. start game
  // 5a. inform players' onStart
  gameController->start();

  int waitTime = Utils::parseInt(AppConfig::getConfig("delay"));

  // main loop
  while (!main_disp.is_closed()) {
    // check if "r" key is clicked: toggle auto-step and manual-step mode
    if (main_disp.key(cimg::keyR)) {
      gameController->toggleMode();
    }
    
    if (gameController->isEnding() || gameController->isEnded()) {
      if (gameController->isEnded() == false) {
        // if ending:
        // inform players" onFinish
        gameController->finish();
        // and report result
      }
    } else if (gameController->isInAutoMode()) {
    // check if auto-step or manual-step
      // auto step mode
      // in auto-step mode, the controller asks each player in turn to move,
      // wait for DELAY time, then asks the next player, until end game
      gameController->nextTurn();
      gameController->updateDisplay(main_disp);
      main_disp.wait(waitTime);
    } else {
      // manual-step mode
      // in manual-step mode, the controller wait for the key event "SPACE" to 
      // asks the next player to move
      if (main_disp.key(cimg::keySPACE)) {
        gameController->nextTurn();
        gameController->updateDisplay(main_disp);
      }
    }
  }
}