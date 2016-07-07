#include "Message.h"

Message::Message() {
	msg = "";
	color = NULL;
}

Message::Message(string newMsg, SColor *newColor) {
	msg = newMsg;
	color = newColor;
}

Message::~Message() {
	delete color;
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