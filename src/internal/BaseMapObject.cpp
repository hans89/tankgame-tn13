#include "BaseMapObject.h"
#include "../IMap.h"

#pragma region IMapObjectImplementation
int BaseMapObject::getHP() const {
  return _HP;
}

std::pair<int, int> BaseMapObject::getPosition() const {
  return _pos;
}

// return if the Object in on map
bool BaseMapObject::isOnMap() const {
  return _pos != IMap::npos;
}
#pragma endregion

#pragma region ModelPreservedInterfaces
void BaseMapObject::decreaseHP(int amount) {
  _HP -= amount;

  if (_HP <= 0) {
    kill();
  }
}

void BaseMapObject::kill() {
  _HP = 0;
}

void BaseMapObject::move(const std::pair<int, int>& newPost) {
  _pos = newPost;
}

void BaseMapObject::removeFromMap() {
  _pos = IMap::npos;
}

BaseMapObject::BaseMapObject(int HP, const std::pair<int,int>& pos)
    : _HP(HP), _pos(pos) {}
#pragma endregion