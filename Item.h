//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#ifndef ITEM
#define ITEM

#include "ItemAttribute.h"
#include "def.h"
#include "MessageList.h"
#include <GL/glut.h>
#include <string>
using namespace std;

class Item {
public:
	Item();
	Item(int newLevel);
	Item(int newLevel, int newType);
	~Item();

	int getHPBonus();
	int getACBonus();
	int getARBonus();
	int getREBonus();
	int getDRBonus();
	int getLIBonus();
	int getWWBonus();
	int getAvgDmg();
	int getDmgVar();
	int getLevel();
	int getDmgBonus();
	int getType();

	int getDmgMin();
	int getDmgMax();
	int getDmgRange();

	int getNumAttributes();

	void draw();

	string toString();
	string getName();

	MessageList* createDisplayMsgList();

	void drawNewItem();

	void print();
private:
	void renderBitmapString(float x, float y, float z, void *font, string theString, SColor* theColor);

	void initialize();
	void createName();
	void generate(int newLevel);
	void updateStats();

	int baseAC;
	int hp, ac, ar, reg, dr, light, ww, avgdmg, dmgvar, dmgBonus;

	int level, type, numAttributes; //level holds the level of the item.  slot holds what type of item it is (main hand, off hand, head, chest)
	string name;

	ItemAttribute attributes[NUM_ITEM_ATTRIBUTES];
};

#endif