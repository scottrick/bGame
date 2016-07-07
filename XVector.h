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

#ifndef XVECTOR
#define XVECTOR

//#include <iostream>
//using namespace std;

class XVector {
public:
	XVector();
	XVector(float xx, float yy, float zz);
	~XVector();

	float getX();
	float getY();
	float getZ();

	void setX(float xx);
	void setY(float yy);
	void setZ(float zz);

	XVector crossProduct(XVector crossThis);
private:
	float x, y, z;
};

#endif