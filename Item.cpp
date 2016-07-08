#include "Item.h"

Item::Item() {
	//create item level 0, random type
	initialize();
	generate(0);
}

Item::Item(int newLevel) {
	//create new item of level newLevel, random type
	initialize();

	//cout << "NEW LEVEL  " << newLevel << endl;

	generate(newLevel);	
}

Item::Item(int newLevel, int newType) {
	//create item of type newType and level newLevel
	initialize();
	type = newType;
	generate(newLevel);
}

Item::~Item() {

}

string Item::toString() {
	ostringstream buffer;

	buffer << "name=" << name << "  lvl=" << level << "  #attrib=" << numAttributes << "  type=" << type;
	return buffer.str();
}

string Item::getName() {
	return name;
}

void Item::generate(int newLevel) {
	numAttributes = 0;

	if (newLevel < 0) {
		level = 0;
	}
	else {
		level = newLevel;
	}

	//if no type has been determined already... determine one randomly
	if (type == 0) {
		type = Utility::generateRandomInt(3) + 1;
	}

	//set the item name
	createName();
	
	//generate base stats of item here
	if (type == MAINHAND) { //mainhand has basic dmg bonus
		avgdmg = 4 + level;
		dmgvar = Utility::generateRandomInt(level) + 1;
	}	
	if (type == OFFHAND) { //offhand has small innate hitpoint bonus
		baseAC += (level + 4) / 4;
		hp += ((level + 1) / 2) + Utility::generateRandomInt(level);
	}	
	if (type == CHEST) { //chest has innate damage reduction bonus
		baseAC += (level + 3) / 3;
		dr += (level + 1) / 3;
	}	
	if (type == HELM) { //helm has innate regeneration bonus
		baseAC += (level + 3) / 3;
		reg += (level + 1) / 4;
	}

	//level zero items have no magic properties
	if (level == 0) {
		return;
	}

	float chance;

	for (int i = 0; i < NUM_ITEM_ATTRIBUTES; i++) {
		chance = Utility::generateRandomFloat(1.0f);

		if (chance <= 0.44f) { //assign an attribute
			chance = Utility::generateRandomFloat(1.0f);

			//70% of the time, the attribute generated is the level of the item;  15% it is level + 1, 15% it is level - 1
			if (chance <= 0.7f) {
				attributes[numAttributes++] = ItemAttribute(level);
			}
			else if (chance <= 0.85f) {
				attributes[numAttributes++] = ItemAttribute(level - 1);
			}
			else if (chance <= 1.0f) {
				attributes[numAttributes++] = ItemAttribute(level + 1);
			}
			else {
				cout << "Invalid random number.  Item::generate()" << endl;
			}
		}
	}

	updateStats();
}

void Item::createName() {
	//first select item base name
	switch (type) {
		case MAINHAND:
			name = MAIN_HAND_STRINGS[Utility::generateRandomInt(MAIN_HAND_STRINGS_SIZE - 1)];		
			break;
		case OFFHAND:
			name = OFF_HAND_STRINGS[Utility::generateRandomInt(OFF_HAND_STRINGS_SIZE - 1)];
			break;
		case CHEST:
			name = CHEST_STRINGS[Utility::generateRandomInt(CHEST_STRINGS_SIZE - 1)];
			break;
		case HELM:	
			name = HELM_STRINGS[Utility::generateRandomInt(HELM_STRINGS_SIZE - 1)];
			break;
		default:
			name = "BAD NAME";
			break;
	}

	//items with level less than 1 do not recieve other name attributes
	if (level < 1) {
		return;
	}	

	float random = Utility::generateRandomFloat(1.0f);

	if (random <= 0.35f) { //just give the name a prefix
		name = PREFIX_STRINGS[Utility::generateRandomInt(PREFIX_STRINGS_SIZE - 1)] + " " + name;
	}
	else if (random <= 0.7f) { //just give the name a suffix
		name = name + " " + SUFFIX_STRINGS[Utility::generateRandomInt(SUFFIX_STRINGS_SIZE - 1)];
	}
	else if (random <= 1.0f) { //give the name both prefix and suffix!
		name = PREFIX_STRINGS[Utility::generateRandomInt(PREFIX_STRINGS_SIZE - 1)] + " " + name + " " + SUFFIX_STRINGS[Utility::generateRandomInt(SUFFIX_STRINGS_SIZE - 1)];
	}
	else {
		name = "BADNAME Item::createName()";
	}
}

int Item::getNumAttributes() {
	return numAttributes;
}

