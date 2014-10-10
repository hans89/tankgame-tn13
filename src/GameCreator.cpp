#include "GameCreator.h"

#include "GameController.h"

IController* GameCreator::createGame(const char* configFile) {
  GameController* controller = new GameController();

  // 1. load config
  AppConfig* config = new AppConfig(configFile);
  controller->setConfig(config);

  // 2. load resource
  TileManager* tileManager = new TileManager(configFile);
  controller->setTileManager(tileManager);

  // 3. load map and map id's


  // 4. set up model

  return controller;
}