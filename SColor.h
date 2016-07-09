//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#ifndef SCOLOR
#define SCOLOR

#include "Utility.h"
#include "def.h"
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

	void setR(float newR);
	void setG(float newG);
	void setB(float newB);
	void setAll(float newAll);

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