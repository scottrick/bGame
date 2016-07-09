//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#include "Player.h"

Player::Player() {
	name = RANDOM_NAMES[Utility::generateRandomInt(RANDOM_NAMES_SIZE - 1)];
	initialize();
}

Player::Player(string newName) {
	name = newName;
	initialize();
}

//create a new player for populating the highscore list;  skill number refers to the number of dragons killed, and the level of equipment
Player::Player(int dkills, int kills) {
	name = RANDOM_NAMES[Utility::generateRandomInt(RANDOM_NAMES_SIZE - 1)];
	initialize();

	//generate level SKILL items for all slots
	for (int i = 1; i <= 4; i++) {
		items[i] = new Item(dkills + Utility::generateRandomInt(2), i);
	}

	dragonsSlain = dkills;
	killCount = kills;
	actions = killCount * 14 + Utility::generateRandomInt(50);

	equipmentUpdate();
}

Player::~Player() {
	for (int i = 1; i <= 4; i++) {
		delete items[i];
	}

	delete healthColor;
	delete color;
}

void Player::initialize() {
	baseHP = 50;
	currentHP = 50;
	hp = 0;
	ac = 0;
	baseAC = 5;
	ar = 0;
	dmg = 0;
	reg = 0;
	dr = 0;
	light = 0;
	baseLIGHT = 4;
	baseREG = 1;
	ww = 0;
	killCount = 0;
	dragonsSlain = 0;
	location = NULL;
	currentDegrees = 0;
	moving = 0;
	movingFrame = 0;
	attacked = false;
	attackSuccessful = false;
	attackedFrame = 0;
	attacking = false;
	attackingFrame = 0;
	attackingDirection = 0;
	newItemAvailable = false;
	cheated = false;
	actions = 0;

	rotationX = Utility::generateRandomFloat(2.0f) - 1.0f;
	rotationY = Utility::generateRandomFloat(2.0f) - 1.0f;
	rotationZ = Utility::generateRandomFloat(2.0f) - 1.0f;
	degrees = 90.0f;

	healthColor = new SColor(0.15f, 0.15f, 0.15f);
	color = new SColor(1.0f, 1.0f, 1.0f);

	int itemLevel = 0;

	if (name == "tester50") {
		itemLevel = 50;
		cheated = true;
	}
	if (name == "tester7") {
		itemLevel = 7;
		cheated = true;
	}

	//generate level 0 items for all slots
	for (int i = 1; i <= 4; i++) {
		items[i] = new Item(itemLevel, i);
	}

	equipmentUpdate();
}

void Player::equipmentUpdate() {
	//updates the bonuses from all the items 
	//call this function whenever a piece of equipment changes

	hp = 0;
	ac = 0;
	ar = 0;
	reg = 0;
	dr = 0;
	light = 0;
	ww = 0;
	dmg = 0;

	for (int i = 1; i <= 4; i++) {
		hp += items[i]->getHPBonus();
		ac += items[i]->getACBonus();
		ar += items[i]->getARBonus();
		reg += items[i]->getREBonus();
		dr += items[i]->getDRBonus();
		light += items[i]->getLIBonus();
		ww += items[i]->getWWBonus();
		dmg += items[i]->getDmgBonus();
	}

	//do not allow hp to be greater than max!
	if (currentHP > getMaxHP()) {
		currentHP = getMaxHP();
	}
}

void Player::print() {
	/*
	cout << "~~~PLAYER SUMMARY~~~" << endl;
	cout << "name: " << getNameAndTitle() << endl;
	cout << "killcount=" << getKillCount() << endl;
	cout << "dragonsSlain=" << getDragonsSlain() << endl;
	cout << getCurrentHP() << " / " << getMaxHP() << " HP" << endl;
	cout << getAC() << " AC" << endl;
	cout << getAR() << " AR" << endl;
	cout << getREG() << " Regeneration" << endl;
	cout << getDR() << " Damage Reduction" << endl;
	cout << getLIGHT() << " Light Aura" << endl;
	cout << getWW() << " Waterwalk" << endl;
	cout << getDMG() << " Bonus Damage" << endl;

	for (int i = 1; i <= 4; i++) {
	cout << endl;
	items[i]->print();
	}
	*/
}

