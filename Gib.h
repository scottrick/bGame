#ifndef GIB
#define GIB

#include "SColor.h"
//#include <iostream>
#include <GL/glut.h>
using namespace std;

class Gib {
public:
	Gib();
	Gib(SColor* newColor);

	~Gib();

	void setVelocityX(float newVelocityX);
	void setVelocityY(float newVelocityY);
	void setVelocityZ(float newVelocityZ);

	float getVelocityX();
	//void setAccelerateZ(float newAccelerateZ);

	void setScale(float newScale);

	bool getExpired();

	void frameUpdate();

	void draw();

	void print();
private:
	void initialize();

	float x, y, z, velocityX, velocityY, velocityZ, scale;

	//rotation info
	float rotationX, rotationY, rotationZ, degrees, currentDegrees;

	SColor* color;
};

#endif
