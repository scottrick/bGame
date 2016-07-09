#include "Utility.h"
#include <GL/glut.h>

float Utility::generateRandomFloat(float range) {
	//return (range * ((float)rand() / (float)RAND_MAX));
	return (float)rand() / (float(RAND_MAX + 1)) * range;
}	

int Utility::generateRandomInt(int range) {
	//returns random int from 0 to range
	return rand() % (range + 1);
}

void Utility::renderBitmapString(float x, float y, float z, void *font, string theString) {
	char line[200];
	strcpy_s(line, theString.substr(0, 199).c_str());

	char *c;
	glRasterPos3f(x, y, z);

	for (c = line; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}