int Player::getCurrentHP() {
	return currentHP;
}

int Player::getMaxHP() {
	return baseHP + hp;
}

int Player::getAC() {
	return baseAC + ac;
}

int Player::getAR() {
	return ar;
}

void Player::setCheated() {
	cheated = true;
}

int Player::getREG() {
	return baseREG + reg;
}

int Player::getDR() {
	return dr;
}

int Player::getLIGHT() {
	return light + baseLIGHT;
}

int Player::getWW() {
	return ww;
}

int Player::getKillCount() {
	return killCount;
}

int Player::getDragonsSlain() {
	return dragonsSlain;
}

string Player::getName() {
	return name;
}

int Player::getDMG() {
	return dmg;
}

int Player::getScore() {
	return getKillCount() + (24 * getDragonsSlain());
}

//return name with title.  ex:  "Lord Scott"
string Player::getNameAndTitle() {
	if (cheated) {
		return "Cheater " + getName();
	}

	int score = getScore();

	if (score < 50) {
		return PLAYER_TITLES[0] + " " + getName();
	}
	else if (score < 200) {
		return PLAYER_TITLES[1] + " " + getName();
	}
	else if (score < 500) {
		return PLAYER_TITLES[2] + " " + getName();
	}
	else {
		return PLAYER_TITLES[3] + " " + getName();
	}
}

Item* Player::getMainhand() {
	return items[MAINHAND];
}

Item* Player::getOffhand() {
	return items[OFFHAND];
}

Item* Player::getChest() {
	return items[CHEST];
}

Item* Player::getHelm() {
	return items[HELM];
}

