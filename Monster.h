#ifndef MONSTER
#define MONSTER

#include "Utility.h"
#include "def.h"
#include "SColor.h"
#include "BoardLocation.h"
#include <GL/glut.h>
//#include <iostream>
#include <string>
#include <sstream>
using namespace std;

class Monster {
public:
	Monster(); //default constructor
	Monster(int newLevel);
	Monster(int newLevel, string newName, bool newDragon, SColor* newColor);

	~Monster();

	int getMaxHP();
	int getCurrentHP();
	void changeHealthBy(int changeBy);
	
	int getAC();
	int getAR();
	int getREG();
	int getDR();
	int getLevel();

	bool isDragon();

	int getDmgBonus();

	int getAvgDmg();
	int getDmgVar();

	int getRandomDmg(); //returns a random result of a damage roll

	string getName();
	string getBonus();
	
	int generateItemLevel();

	void print();
	
	void drawInfo();
	void draw(int drawOption);

	void turnUpdate();
	void frameUpdate();

	SColor* getColor();

	void setLocation(BoardLocation* newLocation);
	BoardLocation* getLocation();

	void signalAttacked(bool success);
	void signalAttacking(int direction);

	void signalMoving(int direction);

	bool getDead();
	void signalDead();
private:
	void renderBitmapString(float x, float y, float z, void *font, string theString, SColor* theColor);

	void generate();
	void generateName();
	void initialize();

	bool dragon;

	BoardLocation* location;

	string name, bonus;

	int improvedAC, improvedAR, improvedDR, improvedREG, improvedDMG;

	int currentHP, maxHP, ac, ar, reg, dr, avgdmg, dmgvar, level, dmg;
	SColor* color;

	int movingFrame;
	int moving;

	//true if the monster is dead
	bool dead;

	//drawing stuff
	bool attacked, attackSuccessful;
	int attackedFrame;

	bool attacking;
	int attackingFrame, attackingDirection;

	int slices, stacks;
	float rotationX, rotationY, rotationZ, degrees, currentDegrees;
};

#endif