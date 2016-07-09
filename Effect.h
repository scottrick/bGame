//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#ifndef EFFECT
#define EFFECT

#include "Utility.h"
#include "SColor.h"
#include "Gib.h"
#include <GL/glut.h>
#include <cmath>
#include <vector>
using namespace std;

class Effect {
public:
	Effect();
	Effect(int newEffectType, int newIntensity, SColor* newColor);

	~Effect();

	void frameUpdate(float deltaTime);

	void draw();

	bool getExpired();
private:
	void initialize();

	SColor* color;

	vector<Gib*> gibs;

	int intensity;
	int effectType;
};

#endif