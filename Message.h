//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#ifndef MESSAGE
#define MESSAGE

#include "SColor.h"
#include <string>
using namespace std;

class Message {
public:
	Message();
	Message(string newMsg, SColor* newColor);
	~Message();

	string getMessage();
	SColor* getColor();

	string getCharMessage();
	void renderBitmapString(float x, float y, float z, void *font);

private:
	string msg;
	SColor* color;
	int dwDrawList;
};

#endif