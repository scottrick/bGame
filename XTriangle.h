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

#ifndef XTRIANGLE
#define XTRIANGLE

#include "XVertex.h"
#include "XVector.h"
#include <cmath>
#include <GL/glut.h>

class XTriangle {
public:
	XTriangle();
	XTriangle(XVertex* one, XVertex* two, XVertex* three);
	~XTriangle();

	void setAll(XVertex* one, XVertex* two, XVertex* three);
	XVertex* getVertexOne();
	XVertex* getVertexTwo();
	XVertex* getVertexThree();
	
	XVector getNormal();
	XVector calculateNormal(XVertex* vertexOne, XVertex* vertexTwo, XVertex* vertexThree);
private:
	void calculateNormal();

	XVertex *vertexOne, *vertexTwo, *vertexThree;
	XVector normalVector;
};

#endif