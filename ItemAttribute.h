#ifndef ITEM_ATTRIBUTE
#define ITEM_ATTRIBUTE

#include "Utility.h"
//#include <iostream>
#include <string>
#include <sstream>
using namespace std;

class ItemAttribute {
public:
	ItemAttribute();
	ItemAttribute(int newLevel);
	~ItemAttribute();

	int getHPBonus();
	int getACBonus();
	int getARBonus();
	int getREBonus();
	int getDRBonus();
	int getLIBonus();
	int getWWBonus();
	int getLevel();
	int getDmgBonus();

	string toString();
	string toStringDisplay();
private:
	void generateAttribute(int newLevel);

	int level, hp, ac, ar, reg, dr, light, ww, dmg;
};

#endif