//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#ifndef HIGHSCORE
#define HIGHSCORE

#include "Def.h"
#include "MessageList.h"
#include "Player.h"
#include <fstream>
using namespace std;

class Highscore {
public:
	Highscore();
	Highscore(Player* thisPlayer);

	~Highscore();

	void setPlayerMessageList(MessageList* newPlayerList);
	void setEquipOneMessageList(MessageList* newEquipOneList); //mainhand
	void setEquipTwoMessageList(MessageList* newEquipTwoList); //offhand
	void setEquipThreeMessageList(MessageList* newEquipThreeList); //chest
	void setEquipFourMessageList(MessageList* newEquipFourList); //helm

	MessageList* getPlayerMessageList();
	MessageList* getEquipOneMessageList();
	MessageList* getEquipTwoMessageList();
	MessageList* getEquipThreeMessageList();
	MessageList* getEquipFourMessageList();

	void write();

	void setDragonsSlain(int newSlain);
	int getDragonsSlain();
	void setKillCount(int newCount);
	int getKillCount();
	void setActions(int newActions);
	int getActions();
private:
	MessageList *playerList, *equipOneList, *equipTwoList, *equipThreeList, *equipFourList;

	int dragonsSlain, killCount, actions;
};

#endif