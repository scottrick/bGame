#include "MessageList.h"

MessageList::MessageList() {
	num = 10; //default list size
}	

MessageList::MessageList(int newNum) {
	num = newNum;
}

MessageList::~MessageList() {
	//Message* current;
	
	for (unsigned int i = 0; i < theMessages.size(); i++) {
		delete theMessages.at(i);
	}
}

vector<Message*> MessageList::getMessages() {
	return theMessages;
}

void MessageList::addMessage(Message *newMessage) {
	//add new Message to the back of the list
	//if the list is full, pop the front message until it is not full anymore

	theMessages.push_back(newMessage);
		
	while ((int)theMessages.size() > num) {
		delete theMessages.front();
		theMessages.erase(theMessages.begin());
	}
}

void MessageList::print() {
	
}

int MessageList::getSize() {
	return (int)theMessages.size();
}

Message* MessageList::getMessage(int index) {
	return theMessages.at(index);
}