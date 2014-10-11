#include "BasePlayer.h"

BasePlayer::BasePlayer() : _playerInfo(NULL), _gameInfo(NULL) {}

BasePlayer::~BasePlayer() {}

void BasePlayer::onBindPlayerInfo(IPlayerInfo* playerInfo) {
  _playerInfo = playerInfo;
}
void BasePlayer::onBindGameInfo(IGameInfo* gameInfo) {
  _gameInfo = gameInfo;
}

IPlayerInfo* BasePlayer::getPlayerInfo() const {
  return _playerInfo;
}

bool BasePlayer::onStart() {
  return true;
}

bool BasePlayer::onFinish(){
  return true;
}

Command BasePlayer::nextMove() {
  return Command();
}