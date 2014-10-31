// #include "player1.h"
// #include "player2.h"

#include "include/CImg.h"
#include "internal/GameCreator.h"
#include "internal/IController.h"
#include "BasePlayer.h"

#include <cstdlib>
#include <ctime>

using namespace cimg_library;
#include <iostream>

int main(void) {
  std::srand(std::time(NULL));
  // main game flow
  // 1. create game controller
  IController* gameController = GameCreator::createGame("app.config");
  
  // 2. set up display
  CImgDisplay main_disp;
  main_disp.set_title("Run");

  gameController->setDisplay(&main_disp);

  // 3. load players and bind with controller
  BasePlayer* player1 =  new BasePlayer();
  BasePlayer* player2 =  new BasePlayer();

  gameController->registerPlayer(player1);
  gameController->registerPlayer(player2);
  
  // 4. start game
  // 4a. inform players' onStart
  gameController->start();

  int waitTime = Utils::parseInt(gameController->getConfig("delay"));

  // main loop
  while (!main_disp.is_closed()) {

    const unsigned int keyPressed = main_disp.key();
    // check if "r" key is clicked: toggle auto-step and manual-step mode
    if (keyPressed == cimg::keyR) {
      // flush all keys
      main_disp.set_key();
      gameController->toggleMode();
    }
    
    if (gameController->isEnding() || gameController->isEnded()) {
      if (!gameController->isEnded()) {
        // if ending:
        // inform players" onFinish
        gameController->finish();
        // and report result
        // TODO: report result
      }
    } else if (gameController->isInAutoMode()) {
    // check if auto-step or manual-step
      // auto step mode
      // in auto-step mode, the controller asks each player in turn to move,
      // wait for DELAY time, then asks the next player, until end game
      gameController->nextTurn();
      gameController->updateDisplay();
      main_disp.wait(waitTime);
    } else {
      // manual-step mode
      // in manual-step mode, the controller wait for the key event "SPACE" to 
      // asks the next player to move
      if (keyPressed == cimg::keySPACE) {
        // flush all keys
        main_disp.set_key();
        gameController->nextTurn();
        gameController->updateDisplay();
      }
    }
  }

  delete gameController;
  delete player1;
  delete player2;
}