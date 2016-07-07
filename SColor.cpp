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

#include "SColor.h"

SColor:: SColor() {
	r = Utility::generateRandomFloat(1.0f);
	g = Utility::generateRandomFloat(1.0f);
	b = Utility::generateRandomFloat(1.0f);
}

SColor::SColor(float rr, float gg, float bb) {
	
	if (rr < 0) {
		rr = 0;
	}
	if (gg < 0) {
		gg = 0;
	}
	if (bb < 0) {
		bb = 0;
	}

	r = rr;
	g = gg;
	b = bb;
}

SColor::SColor(int rr, int gg, int bb) {
	if (rr < 0) {
		rr = 0;
	}
	if (gg < 0) {
		gg = 0;
	}
	if (bb < 0) {
		bb = 0;
	}
	if (rr > 255) {
		rr = 255;
	}
	if (gg > 255) {
		gg = 255;
	}
	if (bb > 255) {
		bb = 255;
	}

	r = (float)rr / 255.0f;
	g = (float)gg / 255.0f;
	b = (float)bb / 255.0f;
}

SColor::~SColor() {
}

void SColor::print() {
	cout << "SColor " << this << ": " << r << " " << g << " " << b << endl;
}	

int SColor::getIntB() {
	float bb = b * 255;
	return (int)bb;
}

int SColor::getIntG() {
	float gg = g * 255;
	return (int)gg;
}

int SColor::getIntR() {
	float rr = r * 255;
	return (int)rr;
}

float SColor::getB() {
	return b;
}

float SColor::getG() {
	return g;
}

float SColor::getR() {
	return r;
}

SColor* SColor::multiplyBy(float x) {
	float rr = r * x;
	float gg = g * x;
	float bb = b * x;
	return new SColor(rr, gg, bb);
}	

SColor* SColor::addTo(SColor* addThis) {
	float newR = r + addThis->getR();
	float newG = g + addThis->getG();
	float newB = b + addThis->getB();
	return new SColor(newR, newG, newB);
}

void SColor::normalize(float max) {
	r = r / max;
	g = g / max;
	b = b / max;
}

SColor* SColor::generateSimilarColor() {
	float rrr = r + Utility::generateRandomFloat(COLOR_VARIATION) - 0.5f * COLOR_VARIATION;
	float ggg = g + Utility::generateRandomFloat(COLOR_VARIATION) - 0.5f * COLOR_VARIATION;
	float bbb = b + Utility::generateRandomFloat(COLOR_VARIATION) - 0.5f * COLOR_VARIATION;

	return new SColor(rrr, ggg, bbb);
}

SColor* SColor::generateSameColor() {
	return new SColor(r, g, b);
}