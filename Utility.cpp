#include "Utility.h"

float Utility::generateRandomFloat(float range) {
	//return (range * ((float)rand() / (float)RAND_MAX));
	return (float)rand() / (float(RAND_MAX + 1)) * range;
}	

int Utility::generateRandomInt(int range) {
	//returns random int from 0 to range
	return rand() % (range + 1);
}
