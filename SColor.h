//***********************************************************
//
// Scott Atkins
// ID # 2728123
// 
// CSCI 5107
// Assignment Four, Part One, Two, and Three
// 
// December 9th, 2005
// 
//***********************************************************

#ifndef SCOLOR
#define SCOLOR

#include "Utility.h"
#include "def.h"
#include <iostream>
using namespace std;

class SColor {
public:
	SColor();
	SColor(float rr, float gg, float bb);
	SColor(int rr, int gg, int bb);
	~SColor();

	float getR();
	float getG();
	float getB();

	int getIntR();
	int getIntG();
	int getIntB();

	SColor* generateSameColor();
	SColor* generateSimilarColor();

	void normalize(float max);

	void print();
	SColor* multiplyBy(float x);
	SColor* addTo(SColor* addThis);
private:
	float r, g, b;
};

#endif