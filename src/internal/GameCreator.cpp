#include "GameCreator.h"

#include "GameController.h"
#include "ConcurrentGameController.h"

#ifdef DEBUG
#include <iostream>
#include <fstream>
#endif
IController* GameCreator::createGame(const char* configFile, 
                                      const char* mapFile) {

  // 1. load config
  AppConfig* config = new AppConfig(configFile);
  
  if (mapFile != NULL)
    config->setConfig("map", mapFile);

  GameController* controller;

  if (config->getConfig("turn_base") == "true")
    controller = new GameController();
  else
    controller = new ConcurrentGameController();

  controller->setConfig(config);

  // 2. load resource
  TileManager* tileManager = new TileManager(config);
  controller->setTileManager(tileManager);  
  

  // 3. load map and map id's, set up model
  controller->createGameModel();

  // 4. create view
  controller->createGameView();

  return controller;
}