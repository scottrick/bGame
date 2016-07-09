//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#include "BoardLocation.h"

BoardLocation::BoardLocation() {
	x = 0;
	y = 0;
	passable = false;

	initialize();
}

BoardLocation::BoardLocation(int newX, int newY, bool newPassable) {
	x = newX;
	y = newY;
	passable = newPassable;

	initialize();
}

BoardLocation::~BoardLocation() {
	//cout << "boardocation destructor" << endl;
	delete color;
	delete one;
	delete two;
	delete three;
	delete four;

	for (unsigned int i = 0; i < effects.size(); i++) {
		delete effects.at(i);
	}

	if (dwDrawList == -1)
	{
		glDeleteLists(dwDrawList, 1);
	}
}

void BoardLocation::initialize() {
	monsterNum = -1;
	dwDrawList = -1;
	occupied = false;
	created = false;

	north = NULL;
	south = NULL;
	east = NULL;
	west = NULL;

	color = new SColor(0, 0, 0);
}

void BoardLocation::initializeTerrain() {
	float terrainMultiplier;

	if (type == MOUNTAINS) {
		terrainMultiplier = 2.0f;
	}
	else if (type == LAKE) {
		terrainMultiplier = 1.4f;
	}
	else if (type == PLAINS) {
		terrainMultiplier = 0.9f;
	}
	else if (type == GRASS) {
		terrainMultiplier = 0.4f;
	}
	else {
		//cout << "BAD TERRAIN TYPE   BoardLocation::initializeTerrain()" << endl;
	}

	//terrainMultiplier is multiplied by the z Coordinate to give rockier feel to mountains, flatter to grass, etc
	midpoint = new XVertex(Utility::generateRandomFloat(TERRAIN_VAR * 2.0f) - TERRAIN_VAR, Utility::generateRandomFloat(TERRAIN_VAR * 2.0f) - TERRAIN_VAR, (Utility::generateRandomFloat(TERRAIN_VAR * 2.0f) - TERRAIN_VAR) * terrainMultiplier);

	//cout << "midpoint!  " << midpoint.getX() << " " << midpoint.getY() << " " << midpoint.getZ() << endl;

	one = new XTriangle(new XVertex(-1.0f, -1.0f, 0.1f), new XVertex(-1.0f, 1.0f, 0.1f), midpoint->duplicate());
	two = new XTriangle(new XVertex(-1.0f, 1.0f, 0.1f), new XVertex(1.0f, 1.0f, 0.1f), midpoint->duplicate());
	three = new XTriangle(new XVertex(1.0f, 1.0f, 0.1f), new XVertex(1.0f, -1.0f, 0.1f), midpoint->duplicate());
	four = new XTriangle(new XVertex(1.0f, -1.0f, 0.1f), new XVertex(-1.0f, -1.0f, 0.1f), midpoint->duplicate());

	delete midpoint;

	if (dwDrawList == -1)
	{ //create displaylist
		dwDrawList = glGenLists(1);
		glNewList(dwDrawList, GL_COMPILE);
		drawSlow();
		glEndList();
	}
}

int BoardLocation::getType() {
	return type;
}

void BoardLocation::setType(int newType) {
	type = newType;
}

BoardLocation* BoardLocation::getNorth() {
	return north;
}

BoardLocation* BoardLocation::getSouth() {
	return south;
}

BoardLocation* BoardLocation::getEast() {
	return east;
}

BoardLocation* BoardLocation::getWest() {
	return west;
}

BoardLocation* BoardLocation::getDirection(int direction) {
	if (direction == NORTH) {
		return north;
	}

	if (direction == SOUTH) {
		return south;
	}

	if (direction == EAST) {
		return east;
	}

	if (direction == WEST) {
		return west;
	}

	//bad direction
	//cout << "BoardLocation::getDirection(int direction) given BAD DIRECTION." << endl;

	return 0;
}

void BoardLocation::setNorth(BoardLocation* newNorth) {
	north = newNorth;
}

void BoardLocation::setSouth(BoardLocation* newSouth) {
	south = newSouth;
}

void BoardLocation::setEast(BoardLocation* newEast) {
	east = newEast;
}

void BoardLocation::setWest(BoardLocation* newWest) {
	west = newWest;
}

bool BoardLocation::getPassable() {
	return passable;
}

void BoardLocation::setPassable(bool newPassable) {
	passable = newPassable;
}

