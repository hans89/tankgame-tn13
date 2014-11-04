// #include "player1.h"
// #include "player2.h"

#include "include/CImg.h"
#include "internal/GameCreator.h"
#include "internal/IController.h"
#include "BasePlayer.h"
// #include "players/Nhom01/Nhom01Player.h"
#include "players/Nhom02/Nhom02Player.h"
#include "players/Nhom03/Nhom03Player.h"
#include "players/Nhom04/Nhom04Player.h"
#include "players/Nhom05/Nhom05Player.h"
#include "players/Nhom06/Nhom06Player.h"
// #include "players/Nhom07/Nhom07Player.h"
#include "players/Nhom08/Nhom08Player.h"
#include "players/Nhom09/Nhom09Player.h"
#include "players/Nhom10/Nhom10Player.h"
// #include "players/Nhom11/Nhom11Player.h"
#include "players/Nhom12/Nhom12Player.h"
#include "players/Nhom13/Nhom13Player.h"
#include "players/Nhom15/Nhom15Player.h"

#include <cstdlib>
#include <ctime>

using namespace cimg_library;
#include <iostream>

int main(int argc, char* argv[]) {
  std::srand(std::time(NULL));

  // main game flow
  // 1. create game controller
  IController* gameController;

  if (argc >= 4)
    gameController = GameCreator::createGame("app.config", argv[3]);
  else
    gameController = GameCreator::createGame("app.config");
  
  // 2. set up display
  CImgDisplay main_disp;
  main_disp.set_title("Run");

  gameController->setDisplay(&main_disp);

  // 3. load players and bind with controller
  std::vector<IPlayer*> players(16, NULL);
  // players[1] = new Nhom01::Nhom01Player();
  players[2] = new Nhom02::Nhom02Player();
  players[3] = new Nhom03::Nhom03Player();
  players[4] = new Nhom04::Nhom04Player();
  players[5] = new Nhom05::Nhom05Player();
  players[6] = new Nhom06::Nhom06Player();
  //players[7] = new Nhom07::Nhom07Player();
  // players[8] = new Nhom08::Nhom08Player();
  players[9] = new Nhom09::Nhom09Player();
  players[10] = new Nhom10::Nhom10Player();
  //players[11] = new Nhom11::Nhom11Player();
  players[12] = new Nhom12::Nhom12Player();
  players[13] = new Nhom13::Nhom13Player();
  // players[14] = new Nhom14::Nhom14Player();
  players[15] = new Nhom15::Nhom15Player();

  int player1Idx = atoi(argv[1]),
      player2Idx = atoi(argv[2]);

  if (player1Idx <= 0 || player1Idx > players.size()
      || players[player1Idx] == NULL
      || player2Idx <= 0 || player2Idx > players.size()
      || players[player2Idx] == NULL)
    return 0;

  gameController->registerPlayer(players[player1Idx]);
  gameController->registerPlayer(players[player2Idx]);
  
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
      gameController->updateDisplay();
    }
    
    if (gameController->isEnding() || gameController->isEnded()) {
      if (!gameController->isEnded()) {
        // if ending:
        // inform players" onFinish
        gameController->finish();
        // and report result
        // TODO: report result
        gameController->reportResult();
        gameController->updateDisplay();
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
  for (int i = 0 ; i < players.size(); i++)
    if (players[i] != NULL)
      delete players[i];

  return 0;
}