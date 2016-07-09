//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#include "XVertex.h"


XVertex::XVertex() {
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

XVertex::XVertex(float xx, float yy, float zz) {
	x = xx;
	y = yy;
	z = zz;
}

XVertex::~XVertex() {
	//cout << "something was deleted...\n";
	//cout << "XVertex destructor  " << x << " " << y << " " << z << endl;
}

float XVertex::getX() {
	return x;
}

float XVertex::getY() {
	return y;
}

float XVertex::getZ() {
	return z;
}

void XVertex::changeXBy(float changeX) {
	x += changeX;
}

void XVertex::changeYBy(float changeY) {
	y += changeY;
}

void XVertex::changeZBy(float changeZ) {
	z += changeZ;
}

void XVertex::setAll(float xx, float yy, float zz) {
	x = xx;
	y = yy;
	z = zz;
}

XVertex* XVertex::duplicate() {
	return new XVertex(x, y, z);
}