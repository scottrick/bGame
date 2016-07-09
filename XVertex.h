//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#ifndef XVERTEX
#define XVERTEX

#include "XVector.h"
#include <vector>
#include <GL/glut.h>
using namespace std;

class XVertex {
public:
	XVertex();
	XVertex(float xx, float yy, float zz);
	~XVertex();

	void setAll(float xx, float yy, float zz);
	float getX();
	float getY();
	float getZ();

	XVertex* duplicate();

	void changeXBy(float changeX);
	void changeYBy(float changeY);
	void changeZBy(float changeZ);

private:
	float x, y, z;
};

#endif