void Player::drawPlayerInfo() {
	SColor* renderColor;
	ostringstream buffer;

	float top = 0.95f;
	float increment = (2.0f * top) / 10.0f;

	const float HEALTH_X = 0.95f;
	const float HEALTH_Y_BOTTOM = top - 3 * increment;
	const float HEALTH_Y_TOP = top - 2 * increment;

	//draw name and title
	renderColor = new SColor(1.0f, 1.0f, 1.0f);
	renderBitmapString(-0.95f, top - increment, -0.1f, GLUT_BITMAP_HELVETICA_18, getNameAndTitle(), renderColor);
	delete renderColor;

	//draw health bar border
	glBegin(GL_QUADS);
	float diff = 0.01f;
	glColor3f(1.0f, 0.67f, 0.0f);
	glVertex3f(-HEALTH_X, HEALTH_Y_BOTTOM, -0.1f);
	glVertex3f(-HEALTH_X, HEALTH_Y_TOP - diff, -0.1f);
	glVertex3f(-HEALTH_X + diff, HEALTH_Y_TOP - diff, -0.1f);
	glVertex3f(-HEALTH_X + diff, HEALTH_Y_BOTTOM, -0.1f);
	glVertex3f(-HEALTH_X, HEALTH_Y_TOP - diff, -0.1f);
	glVertex3f(-HEALTH_X, HEALTH_Y_TOP, -diff);
	glVertex3f(HEALTH_X - diff, HEALTH_Y_TOP, -0.1f);
	glVertex3f(HEALTH_X, HEALTH_Y_TOP - diff, -0.1f);
	glVertex3f(HEALTH_X - diff, HEALTH_Y_TOP, -0.1f);
	glVertex3f(HEALTH_X, HEALTH_Y_TOP, -0.1f);
	glVertex3f(HEALTH_X, HEALTH_Y_BOTTOM + diff, -0.1f);
	glVertex3f(HEALTH_X - diff, HEALTH_Y_BOTTOM + diff, -0.1f);
	glVertex3f(HEALTH_X, HEALTH_Y_BOTTOM + diff, -0.1f);
	glVertex3f(HEALTH_X, HEALTH_Y_BOTTOM, -0.01f);
	glVertex3f(-HEALTH_X + diff, HEALTH_Y_BOTTOM, -0.1f);
	glVertex3f(-HEALTH_X + diff, HEALTH_Y_BOTTOM + diff, -0.1f);

	//draw health bar, color based on %full
	float percent = (float)getCurrentHP() / (float)getMaxHP();
	float xRange = (2 * HEALTH_X - 0.02f) * percent;

	//health bar gets brighter red as it gets lower
	glColor3f(1.0f - percent * 0.65f, 0.0f, 0.0f);
	glVertex3f(-HEALTH_X + 0.01f, HEALTH_Y_BOTTOM + 0.01f, -0.1f);
	glVertex3f(-HEALTH_X + 0.01f, HEALTH_Y_TOP - 0.01f, -0.1f);
	glVertex3f(-HEALTH_X + 0.01f + xRange, HEALTH_Y_TOP - 0.01f, -0.1f);
	glVertex3f(-HEALTH_X + 0.01f + xRange, HEALTH_Y_BOTTOM + 0.01f, -0.1f);
	glEnd();

	//draw text with health total.  ex:  44/50
	renderColor = new SColor(1.0f, 1.0f, 1.0f);
	buffer.str("");
	buffer.clear();
	buffer << getCurrentHP() << " / " << getMaxHP();
	renderBitmapString(-0.1f, (HEALTH_Y_BOTTOM + HEALTH_Y_TOP) / 2.0f - 0.035f, -0.11f, GLUT_BITMAP_HELVETICA_12, buffer.str(), renderColor);
	delete renderColor;

	//draw kill count and # of dragons slain
	buffer.str("");
	buffer.clear();
	buffer << getDragonsSlain() << " / " << NUM_DRAGONS << " Dragons Slain    " << getKillCount() << " Kills    " << getActions() << " Actions";
	renderColor = new SColor(0.85f, 0.85f, 0.85f);
	renderBitmapString(-0.95f, top - 4 * increment, -0.1f, GLUT_BITMAP_HELVETICA_12, buffer.str(), renderColor);

	//draw armor and attack bonus
	buffer.str("");
	buffer.clear();
	buffer << getAC() << " Armor        +" << getAR() << " Attack";
	renderBitmapString(-0.95f, top - 5 * increment, -0.1f, GLUT_BITMAP_HELVETICA_12, buffer.str(), renderColor);

	//draw attack dmg and avg
	buffer.str("");
	buffer.clear();
	buffer << getMainhand()->getDmgMin() << " to " << getMainhand()->getDmgMax() << " + " << getDMG() << "  (" << getMainhand()->getAvgDmg() + getDMG() << " Avg Damage)";
	renderBitmapString(-0.95f, top - 6 * increment, -0.1f, GLUT_BITMAP_HELVETICA_12, buffer.str(), renderColor);
	delete renderColor;

	buffer.str("");
	buffer.clear();
	buffer << "+" << getREG() << " Regeneration        +" << getDR() << " Damage Reduction";
	renderColor = new SColor(0.2f, 0.6f + 0.05f * (getREG() + getDR()), 0.2f);
	renderBitmapString(-0.95f, top - 7 * increment, -0.1f, GLUT_BITMAP_HELVETICA_12, buffer.str(), renderColor);
	delete renderColor;

	buffer.str("");
	buffer.clear();
	buffer << "+" << getLIGHT() << " Light Aura";
	renderColor = new SColor(0.5f + 0.05f * getLIGHT(), 0.5f + 0.05f * getLIGHT(), 0.5f + 0.05f * getLIGHT());
	renderBitmapString(-0.95f, top - 8 * increment, -0.1f, GLUT_BITMAP_HELVETICA_12, buffer.str(), renderColor);
	delete renderColor;

	buffer.str("");
	buffer.clear();
	if (getWW() > 0) {
		renderColor = new SColor(0.1f, 0.1f, 0.95f);
		buffer << "Waterwalk";
	}
	else {
		renderColor = new SColor(0.6f, 0.6f, 0.6f);
		buffer << "No Waterwalk";
	}
	renderBitmapString(-0.95f, top - 9 * increment, -0.1f, GLUT_BITMAP_HELVETICA_12, buffer.str(), renderColor);
	delete renderColor;
}

