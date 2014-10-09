#include "GameCreator.h"

#include "GameController.h"

IController* GameCreator::createGame(const char* configFile) {
  GameController* controller = new GameController();

  AppConfig* config = new AppConfig(configFile);
  controller->setConfig(config);

  TileManager* tileManager = new TileManager

  // 1. load tiles
  

  return controller;
}