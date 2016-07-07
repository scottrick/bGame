//***********************************************************
//
// Scott Atkins
// ID # 2728123
// 
// CSCI 5108
// Project
// 
// April 24th, 2006
// 
//***********************************************************

#ifndef XVERTEX
#define XVERTEX

#include "XVector.h"
#include <vector>
#include <iostream>
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

	XVector getNormal();
	void setNormal(XVector newNormal);

	XVertex* duplicate();

	void changeXBy(float changeX);
	void changeYBy(float changeY);
	void changeZBy(float changeZ);

	void draw();
	
private:
	float x, y, z;
};

#endif