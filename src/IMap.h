#ifndef __TANKGAME_IMAP__
#define __TANKGAME_IMAP__

#include <utility>

class IMap {
protected:
	virtual ~IMap() {}
public:
  const static std::pair<int, int> npos;
  
  // return the width (x-coordinate) of this map
	virtual int getWidth() const = 0;

  // return the height (y-coordinate) of this map
	virtual int getHeight() const = 0;

  // return the cell value as char at (x,y)
  // (0,0) is at the left-top corner
	virtual char operator()(int x, int y) const = 0;

  // return if the cell is on land, empty and can be occupied
  virtual bool isEmptySpace(int x, int y) const = 0;
};

#endif