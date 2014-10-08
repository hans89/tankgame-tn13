#ifndef __TANKGAME_IPLAYER__
#define __TANKGAME_IPLAYER__

class IPlayerInfo;
class IGameInfo;

#include "Command.h"
/**
 * Interface to implement a player's strategy
 * The game controller will request a player's move every turn of the game
 * through this interface.
 */
class IPlayer
{
protected:
  // prevent deletion through this interface
  virtual ~IPlayer() {}
public:
  // The framework provides the info structure that the player will control
  // Player must save this info for further reference to the assets
  // Player must implement this method
  virtual void onBindPlayerInfo(IPlayerInfo* playerInfo) = 0;

  // The framework provides the info structure of the whole game
  // Player must save this info for further reference to the game board
  // Player must implement this method
  virtual void onBindGameInfo(IGameInfo* gameInfo) = 0;

  // Provide the info of the current user
  // Player must implement this method
  virtual IPlayerInfo* getPlayerInfo() const = 0;

  // onStart will be called by the framework before the game begins
  // for the player to set up information.
  // Player must implement this method
  virtual bool onStart() = 0;

  // onFinish will be called by the framework after the game ends
  // for the player to clean up.
  // Player must implement this method
  virtual bool onFinish() = 0;

  // onNextMove will be called by the framework on the player's turn
  // The player must describe the next move in the Command structure
  // The framework will check if the action is valid (see the Command for more info).
  // If the command is invalid, the framework will consider that the player
  // skip this turn.
  // Player must implement this method
  virtual Command nextMove() = 0;
};

#endif