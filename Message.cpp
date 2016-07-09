//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#include "Message.h"
#include <GL/glut.h>

Message::Message() {
	msg = "";
	dwDrawList = -1;
	color = NULL;
}

Message::Message(string newMsg, SColor *newColor) {
	msg = newMsg;
	dwDrawList = -1;
	color = newColor;
}

Message::~Message() {
	delete color;
	if (dwDrawList != -1)
	{ //destroy display list!
		glDeleteLists(dwDrawList, 1);
	}
}

string Message::getMessage() {
	return msg;
}

SColor* Message::getColor() {
	return color;
}

string Message::getCharMessage() {
	return msg;
}

void Message::renderBitmapString(float x, float y, float z, void *font) {
	glRasterPos3f(x, y, z);

	if (dwDrawList == -1)
	{ //make a display list if it doesn't exist
		dwDrawList = glGenLists(1);
		glNewList(dwDrawList, GL_COMPILE);

		char line[200];
		strcpy_s(line, msg.substr(0, 199).c_str());
		char *c;
		for (c = line; *c != '\0'; c++) {
			glutBitmapCharacter(font, *c);
		}

		glEndList();
	}
	else
	{ //use display list!
		glCallList(dwDrawList);
	}
}