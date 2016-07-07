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
private:
	string msg;
	SColor* color;
};

#endif