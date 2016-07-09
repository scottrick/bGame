#ifndef UTILITY
#define UTILITY

#include <cstdlib>
#include <string>
using namespace std;

class Utility {
public:
	static float generateRandomFloat(float range);
	static int generateRandomInt(int range);
	static void renderBitmapString(float x, float y, float z, void *font, string theString);
};

#endif
