//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#include "XVector.h"

XVector::XVector() {
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

XVector::XVector(float xx, float yy, float zz) {
	x = xx;
	y = yy;
	z = zz;
}

XVector::~XVector() {
	//cout << "SVector destructor" << endl;
}

float XVector::getX() {
	return x;
}

float XVector::getY() {
	return y;
}

float XVector::getZ() {
	return z;
}

void XVector::setX(float xx) {
	x = xx;
}

void XVector::setY(float yy) {
	y = yy;
}

void XVector::setZ(float zz) {
	z = zz;
}

XVector XVector::crossProduct(XVector crossThis) {
	float xx, yy, zz;

	xx = (y * crossThis.getZ()) - (z * crossThis.getY());
	yy = -((x * crossThis.getZ()) - (z * crossThis.getX()));
	zz = (x * crossThis.getY()) - (y * crossThis.getX());

	XVector returnThis = XVector(xx, yy, zz);

	return returnThis;
}