void Item::updateStats() {
	for (int i = 0; i < numAttributes; i++) {
		hp += attributes[i].getHPBonus();
		ac += attributes[i].getACBonus();
		ar += attributes[i].getARBonus();
		reg += attributes[i].getREBonus();
		dr += attributes[i].getDRBonus();
		light += attributes[i].getLIBonus();
		ww += attributes[i].getWWBonus();
		dmgBonus += attributes[i].getDmgBonus();
	}
}

void Item::initialize() {
	hp = 0;
	baseAC = 0;
	ac = 0;
	ar = 0;
	reg = 0;
	dr = 0;
	light = 0;
	ww = 0;
	avgdmg = 0;
	dmgvar = 0;
	numAttributes = 0;
	type = 0;
	level = 0;
	dmgBonus = 0;
	name = "";
}

int Item::getACBonus() {
	return baseAC + ac;
}

int Item::getARBonus() {
	return ar;
}

int Item::getAvgDmg() {
	return avgdmg;
}

int Item::getDmgVar() {
	return dmgvar;
}

int Item::getDRBonus() {
	return dr;
}

int Item::getHPBonus() {
	return hp;
}

int Item::getLIBonus() {
	return light;
}

int Item::getREBonus() {
	return reg;
}

int Item::getWWBonus() {
	return ww;
}

int Item::getLevel() {
	return level;
}

int Item::getDmgBonus() {
	return dmgBonus;
}

void Item::print() {
	cout << endl << getName() << endl;
	cout << "Item Level " << getLevel() << "  ||  " << getNumAttributes() << " Attributes" << endl;
	cout << "Item Type = " << type << endl;
	cout << "dmgBonus = " << dmgBonus << endl;
	cout << "baseAC = " << baseAC << endl;
	cout << "ac = " << ac << endl;
	cout << "ar = " << ar << endl;
	cout << "avgdmg = " << avgdmg << endl;
	cout << "dmgvar = " << dmgvar << endl;
	cout << "dr = " << dr << endl;
	cout << "hp = " << hp << endl;
	cout << "light = " << light << endl;
	cout << "reg = " << reg << endl;
	cout << "ww = " << ww << endl << endl;
	cout << "Attribute list: " << endl;

	for (int i = 0; i < numAttributes; i++) {
		cout << attributes[i].toString() << endl;
	}
}

MessageList* Item::createDisplayMsgList() {
	MessageList* newList = new MessageList(10);

	Message* newMsg;

	ostringstream buffer;

	//first line is item name with level
	//color is white, brighter white the better the item is
	buffer << name << " (" << level << ")";

	newMsg = new Message(buffer.str(), new SColor(0.6f + (float)level * 0.057f, 0.6f + (float)level * 0.057f, 0.6f + (float)level * 0.057f));
	newList->addMessage(newMsg);

	//second line shows the slot of the item along with the base dmg or ac
	buffer.str("");
	buffer.clear();
	SColor* newColor;

	switch (type) {
		case MAINHAND:
			buffer << "[Main Hand]  " << getDmgMin() << " to " << getDmgMax() << " Base Damage";
			newColor = new SColor(MAINHAND_COLOR[0], MAINHAND_COLOR[1], MAINHAND_COLOR[2]);
			break;
		case OFFHAND:
			buffer << "[Off Hand]  " << baseAC << " Innate Armor";
			newColor = new SColor(OFFHAND_COLOR[0], OFFHAND_COLOR[1], OFFHAND_COLOR[2]);
			break;
		case CHEST:
			buffer << "[Chest]  " << baseAC << " Innate Armor";
			newColor = new SColor(CHEST_COLOR[0], CHEST_COLOR[1], CHEST_COLOR[2]);
			break;
		case HELM:
			buffer << "[Head]  " << baseAC << " Innate Armor";
			newColor = new SColor(HELM_COLOR[0], HELM_COLOR[1], HELM_COLOR[2]);
			break;
		default:
			buffer << "INVALID TYPE : Item::createDisplayMsgList()  ";
			newColor = new SColor(1.0f, 1.0f, 1.0f);
			break;
	}

    newMsg = new Message(buffer.str(), newColor);
	newList->addMessage(newMsg);

	//	int hp, ac, ar, reg, dr, light, ww
	//create hp message if hp bonus exists
	if (hp > 0) {
		buffer.str("");
		buffer.clear();
		buffer << "+" << hp << " Health";
		newMsg = new Message(buffer.str(), new SColor(STAT_COLOR[0], STAT_COLOR[1], STAT_COLOR[2]));
		newList->addMessage(newMsg);
	}

	if (ac > 0) {
		buffer.str("");
		buffer.clear();
		buffer << "+" << ac << " Armor";
		newMsg = new Message(buffer.str(), new SColor(STAT_COLOR[0], STAT_COLOR[1], STAT_COLOR[2]));
		newList->addMessage(newMsg);
	}

	if (dmgBonus > 0) {
		buffer.str("");
		buffer.clear();
		buffer << "+" << dmgBonus << " Damage";
		newMsg = new Message(buffer.str(), new SColor(STAT_COLOR[0], STAT_COLOR[1], STAT_COLOR[2]));
		newList->addMessage(newMsg);
	}

	if (ar > 0) {
		buffer.str("");
		buffer.clear();
		buffer << "+" << ar << " Attack";
		newMsg = new Message(buffer.str(), new SColor(STAT_COLOR[0], STAT_COLOR[1], STAT_COLOR[2]));
		newList->addMessage(newMsg);
	}

	if (reg > 0) {
		buffer.str("");
		buffer.clear();
		buffer << "+" << reg << " Regeneration";
		newMsg = new Message(buffer.str(), new SColor(STAT_COLOR[0], STAT_COLOR[1], STAT_COLOR[2]));
		newList->addMessage(newMsg);
	}

	if (dr > 0) {
		buffer.str("");
		buffer.clear();
		buffer << "+" << dr << " Damage Reduction";
		newMsg = new Message(buffer.str(), new SColor(STAT_COLOR[0], STAT_COLOR[1], STAT_COLOR[2]));
		newList->addMessage(newMsg);
	}

	if (light > 0) {
		buffer.str("");
		buffer.clear();
		buffer << "+" << light << " Light Aura";
		newMsg = new Message(buffer.str(), new SColor(STAT_COLOR[0], STAT_COLOR[1], STAT_COLOR[2]));
		newList->addMessage(newMsg);
	}

	if (ww > 0) {
		buffer.str("");
		buffer.clear();
		buffer << "Waterwalk";
		newMsg = new Message(buffer.str(), new SColor(STAT_COLOR[0], STAT_COLOR[1], STAT_COLOR[2]));
		newList->addMessage(newMsg);
	}

	return newList;
}