void BoardLocation::print() {
	//cout << "location (" << x << ", " << y << ") with neighbors N=" << north << " S=" << south << " E=" << east << " W=" << west << endl;
}

SColor* BoardLocation::getColor() {
	return color;
}

bool BoardLocation::getCreated() {
	return created;
}

void BoardLocation::setCreated() {
	created = true;
}

void BoardLocation::setColor(SColor* newColor) {
	if (color != NULL) {
		delete color;
	}

	color = newColor;
}

void BoardLocation::draw() {

	if (dwDrawList == -1)
	{
		drawSlow();
	}
	else
	{
		glCallList(dwDrawList);
	}

	//draw any effects on this location
	for (unsigned int i = 0; i < effects.size(); i++) {
		effects.at(i)->draw();
	}
}

void BoardLocation::drawSlow()
{
	glColor3f(color->getR(), color->getG(), color->getB());
	glBegin(GL_TRIANGLES);

	//draw triangle one
	glNormal3f(one->getNormal().getX(), one->getNormal().getY(), one->getNormal().getZ());
	glVertex3f(one->getVertexOne()->getX(), one->getVertexOne()->getY(), one->getVertexOne()->getZ());
	glVertex3f(one->getVertexTwo()->getX(), one->getVertexTwo()->getY(), one->getVertexTwo()->getZ());
	glVertex3f(one->getVertexThree()->getX(), one->getVertexThree()->getY(), one->getVertexThree()->getZ());

	//draw triangle two
	glNormal3f(two->getNormal().getX(), two->getNormal().getY(), two->getNormal().getZ());
	glVertex3f(two->getVertexOne()->getX(), two->getVertexOne()->getY(), two->getVertexOne()->getZ());
	glVertex3f(two->getVertexTwo()->getX(), two->getVertexTwo()->getY(), two->getVertexTwo()->getZ());
	glVertex3f(two->getVertexThree()->getX(), two->getVertexThree()->getY(), two->getVertexThree()->getZ());

	//draw triangle three
	glNormal3f(three->getNormal().getX(), three->getNormal().getY(), three->getNormal().getZ());
	glVertex3f(three->getVertexOne()->getX(), three->getVertexOne()->getY(), three->getVertexOne()->getZ());
	glVertex3f(three->getVertexTwo()->getX(), three->getVertexTwo()->getY(), three->getVertexTwo()->getZ());
	glVertex3f(three->getVertexThree()->getX(), three->getVertexThree()->getY(), three->getVertexThree()->getZ());

	//draw triangle foue
	glNormal3f(four->getNormal().getX(), four->getNormal().getY(), four->getNormal().getZ());
	glVertex3f(four->getVertexOne()->getX(), four->getVertexOne()->getY(), four->getVertexOne()->getZ());
	glVertex3f(four->getVertexTwo()->getX(), four->getVertexTwo()->getY(), four->getVertexTwo()->getZ());
	glVertex3f(four->getVertexThree()->getX(), four->getVertexThree()->getY(), four->getVertexThree()->getZ());

	glEnd();
}

void BoardLocation::setEdge(bool newEdge) {
	edge = newEdge;
}

bool BoardLocation::getEdge() {
	return edge;
}

int BoardLocation::getX() {
	return x;
}

int BoardLocation::getY() {
	return y;
}

int BoardLocation::getMonsterNum() {
	return monsterNum;
}

void BoardLocation::setMonsterNum(int newMonsterNum) {
	monsterNum = newMonsterNum;
}

bool BoardLocation::getOccupied() {
	return occupied;
}

void BoardLocation::setOccupied(bool newOccupied) {
	occupied = newOccupied;
}

bool BoardLocation::getAvailable() {
	if ((!occupied) && passable) {
		return true;
	}
	else {
		return false;
	}
}

void BoardLocation::frameUpdate(float deltaTime) {
	for (unsigned int i = 0; i < effects.size(); i++) {
		effects.at(i)->frameUpdate(deltaTime);

		if (effects.at(i)->getExpired()) {
			delete effects.at(i);
			effects.erase(effects.begin() + i);

			//decrement since one item was removed from the list
			i--;
		}
	}
}

void BoardLocation::addEffect(Effect* newEffect) {
	effects.push_back(newEffect);
}

int BoardLocation::getEffectsSize() {
	return (int)effects.size();
}