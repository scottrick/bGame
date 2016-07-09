//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#ifndef MESSAGE_LIST
#define MESSAGE_LIST

#include "Message.h"
#include <vector>
using namespace std;

class MessageList {
public:
	MessageList();
	MessageList(int newNum);
	~MessageList();

	vector<Message*> getMessages();

	int getSize();

	void addMessage(Message *newMessage);

	Message* getMessage(int index);

	void print();
private:
	vector<Message*> theMessages;
	int num; //number of messages stored in the list
};

#endif