void Item::drawNewItem() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glTranslatef(0.0f, 0.0f, -10.0f);
	glScalef(1.0f, 1.0f, 0.05f);

	glBegin(GL_QUADS); 
	glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
	glVertex3f(-1.0f, 1.6f, 0.0f);
	glVertex3f(1.0f, 1.6f, 0.0f);
	glVertex3f(1.0f, -1.6f, 0.0f);
	glVertex3f(-1.0f, -1.6f, 0.0f);

	//draw outer border
	glColor3f(0.8f, 0.4f, 0.11f);      
	glVertex3f(-1.0f, -1.6f, -0.1f);
	glVertex3f(-0.98f, -1.6f, -0.1f);
	glVertex3f(-0.98f, 1.58f, -0.1f);
	glVertex3f(-1.0f, 1.58f, -0.1f);
	glVertex3f(-1.0f, 1.58f, -0.1f);
	glVertex3f(-1.0f, 1.6f, -0.1f);
	glVertex3f(0.98f, 1.6f, -0.1f);
	glVertex3f(0.98f, 1.58f, -0.1f);	
	glVertex3f(0.98f, 1.6f, -0.1f);
	glVertex3f(1.0f, 1.6f, -0.1f);
	glVertex3f(1.0f, -1.58f, -0.1f);
	glVertex3f(0.98f, -1.58f, -0.1f);
	glVertex3f(1.0f, -1.58f, -0.1f);
	glVertex3f(1.0f, -1.6f, -0.1f);
	glVertex3f(-0.98f, -1.6f, -0.1f);
	glVertex3f(-0.98f, -1.58f, -0.1f);

	//draw inner border
	glColor3f(0.91f, 0.44f, 0.13f);
	glVertex3f(-0.98f, -1.58f, -0.1f);
	glVertex3f(-0.97f, -1.58f, -0.1f);
	glVertex3f(-0.97f, 1.57f, -0.1f);
	glVertex3f(-0.98f, 1.57f, -0.1f);
	glVertex3f(-0.98f, 1.57f, -0.1f);
	glVertex3f(-0.98f, 1.58f, -0.1f);
	glVertex3f(0.97f, 1.58f, -0.1f);
	glVertex3f(0.97f, 1.57f, -0.1f);	
	glVertex3f(0.97f, 1.58f, -0.1f);
	glVertex3f(0.98f, 1.58f, -0.1f);
	glVertex3f(0.98f, -1.57f, -0.1f);
	glVertex3f(0.97f, -1.57f, -0.1f);
	glVertex3f(0.98f, -1.57f, -0.1f);
	glVertex3f(0.98f, -1.58f, -0.1f);
	glVertex3f(-0.97f, -1.58f, -0.1f);
	glVertex3f(-0.97f, -1.57f, -0.1f);	
	glEnd();

	glDisable(GL_BLEND);

	//disable lighting if it was on
	int lightingOn[1];

	glGetIntegerv(GL_LIGHTING, lightingOn);

	if (lightingOn[0] > 0) {
		lightingOn[0] = 1;
		glDisable(GL_LIGHTING);
		//glDisable(GL_LIGHT0);
		//glDisable(GL_COLOR_MATERIAL);		
	}

	Message* newMsg = new Message("You have found a new item!", new SColor(1.0f, 1.0f, 1.0f));
	renderBitmapString(-0.9f, 1.1f, -0.1f, GLUT_BITMAP_HELVETICA_18, newMsg->getCharMessage(), newMsg->getColor());

	Message* newMsg2 = new Message("Do you want to replace your current item?  (Y / N)", new SColor(1.0f, 1.0f, 1.0f));
	renderBitmapString(-0.9f, -1.1f, -0.1f, GLUT_BITMAP_HELVETICA_18, newMsg2->getCharMessage(), newMsg2->getColor());

	delete newMsg;
	delete newMsg2;

	glPushMatrix();
	glScalef(1.0f, 0.75f, 1.0f);
	draw();
	glPopMatrix();

	//turn lighting back on if it was on
	if (lightingOn[0]) {
		glEnable(GL_LIGHTING);
		//glEnable(GL_COLOR_MATERIAL);
		//glEnable(GL_LIGHT0);
	}
}

