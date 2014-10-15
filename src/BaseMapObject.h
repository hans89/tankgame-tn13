#ifndef __TANKGAME_BASEOBJECT__
#define __TANKGAME_BASEOBJECT__ 

#include "IMapObject.h"

class BaseMapObject : virtual public IMapObject {
public:
  #pragma region IMapObjectImplementation
  
  int getHP() const;  
  std::pair<int, int> getPosition() const;

  // return if the Object in on map
  bool isOnMap() const;
  #pragma endregion

  #pragma region YetToDefineInterface
  // return the char ID of this object on the map
  virtual char getMapID() const = 0;

  #pragma endregion

  #pragma region ModelPreservedInterfaces
  void decreaseHP(int amount = 1);
  void kill();
  void move(const std::pair<int, int>& newPost);
  void removeFromMap();

  BaseMapObject(int HP, const std::pair<int,int>& pos);
  #pragma endregion

protected:
  int _HP;
  std::pair<int, int> _pos;
};
#endif