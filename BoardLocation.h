#ifndef BOARD_LOCATION
#define BOARD_LOCATION

#include "Effect.h"
#include "XTriangle.h"
#include "SColor.h"
#include <GL/glut.h>
//#include <iostream>
using namespace std;

class BoardLocation {
public:
	BoardLocation();
	BoardLocation(int newX, int newY, bool newPassable);
	~BoardLocation();

	BoardLocation* getNorth();
	BoardLocation* getSouth();
	BoardLocation* getEast();
	BoardLocation* getWest();

	BoardLocation* getDirection(int direction);

	void setNorth(BoardLocation* newNorth);
	void setSouth(BoardLocation* newSouth);
	void setEast(BoardLocation* newEast);
	void setWest(BoardLocation* newWest);

	bool getPassable();
	void setPassable(bool newPassable);

    bool getOccupied();
	void setOccupied(bool newOccupied);

	void setEdge(bool newEdge);
	bool getEdge();

	void setType(int newType);
	int getType();

	//returns TRUE if the location is passable as well as unoccupied
	bool getAvailable();

	SColor* getColor();
	void setColor(SColor* newColor);

	void print();
	void draw();

	bool getCreated();
	void setCreated();

	int getMonsterNum();
	void setMonsterNum(int newMonsterNum);

	int getX();
	int getY();

	void initializeTerrain();

	void frameUpdate();

	void addEffect(Effect* newEffect);
	int getEffectsSize();
private:
	int x, y, type;
	bool passable, edge, occupied;

	bool created;

	void initialize();
		
	XVertex* midpoint; //random center point which gives the location a feel
	XTriangle *one, *two, *three, *four; //four triangles that comprise the board location

	int monsterNum;
		
	SColor* color;

	vector<Effect*> effects;

	BoardLocation *north, *south, *east, *west;
};

#endif