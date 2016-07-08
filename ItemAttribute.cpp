#include "ItemAttribute.h"

ItemAttribute::ItemAttribute() {
	
	//generate attribute, level 1
	generateAttribute(1);
}

ItemAttribute::ItemAttribute(int newLevel) {
	//generate attribute with given level
	generateAttribute(newLevel);
}

ItemAttribute::~ItemAttribute() {
	
}	

int ItemAttribute::getACBonus() {
	return ac;
}

int ItemAttribute::getARBonus() {
	return ar;
}

int ItemAttribute::getDRBonus() {
	return dr;
}

int ItemAttribute::getHPBonus() {
	return hp;
}	

int ItemAttribute::getLevel() {
	return level;
}

int ItemAttribute::getLIBonus() {
	return light;
}

int ItemAttribute::getREBonus() {
	return reg;
}

int ItemAttribute::getWWBonus() {
	return ww;
}

int ItemAttribute::getDmgBonus() {
	return dmg;
}

void ItemAttribute::generateAttribute(int newLevel) {
	if (newLevel < 1) { //attribute level must be at least 1
		newLevel = 1;
	}

	//initialize the attributes to zero;
	ac = 0;
	ar = 0;
	dr = 0;
	hp = 0;
	level = newLevel;
	light = 0;
	reg = 0;
	ww = 0;
	dmg = 0;

	float random = Utility::generateRandomFloat(1.0f);
	
	if (random <= 0.15f) { //dmg bonus
		dmg = newLevel;	
	}
	else if (random <= 0.3f) { //hp bonus
		hp = (newLevel * 3) + Utility::generateRandomInt(newLevel * 2);
	}
	else if (random <= 0.45f) { //ac bonus
		ac = newLevel + Utility::generateRandomInt(1);
	}
	else if (random <= 0.6f) { //ar bonus
		ar = newLevel;
	}
	else if (random <= 0.7f) { //regeneration bonus
		reg = (newLevel + 2) / 3;
	}
	else if (random <= 0.8f) { //damage reduction bonus
		dr = (newLevel + 1) / 2 + Utility::generateRandomInt(1);
	}
	else if (random <= 0.9f) { //light radius bonus
		light = (newLevel + 1) / 2;
	}
	else if (random <= 1.0f) { //waterwalk bonus
		ww = 1;
	}
	else {
		//cout << "Invalid random generated.  ItemAttribute::generateAttribute()" << endl;
	}		
}

string ItemAttribute::toString() {
	ostringstream buffer;
	
	if (ac > 0) {
		buffer << "Level " << level << " attribute: +" << ac << " AC";
	}
	else if (ar > 0) {
		buffer << "Level " << level << " attribute: +" << ar << " AR";
	}
	else if (dr > 0) {
		buffer << "Level " << level << " attribute: " << dr << " Damage Reduction";
	}
	else if (hp > 0) {
		buffer << "Level " << level << " attribute: +" << hp << " HP";
	}
	else if (light > 0) {
		buffer << "Level " << level << " attribute: +" << light << " Light Radius";
	}
	else if (reg > 0) {
		buffer << "Level " << level << " attribute: +" << reg << " Regeneration";
	}
	else if (ww > 0) {
		buffer << "Level " << level << " attribute: Waterwalk";
	}
	else if (dmg > 0) {
		buffer << "Level " << level << " attribute: +" << dmg << " Damage";
	}
	else {
		buffer << "Level " << level << " attribute: No Attribute";
	}

	return buffer.str();
}

string ItemAttribute::toStringDisplay() {
	ostringstream buffer;
	
	if (ac > 0) {
		buffer << "+ " << ac << " Armor";
	}
	else if (ar > 0) {
		buffer << "+ " << ar << " Attack";
	}
	else if (dr > 0) {
		buffer << "+ " << dr << " Damage Reduction";
	}
	else if (hp > 0) {
		buffer << "+ " << hp << " Hit Points";
	}
	else if (light > 0) {
		buffer << "+ " << light << " Light Radius";
	}
	else if (reg > 0) {
		buffer << "+ " << reg << " Regeneration";
	}
	else if (ww > 0) {
		buffer << "Waterwalk";
	}
	else if (dmg > 0) {
		buffer << "+ " << dmg << " Damage";
	}
	else {
		buffer << "This shouldn't be there.  ItemAttribute::toStringDisplay()";
	}

	return buffer.str();
}