void Player::renderBitmapString(float x, float y, float z, void *font, string theString, SColor* theColor) {
	//disable lighting if it was on
	int lightingOn[1];

	glGetIntegerv(GL_LIGHTING, lightingOn);

	if (lightingOn[0] > 0) {
		lightingOn[0] = 1;
		glDisable(GL_LIGHTING);
	}

	glColor3f(theColor->getR(), theColor->getG(), theColor->getB());

	char line[75];
	strcpy_s(line, theString.substr(0, 74).c_str());

	char *c;
	glRasterPos3f(x, y, z);

	for (c = line; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}

	//turn lighting back on if it was on
	if (lightingOn[0]) {
		glEnable(GL_LIGHTING);
	}
}

void Player::changeHealthBy(int changeBy) {
	currentHP += changeBy;

	if (currentHP < 0) {
		currentHP = 0;
	}
}

void Player::drawEquipment() {
	int dimensions[4];
	glGetIntegerv(GL_VIEWPORT, dimensions);

	float height = (float)dimensions[3] / 4.0f;

	//draw item one
	glViewport(dimensions[0], dimensions[1], dimensions[2], (int)height);
	items[1]->draw();

	//draw item two
	glViewport(dimensions[0], dimensions[1] + (int)height, dimensions[2], (int)height);
	items[2]->draw();

	//draw item two
	glViewport(dimensions[0], dimensions[1] + (int)(height * 2.0f), dimensions[2], (int)height);
	items[3]->draw();

	//draw item four
	glViewport(dimensions[0], dimensions[1] + (int)(height * 3.0f), dimensions[2], (int)height);
	items[4]->draw();
}

void Player::setLocation(BoardLocation* newLocation) {
	location = newLocation;
}

BoardLocation* Player::getLocation() {
	return location;
}

