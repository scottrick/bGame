//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#include "Highscore.h"

/*
	NOTE:  Messages in MessageLists in a highscore object do NOT necessarily have an SColor assigned to them.
	It cane and will often be NULL
*/

//create an empty highscore;  used when reading a highscore from a file, where the values are set the the highscorelist
Highscore::Highscore() {

}

//	MessageList *playerList, equipOneList, equipTwoList, equipThreeList, equipFourList;
//  used to create a highscore entry directly from a player 
Highscore::Highscore(Player *thisPlayer) {
	playerList = thisPlayer->createHighscoreList();
	equipOneList = thisPlayer->getItem(MAINHAND)->createDisplayMsgList();
	equipTwoList = thisPlayer->getItem(OFFHAND)->createDisplayMsgList();
	equipThreeList = thisPlayer->getItem(CHEST)->createDisplayMsgList();
	equipFourList = thisPlayer->getItem(HELM)->createDisplayMsgList();

	dragonsSlain = thisPlayer->getDragonsSlain();
	killCount = thisPlayer->getKillCount();
	actions = thisPlayer->getActions();
}

Highscore::~Highscore() {
	delete playerList;
	delete equipOneList;
	delete equipTwoList;
	delete equipThreeList;
	delete equipFourList;
}

void Highscore::setPlayerMessageList(MessageList* newPlayerList) {
	playerList = newPlayerList;
}

void Highscore::setEquipOneMessageList(MessageList* newEquipOneList) {
	equipOneList = newEquipOneList;
}

void Highscore::setEquipTwoMessageList(MessageList* newEquipTwoList) {
	equipTwoList = newEquipTwoList;
}

void Highscore::setEquipThreeMessageList(MessageList* newEquipThreeList) {
	equipThreeList = newEquipThreeList;
}

void Highscore::setEquipFourMessageList(MessageList* newEquipFourList) {
	equipFourList = newEquipFourList;
}

MessageList* Highscore::getEquipFourMessageList() {
	return equipFourList;
}

MessageList* Highscore::getEquipThreeMessageList() {
	return equipThreeList;
}

MessageList* Highscore::getEquipTwoMessageList() {
	return equipTwoList;
}

MessageList* Highscore::getEquipOneMessageList() {
	return equipOneList;
}

MessageList* Highscore::getPlayerMessageList() {
	return playerList;
}

void Highscore::setDragonsSlain(int newSlain) {
	dragonsSlain = newSlain;
}

int Highscore::getDragonsSlain() {
	return dragonsSlain;
}

void Highscore::setKillCount(int newCount) {
	killCount = newCount;
}

int Highscore::getKillCount() {
	return killCount;
}

void Highscore::setActions(int newActions) {
	actions = newActions;
}

int Highscore::getActions() {
	return actions;
}

void Highscore::write() {
	ofstream out;
	out.open(HIGHSCORE_FILENAME.c_str(), ios::app);

	//start write
	out << "# BEGIN" << endl;

	//write player info
	out << playerList->getMessages().at(0)->getMessage() << endl;
	out << dragonsSlain << endl << killCount << endl << actions << endl;
	out << playerList->getMessages().at(2)->getMessage() << endl;
	out << playerList->getMessages().at(3)->getMessage() << endl;
	out << playerList->getMessages().at(4)->getMessage() << endl;
	out << playerList->getMessages().at(5)->getMessage() << endl;

	//write equipOne
	out << "# EQUIPONE" << endl;
	for (unsigned int i = 0; i < equipOneList->getMessages().size(); i++) {
		out << equipOneList->getMessage(i)->getMessage() << endl;
	}

	//write equipTwo
	out << "# EQUIPTWO" << endl;
	for (unsigned int i = 0; i < equipTwoList->getMessages().size(); i++) {
		out << equipTwoList->getMessage(i)->getMessage() << endl;
	}

	//write equipThree
	out << "# EQUIPTHREE" << endl;
	for (unsigned int i = 0; i < equipThreeList->getMessages().size(); i++) {
		out << equipThreeList->getMessage(i)->getMessage() << endl;
	}

	//write equipFour
	out << "# EQUIPFOUR" << endl;
	for (unsigned int i = 0; i < equipFourList->getMessages().size(); i++) {
		out << equipFourList->getMessage(i)->getMessage() << endl;
	}

	out << "# END" << endl;

	out.close();
}