#ifndef __TANKGAME_IMAP__
#define __TANKGAME_IMAP__

class IMap {
protected:
	virtual ~IMap() {}
public:
  const static pair<int, int> nopos;
  // return the width (x-coordinate) of this map
	virtual int getWidth() const = 0;

  // return the height (y-coordinate) of this map
	virtual int getHeight() const = 0;

  // return the cell value as char at (x,y)
  // (0,0) is at the left-top corner
	virtual char operator()(int x, int y) const = 0;

  // return if the cell is empty and can be occupied
  virtual bool isWalkable(int x, int y) const = 0;
};

const pair<int, int> IMap::nopos = pair<int,int>(-1,-1);
#endif