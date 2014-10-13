#include "BasePlayer.h"
#include <list>
#include <cstdlib>

using namespace std;

BasePlayer::BasePlayer() 
  : _playerInfo(NULL), _gameInfo(NULL) {}

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
  list<ITank*> alives = _playerInfo->getAliveTanks();
  int numOfTank = alives.size();

  if (numOfTank > 0) {
    int next = rand() % numOfTank;

    list<ITank*>::iterator it;
    int j;
    for (it = alives.begin(), j = 0; it != alives.end() && j < next; ++it, ++j)
      ;

    ITank* nextTank = *it;

    int choice = rand() % 2; // 0: move, 1: fire
    int direction = rand() % 4; //0: north, 1: south, 2: east, 3: west


    Command::Action actchoices[2] = {Command::MOVE, Command::FIRE};
    pair<int,int> delta[4] = {
      pair<int,int>(0,-1), // north
      pair<int,int>(0,1), // south
      pair<int,int>(1,0), // east
      pair<int,int>(-1,0) // west
    };

    pair<int,int> curPos = nextTank->getPosition();
    
    return Command(nextTank, actchoices[choice], 
          curPos.first + delta[direction].first,
          curPos.second + delta[direction].second);
  }

  return Command();
}