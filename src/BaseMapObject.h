#ifndef __TANKGAME_BASEOBJECT__
#define __TANKGAME_BASEOBJECT__ 

#include "IMapObject.h"

class BaseMapObject : virtual public IMapObject {
protected:
  int _HP;
  pair<int, int> _pos;
public:
  #pragma region IMapObjectImplementation
  
  int getHP() const {
    return _HP;
  }
  
  pair<int, int> getPosition() const {
    return _pos;
  }

  // return if the Object in on map
  bool isOnMap() const {
    return _pos == IMap::npos;
  }
  #pragma endregion

  #pragma region YetToDefineInterface

  // return the char ID of this object on the map
  virtual char getMapID() const = 0;

  #pragma endregion

  #pragma region ModelPreservedInterfaces
  void decreaseHP(int amount = 1) {
    _HP -= amount;

    if (_HP <= 0) {
      kill();
    }
  }

  void kill() {
    _HP = 0;
    removeFromMap();
  }

  void move(const pair<int, int>& newPost) {
    _pos = newPost;
  }

  void removeFromMap() {
    _pos = IMap::npos;
  }

  BaseMapObject(int HP, const pair<int,int>& pos) : _HP(HP), _pos(pos) {}
  #pragma endregion
};
#endif