void Player::draw(int drawOption) {
	glTranslatef(0.0f, 0.0f, -1.1f);

	//draw attacked animation below the player piece, if under attack
	if (attacked) {
		//cout << "drawing attacked drawzz" << endl;

		if (attackSuccessful) {
			glColor3f(1.0f - (1.0f / (float)TURN_FRAME_LENGTH) * (float)attackedFrame, 0.0f, 0.0f);
		}
		else {
			glColor3f(1.0f - (1.0f / (float)TURN_FRAME_LENGTH) * (float)attackedFrame, 1.0f - (1.0f / (float)TURN_FRAME_LENGTH) * (float)attackedFrame, 1.0f - (1.0f / (float)TURN_FRAME_LENGTH) * (float)attackedFrame);
		}

		glBegin(GL_QUADS);
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glEnd();
	}

	//if the player is dead, don't draw!
	if (isDead()) {
		return;
	}

	glTranslatef(0.0f, 0.0f, -0.1f);

	//draw attacking animation if the player is attacking
	if (attacking) {
		glPushMatrix();

		glColor3f(0.1f + (0.4f / (float)(TURN_FRAME_LENGTH / 2)) * (float)attackingFrame, 0.1f + (0.4f / (float)(TURN_FRAME_LENGTH / 2)) * (float)attackingFrame, 1.0f);

		if (attackingDirection == NORTH) {

			glRotatef(0.0f - (90.0f / (float)(TURN_FRAME_LENGTH / 2)) * attackingFrame, 0.0f, 0.0f, 1.0f);

		}
		else if (attackingDirection == SOUTH) {
			//rotate 180 degrees
			glRotatef(180.0f - (90.0f / (float)(TURN_FRAME_LENGTH / 2)) * attackingFrame, 0.0f, 0.0f, 1.0f);
		}
		else if (attackingDirection == EAST) {
			glRotatef(270.0f - (90.0f / (float)(TURN_FRAME_LENGTH / 2)) * attackingFrame, 0.0f, 0.0f, 1.0f);
		}
		else if (attackingDirection == WEST) {
			glRotatef(90.0f - (90.0f / (float)(TURN_FRAME_LENGTH / 2)) * attackingFrame, 0.0f, 0.0f, 1.0f);
		}

		glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-1.7f, 1.3f, 0.0f);
		glVertex3f(-1.3f, 1.7f, 0.0f);
		glEnd();

		glPopMatrix();
	}

	glTranslatef(0.0f, 0.0f, -3.0f);

	//translate based on moving and direction
	if (moving) {
		float moveX, moveY;

		if (moving == NORTH) {
			moveX = 0.0f;
			moveY = -2.0f;
		}
		if (moving == SOUTH) {
			moveX = 0.0f;
			moveY = 2.0f;
		}
		if (moving == EAST) {
			moveX = -2.0f;
			moveY = 0.0f;
		}
		if (moving == WEST) {
			moveX = 2.0f;
			moveY = 0.0f;
		}

		moveX -= moveX * ((float)movingFrame / (float)(TURN_FRAME_LENGTH / 2));
		moveY -= moveY * ((float)movingFrame / (float)(TURN_FRAME_LENGTH / 2));

		glTranslatef(moveX, moveY, 0.0f);
	}

	//draw player health %
	if (drawOption > 0) {
		ostringstream buffer;

		//cout << "Monster DRAW INFO" << endl;
		//cout << color->getR() << "  " << color->getG() << "  " << color->getB() << endl;

		buffer << (int)(((float)getCurrentHP() / (float)getMaxHP()) * 100.0f) << "%";
		renderBitmapString(-0.98f, 0.98f, -0.1f, GLUT_BITMAP_HELVETICA_10, buffer.str(), healthColor);
	}

    //draw health bar if injured
	if (isInjured()) {
		float x1 = -1.0f;
		float y1 = 1.15f;
		float x2 = 1.0f;
		float y2 = 0.75f;

		glDisable(GL_LIGHTING);
		glColor4f(0.85f, 0.0f, 0.0f, 1.0f);

		float width = (x2 - x1) * getHealthPercent();
		glRectf(x1, y1, x1 + width, y2);

		glEnable(GL_LIGHTING);
	}

	glRotatef(currentDegrees, rotationX, rotationY, rotationZ);
	glScalef(1.0f, 1.0f, 1.0f);
	glColor3f(color->getR(), color->getG(), color->getB());

	glutSolidOctahedron();
}

void Player::frameUpdate(float deltaTime) {
	currentDegrees += degrees * deltaTime;
	if (currentDegrees > 360.0f) {
		currentDegrees -= 360.0f;
	}

	if (moving > 0) { //if the player is moving, then advance its position by 1 frame
		movingFrame++;

		if (movingFrame > (TURN_FRAME_LENGTH / 2)) { //set not moving if move has completed
			moving = 0;
			movingFrame = 0;
		}
	}

	if (attacked) { //if the player is under attack, then advance its attack frame by one
		attackedFrame++;

		if (attackedFrame > (TURN_FRAME_LENGTH / 2)) { //set to not attacked once attacked is comleted
			attacked = false;
			attackedFrame = 0;
			attackSuccessful = false;
		}
	}

	if (attacking) {  //if the player is attacking a monster, advance the attack by one frame
		attackingFrame++;

		if (attackingFrame > (TURN_FRAME_LENGTH / 2)) { //set to not attacking once the attack is completed
			attacking = false;
			attackingFrame = 0;
			attackingDirection = 0;
		}
	}
}

void Player::signalMoving(int direction) {
	moving = direction;
	movingFrame = 0;
}

int Player::getRandomDmg() {
	return getMainhand()->getAvgDmg() + (Utility::generateRandomInt(getMainhand()->getDmgVar() * 2) - getMainhand()->getDmgVar()) + getDMG();
}

