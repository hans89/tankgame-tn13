#include "player1.h"
#include "player2.h"

#include "include/CImg.h"
#include "GameCreator.h"
#include "IController.h"

using namespace cimg_library;

int main2(void) {
  // main game flow
  // 2. create game controller
  IController* gameController = GameCreator::createGame("app.config");

  // 3. load players and bind with controller
  IPlayer* player1 =  new Player1();
  IPlayer* player2 =  new Player2();

  gameController->registerPlayer(player1);
  gameController->registerPlayer(player2);

  // 4. set up display
  CImg<unsigned char> image(gameController->getDisplayWidth(), 
      gameController->getDisplayHeight(), 1, 3, 0);

  CImgDisplay main_disp(image, "Run");

  gameController->setDisplayImage(&image);
  gameController->setDisplay(&main_disp);
  // 5. start game
  // 5a. inform players' onStart
  gameController->start();

  int waitTime = Utils::parseInt(config.getConfig("delay"));

  // main loop
  while (!main_disp.is_closed()) {
    // check if "r" key is clicked: toggle auto-step and manual-step mode
    if (main_disp.key(cimg::keyR)) {
      gameController->toggleMode();
    }
    
    if (gameController->isEnding() || gameController->isEnded()) {
      if (!gameController->isEnded()) {
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
      gameController->updateDisplay();
      main_disp.wait(waitTime);
    } else {
      // manual-step mode
      // in manual-step mode, the controller wait for the key event "SPACE" to 
      // asks the next player to move
      if (main_disp.key(cimg::keySPACE)) {
        gameController->nextTurn();
        gameController->updateDisplay();
      }
    }
  }

  delete gameController;
  delete player1;
  delete player2;
}