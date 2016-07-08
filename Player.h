#ifndef PLAYER
#define PLAYER 

#include "def.h"
#include "Utility.h"
#include "Item.h"
#include "BoardLocation.h"
#include "SColor.h"
#include <GL/glut.h>
#include <sstream>
//#include <iostream>
#include <string>
using namespace std;

class Player {
public:
	Player();
	Player(string newName);
	~Player();

	int getCurrentHP();
	int getMaxHP();
	float getHealthPercent() { return (float)getCurrentHP() / (float)getMaxHP(); }

	bool isInjured() { return getCurrentHP() != getMaxHP(); }
	
	int getHP();
	int getAC();
	int getAR();
	int getDMG();
	int getREG();
	int getDR();
	int getLIGHT();
	int getWW();

	Item* getMainhand();
	Item* getOffhand();
	Item* getChest();
	Item* getHelm();

	string getName();
	string getNameAndTitle();

	//draws player info
	void drawPlayerInfo();
	//draws player icon on the map
	void draw(int drawOption);
	void drawEquipment();

	int getRandomDmg();

	int getAvgEquipLevel();

	void setLocation(BoardLocation* newLocation);
	BoardLocation* getLocation();

	void incrementKillCount();
	void incrementDragonsSlain();

	int getKillCount();
	int getDragonsSlain();
	int getScore();

	bool isDead();

	//used when player is dead to lightup the whole board in a cool fashion  :P
	void setLight(int newLight);

	void signalMoving(int direction);

	void changeHealthBy(int changeBy);

	void print();	

	void signalAttacked(bool success);
	void signalAttacking(int direction);

	void playerPressedYesOrNo(bool input);
	void signalNewItemAvailable(Item* theNewItem);
	bool getNewItemAvailable();
	Item* getNewItem();

	SColor* getColor();

	void turnUpdate();
	void frameUpdate(float deltaTime);
private:
	void renderBitmapString(float x, float y, float z, void *font, string theString, SColor* theColor);

	BoardLocation* location;

	void initialize();
	void equipmentUpdate();

	//holds a new item while the player decides to keep it or not
	bool newItemAvailable;
	Item* newItem;

	Item* items[5]; //item 0 holds nothing

	SColor* color; //player color
	SColor* healthColor; //color for drawing players health %

	int baseAC, baseHP, currentHP, baseLIGHT, baseREG;
	int hp, ac, ar, dmg, reg, dr, light, ww;
	int killCount, dragonsSlain;
	string name;

	int movingFrame;
	int moving;

	//drawing info
	bool attacked, attackSuccessful;
	int attackedFrame;

	bool attacking;
	int attackingFrame, attackingDirection;

	float rotationX, rotationY, rotationZ, degrees, currentDegrees;
};

#endif