/*
Attack signaling works like this:

When a monster attacks, it signals the player so the player knows enough to draw the attack animation.
The attackedFrame is initialized to zero.
The attacked bool is set to TRUE since the player is being attacked.
IF the attack was successful, the attackSuccessful is set to TRUE.
If multiple attacks occur against the player, only one needs to be successful for the attack animation to draw the successful animation.
*/
void Player::signalAttacked(bool success) {
	attackedFrame = 0;

	attacked = true;

	if (!attackSuccessful) {
		attackSuccessful = success;
	}
}

void Player::signalAttacking(int direction) {
	attacking = true;
	attackingFrame = 0;
	attackingDirection = direction;
}

void Player::incrementKillCount() {
	killCount++;
}

void Player::incrementDragonsSlain() {
	dragonsSlain++;
}

bool Player::isDead() {
	if (getCurrentHP() <= 0) {
		return true;
	}
	else {
		return false;
	}
}

SColor* Player::getColor() {
	return color;
}

void Player::signalNewItemAvailable(Item* theNewItem) {
	newItem = theNewItem;
	newItemAvailable = true;
}

//handle Yes/No Dialog boxes with this 
void Player::playerPressedYesOrNo(bool input) {
	//if a new item is available, this is the players choice
	if (newItemAvailable) {
		//player says YES
		if (input) {
			delete items[newItem->getType()];
			items[newItem->getType()] = newItem;
			newItemAvailable = false;
			equipmentUpdate();
		}
		//player says NO
		else {
			delete newItem;
			newItemAvailable = false;
		}
	}
}

Item* Player::getNewItem() {
	return newItem;
}

bool Player::getNewItemAvailable() {
	return newItemAvailable;
}

int Player::getAvgEquipLevel() {
	int temp = 0;

	for (int i = 1; i < 5; i++) {
		temp += items[i]->getLevel();
	}

	temp = temp / 4;

	//min avg equip level is 1
	if (temp <= 0) {
		temp = 1;
	}

	return temp;
}

//increase health based on regen, etc
void Player::turnUpdate() {
	//do nothing if the player is already dead
	if (isDead()) {
		return;
	}

	currentHP += getREG();

	if (currentHP > getMaxHP()) {
		currentHP = getMaxHP();
	}
}

void Player::setLight(int newLight) {
	light = newLight;
}

MessageList* Player::createHighscoreList() {
	MessageList* list = new MessageList();

	ostringstream buffer;

	list->addMessage(new Message(getNameAndTitle(), NULL));

	buffer << getDragonsSlain() << " / " << NUM_DRAGONS << " Dragons Slain    " << getKillCount() << " Kills    " << getActions() << " Actions";
	list->addMessage(new Message(buffer.str(), NULL));

	buffer.str("");
	buffer.clear();
	buffer << getAC() << " Armor        +" << getAR() << " Attack";
	list->addMessage(new Message(buffer.str(), NULL));

	buffer.str("");
	buffer.clear();
	buffer << getMainhand()->getDmgMin() << " to " << getMainhand()->getDmgMax() << " + " << getDMG() << "  (" << getMainhand()->getAvgDmg() + getDMG() << " Avg Damage)";
	list->addMessage(new Message(buffer.str(), NULL));

	buffer.str("");
	buffer.clear();
	buffer << "+" << getREG() << " Regeneration        +" << getDR() << " Damage Reduction";
	list->addMessage(new Message(buffer.str(), NULL));

	buffer.str("");
	buffer.clear();
	buffer << "+" << getLIGHT() << " Light Aura";
	list->addMessage(new Message(buffer.str(), NULL));

	//don't include waterwalk stat on highscores

	return list;
}

Item* Player::getItem(int type) {
	//check to see if it is a valid item type
	if ((type < 1) || (type > 4)) {
		//cout << "INVALID ITEM TYPE!!!!!!" << endl;
	}

	return items[type];
}

int Player::getActions() {
	return actions;
}

void Player::incrementActions() {
	actions++;
}