void Item::draw() {
	MessageList* list = createDisplayMsgList();

	//draw colored border based on item type
	glBegin(GL_QUADS);
	glColor3f(list->getMessage(1)->getColor()->getR(), list->getMessage(1)->getColor()->getG(), list->getMessage(1)->getColor()->getB());
	glVertex3f(-1.0f, -1.0f, -0.1f);
	glVertex3f(-0.95f, -1.0f, -0.1f);
	glVertex3f(-0.95f, 0.95f, -0.1f);
	glVertex3f(-1.0f, 0.95f, -0.1f);
	glVertex3f(-1.0f, 0.95f, -0.1f);
	glVertex3f(-1.0f, 1.0f, -0.1f);
	glVertex3f(0.95f, 1.0f, -0.1f);
	glVertex3f(0.95f, 0.95f, -0.1f);	
	glVertex3f(0.95f, 1.0f, -0.1f);
	glVertex3f(1.0f, 1.0f, -0.1f);
	glVertex3f(1.0f, -0.95f, -0.1f);
	glVertex3f(0.95f, -0.95f, -0.1f);
	glVertex3f(1.0f, -0.95f, -0.1f);
	glVertex3f(1.0f, -1.0f, -0.1f);
	glVertex3f(-0.95f, -1.0f, -0.1f);
	glVertex3f(-0.95f, -0.95f, -0.1f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();

	float space = 1.44f / ((float)list->getSize() + 0.0f);

	Message* currentMessage = list->getMessage(0);

	for (int i = 0; i < list->getSize(); i++) {
		currentMessage = list->getMessage(i);

		if (i == 0) {
			renderBitmapString(-0.9f, 0.54f, -0.1f, GLUT_BITMAP_HELVETICA_18, currentMessage->getCharMessage(), currentMessage->getColor());
		}
		else {
			renderBitmapString(-0.9f, 0.54f - space * (float)(i), -0.1f, GLUT_BITMAP_HELVETICA_12, currentMessage->getCharMessage(), currentMessage->getColor());
		}
	}

	delete list;
}

int Item::getDmgMax() {
	return avgdmg + dmgvar;
}

int Item::getDmgMin() {
	return avgdmg - dmgvar;
}

int Item::getDmgRange() {
	return dmgvar * 2;
}

void Item::renderBitmapString(float x, float y, float z, void *font, string theString, SColor* theColor) { 
	//disable lighting if it was on
	int lightingOn[1];

	glGetIntegerv(GL_LIGHTING, lightingOn);

	if (lightingOn[0] > 0) {
		lightingOn[0] = 1;
		glDisable(GL_LIGHTING);
		//glDisable(GL_LIGHT0);
		//glDisable(GL_COLOR_MATERIAL);		
	}
		
	glColor3f(theColor->getR(), theColor->getG(), theColor->getB());

	char line[75];
	strcpy_s(line, theString.substr(0, 74).c_str());
	
	char *c;
	glRasterPos3f(x, y, z);

	for (c =line; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}

	//turn lighting back on if it was on
	if (lightingOn[0]) {
		glEnable(GL_LIGHTING);
		//glEnable(GL_COLOR_MATERIAL);
		//glEnable(GL_LIGHT0);
	}
}

int Item::getType() {
	return type;
}