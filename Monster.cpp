#include "Monster.h"

Monster::Monster() { //randomly generate level 1 monster with random name
	level = 1;
	dragon = false;
	color = NULL;

	initialize();
	generateName();
	generate();
}	

Monster::Monster(int newLevel) { //randomly generate level newLevel monster with random name
	level = newLevel;
	dragon = false;
	color = NULL;

	initialize();
	generateName();
	generate();
}

Monster::Monster(int newLevel, string newName, bool newDragon, SColor* newColor) { //new monster with given level and name
	level = newLevel;
	name = newName;
	dragon = newDragon;
	color = newColor;

	initialize();
	generate();
}	

Monster::~Monster() {
	delete color;
}

void Monster::initialize() {
	currentHP = 0;
	maxHP = 0;
	ac = 0;
	ar = 0;
	reg = 0;
	dr = 0;
	avgdmg = 0;
	dmgvar = 0;
	dmg = 0;
	improvedAC = 0;
	improvedAR = 0;
	improvedREG = 0;
	improvedDR = 0;
	improvedDMG = 0;
	currentDegrees = 0.0f;
	moving = 0;
	movingFrame = 0;
	attacked = false;
	attackSuccessful = false;
	attackedFrame = 0;
	attacking = false;
	attackingFrame = 0;
	attackingDirection = 0;
	dead = false;
}

void Monster::generate() { //generate monster based on given level
	//generate stats based on level

	int random;

	float startRed = 0.3f;
	float endRed = 1.0f;
	float startGreen = 0.5f;
	float endGreen = 0.0f;
	float startBlue = 0.5f;
	float endBlue = 0.0f;
	int levelMax = 7; //where the end color is reached

	if (!color) {
		int diff = levelMax - level - 1;
		float percentOfEnd = (float)level / (float)levelMax;
		float percentOfStart = 1.0f - percentOfEnd;

		color = new SColor(
			percentOfStart * startRed + percentOfEnd * endRed,
			percentOfStart * startGreen + percentOfEnd * endGreen,
			percentOfStart * startBlue + percentOfEnd * endBlue);
	}

	//generate poly info
	stacks = Utility::generateRandomInt(MONSTER_POLY_MAX - MONSTER_POLY_MIN) + MONSTER_POLY_MIN;
	slices = Utility::generateRandomInt(MONSTER_POLY_MAX - MONSTER_POLY_MIN) + MONSTER_POLY_MIN;

	rotationX = Utility::generateRandomFloat(2.0f) - 1.0f;
	rotationY = Utility::generateRandomFloat(2.0f) - 1.0f;
	rotationZ = Utility::generateRandomFloat(2.0f) - 1.0f;

	//generate rotation info
	if (dragon) {
		degrees = DRAGON_SPIN;
	}
	else {
		degrees = (Utility::generateRandomFloat(MONSTER_SPIN_MAX - MONSTER_SPIN_MIN) + MONSTER_SPIN_MIN) * (float)level;
	}

	random = Utility::generateRandomInt(level);

	currentHP = 10 * level + random;
	maxHP = currentHP;

	ac = 10 + 2 * level;
	ar = 3 + 2 * level;
	
	avgdmg = 2 * level + 1;
	dmgvar = 1 + Utility::generateRandomInt(level);

	//a level X monster has X "stat points"
	//a stat point will be randomly spent on either an additional AC, an additional AR, +1 Regeneration, +1 DR, or +1 dmg
	for (int i = 0; i < level; i++) {
		random = Utility::generateRandomInt(4);

		switch (random) {
			case 0:
				ac++;
				improvedAC++;
				break;
			case 1:
				ar++;
				improvedAR++;
				break;
			case 2:
				reg++;
				improvedREG++;
				break;
			case 3:
				dr++;
				improvedDR++;
				break;
			case 4:
				dmg++;
				improvedDMG++;
				break;
			default:
				cout << "Bad randon int.  Monster::generate()" << endl;
				break;
		}
	}

	//set the bonus string
	ostringstream buffer;

	if (improvedAC > 0) {
		buffer << "Armor (" << improvedAC << ") ";
	}
	if (improvedAR > 0) {
		buffer << "Attack (" << improvedAR << ") ";
	}
	if (improvedREG > 0) {
		buffer << "Regeneration (" << improvedREG << ") ";
	}
	if (improvedDR > 0) {
		buffer << "Protection (" << improvedDR << ") ";
	}
	if (improvedDMG > 0) {
		buffer << "Damage (" << improvedDMG << ") ";
	}

	bonus = buffer.str();
}

void Monster::print() {
	cout << name << endl;
	cout << "Level " << level << endl;

	cout << "hp=" << maxHP << endl;
	cout << "ac=" << ac << endl;
	cout << "ar=" << ar << endl;
	cout << "reg=" << reg << endl;
	cout << "dr=" << dr << endl;
	
	cout << "dmg= " << (avgdmg - dmgvar) << " to " << (avgdmg + dmgvar) << " + " << dmg << endl;
	cout << bonus << endl;

	cout << endl;
}	

void Monster::generateName() {
	name = MONSTER_ADJ[Utility::generateRandomInt(MONSTER_ADJ_SIZE - 1)] + " " + MONSTER_STRINGS[Utility::generateRandomInt(MONSTER_STRINGS_SIZE - 1)];
}

string Monster::getName() {
	return name;
}

void Monster::drawInfo() {
	SColor* renderColor;
	ostringstream buffer;
	
	float top = 0.95f;
	float increment = (2.0f * top) / 5.0f;

	const float HEALTH_X = 0.95f;
	const float HEALTH_Y_BOTTOM = top - 3 * increment;
	const float HEALTH_Y_TOP = top - 2 * increment;

	//draw name and title
	buffer << getName() << " (" << level << ")";
	renderColor = new SColor(1.0f, 1.0f, 1.0f);
	renderBitmapString(-0.95f, top - increment, -0.1f, GLUT_BITMAP_HELVETICA_18, buffer.str(), renderColor);
	delete renderColor;

	//draw health bar border
	glBegin(GL_QUADS);
	float diff = 0.02f;
	glColor3f(1.0f, 0.67f, 0.0f);
	
	glVertex3f(-HEALTH_X, HEALTH_Y_BOTTOM, -0.1f);
	glVertex3f(-HEALTH_X, HEALTH_Y_TOP - diff, -0.1f);
	glVertex3f(-HEALTH_X + diff, HEALTH_Y_TOP - diff, -0.1f);
	glVertex3f(-HEALTH_X + diff, HEALTH_Y_BOTTOM, -0.1f);
	
	glVertex3f(-HEALTH_X, HEALTH_Y_TOP - diff, -0.1f);
	glVertex3f(-HEALTH_X, HEALTH_Y_TOP, -0.1f);
	glVertex3f(HEALTH_X, HEALTH_Y_TOP, -0.1f);
	glVertex3f(HEALTH_X, HEALTH_Y_TOP - diff, -0.1f);	
	
	glVertex3f(HEALTH_X - diff, HEALTH_Y_TOP, -0.1f);
	glVertex3f(HEALTH_X, HEALTH_Y_TOP, -0.1f);
	glVertex3f(HEALTH_X, HEALTH_Y_BOTTOM + diff, -0.1f);
	glVertex3f(HEALTH_X - diff, HEALTH_Y_BOTTOM + diff, -0.1f);
	
	glVertex3f(HEALTH_X, HEALTH_Y_BOTTOM + diff, -0.1f);
	glVertex3f(HEALTH_X, HEALTH_Y_BOTTOM, -0.1f);
	glVertex3f(-HEALTH_X, HEALTH_Y_BOTTOM, -0.1f);
	glVertex3f(-HEALTH_X, HEALTH_Y_BOTTOM + diff, -0.1f);
		
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
	//buffer << getCurrentHP() << " / " << getMaxHP();
	//draw % instead of exact figure
	buffer << (int)(((float)getCurrentHP() / (float)getMaxHP()) * 100) << "%";
	renderBitmapString(-0.1f, (HEALTH_Y_BOTTOM + HEALTH_Y_TOP) / 2.0f - 0.02f, -0.11f, GLUT_BITMAP_HELVETICA_12, buffer.str(), renderColor);
	delete renderColor;

	//draw bonus attribute string
	renderColor = new SColor(0.85f, 0.85f, 0.85f);
	renderBitmapString(-0.95f, top - 4 * increment, -0.1f, GLUT_BITMAP_HELVETICA_12, bonus, renderColor);
	delete renderColor;
}

void Monster::renderBitmapString(float x, float y, float z, void *font, string theString, SColor* theColor) { 
	//disable lighting if it was on
	int lightingOn[1];

	glGetIntegerv(GL_LIGHTING, lightingOn);

	if (lightingOn[0] > 0) {
		lightingOn[0] = 1;
		glDisable(GL_LIGHTING);
		//glDisable(GL_LIGHT0);
		//glDisable(GL_COLOR_MATERIAL);		
	}
	
	//glDisable(GL_LIGHTING);
	//glDisable(GL_COLOR_MATERIAL);
	//glDisable(GL_LIGHT0);
		
	//cout << theColor->getR() << "  " << theColor->getG() << "  " << theColor->getB() << endl;
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

	//glEnable(GL_LIGHTING);
	//glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_LIGHT0);
}

int Monster::getCurrentHP() {
	return currentHP;
}

int Monster::getDR() {
	return dr;
}

int Monster::getREG() {
	return reg;
}

int Monster::getMaxHP() {
	return maxHP;
}

int Monster::getAC() {
	return ac;
}

int Monster::getAR() {
	return ar;
}

int Monster::getAvgDmg() {
	return avgdmg;
}

int Monster::getDmgVar() {
	return dmgvar;
}

string Monster::getBonus() {
	return bonus;
}

SColor* Monster::getColor() {
	return color;
}

void Monster::setLocation(BoardLocation* newLocation) {
	location = newLocation;
}

BoardLocation* Monster::getLocation() {
	return location;
}

void Monster::draw(int drawOption) {
	glTranslatef(0.0f, 0.0f, -1.1f);

	//draw attacked animation below the player piece, if under attack
	if (attacked) {
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

	glTranslatef(0.0f, 0.0f, -0.1f);

	//draw attacking animation if the player is attacking
	if (attacking) {
		glPushMatrix();

		glColor3f(color->getR(), color->getG(), color->getB());
			
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

	glColor3f(0.9f, 0.9f, 0.9f);
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

	//draw monster level and health %
	if (drawOption > 0) {
		ostringstream buffer;

		//cout << "Monster DRAW INFO" << endl;
		//cout << color->getR() << "  " << color->getG() << "  " << color->getB() << endl;

		buffer << getLevel() << "  " << (int)(((float)getCurrentHP() / (float)getMaxHP()) * 100.0f) << "%";
		renderBitmapString(-0.98f, -0.98f, -0.1f, GLUT_BITMAP_HELVETICA_10, buffer.str(), color);
	}

	//draw health bar if injured 
	if (isInjured()) {
		float x1 = -1.0f;
		float y1 = 1.15f;
		float x2 = 1.0f;
		float y2 = 0.75f;

		glDisable(GL_LIGHTING);
		glColor4f(0.65f, 0.0f, 0.0f, 1.0f);

		float width = (x2 - x1) * getHealthPercent();
		glRectf(x1, y1, x1 + width, y2);

		glEnable(GL_LIGHTING);
	}

	glColor3f(color->getR(), color->getG(), color->getB());
	glRotatef(currentDegrees, rotationX, rotationY, rotationZ);

	if (dragon) {
		//glutSolidDodecahedron();
		//glutSolidTetrahedron();
		//glutSolidOctahedron();
		//glutSolidTeapot(1);
		glScalef(1.25f, 1.25f, 1.25f);
		glutSolidIcosahedron();
	}
	else {
		//glutSolidCube(1.4f);
		//glutSolidTorus(0.4f, 0.7f, 16, 16);
		//glutSolidCone(0.7f, 1.4f, 16, 16);  //looks stupid

		//glScalef(0.6f, 0.6f, 0.6f);
		//glutSolidDodecahedron();
		glutSolidSphere(0.95f, slices, stacks);
	}
}	

void Monster::frameUpdate(float deltaTime) {
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

void Monster::signalMoving(int direction) {
	moving = direction;
	movingFrame = 0;
}

int Monster::getLevel() {
	return level;
}

int Monster::getRandomDmg() {
	//	int getAvgDmg();
	//  int getDmgVar();

	int dmgModifier = Utility::generateRandomInt(getDmgVar() * 2) - getDmgVar();

	//average damage + random range modifier + bonus damage
	return getAvgDmg() + dmgModifier + dmg;
}

void Monster::changeHealthBy(int changeBy) {
	currentHP += changeBy;
}

void Monster::signalAttacked(bool success) {
	attackedFrame = 0;
	
	attacked = true;

	if (!attackSuccessful) {
		attackSuccessful = success;
	}
}

void Monster::signalAttacking(int direction) {
	attacking = true;
	attackingFrame = 0;
	attackingDirection = direction;
}	

void Monster::signalDead() {
	//remove monster from current location
	getLocation()->setOccupied(false);
	getLocation()->setMonsterNum(-1);
	dead = true;
}

bool Monster::isDragon() {
	return dragon;
}

bool Monster::getDead() {
	return dead;
}

//returns the random level of a randomly generated item dropped by this monster.  The level is close to the monster's level
int Monster::generateItemLevel() {
	float random = Utility::generateRandomFloat(1.0f);
	int itemLevel;

	/*
	if (random <= 0.1f) {
		itemLevel = level - 2;
	}
	*/
	if (random <= 0.25f) {
		itemLevel = level - 1;
	}
	else if (random <= 0.7f) {
		itemLevel = level;
	}
	else if (random <= 0.9f) {
		itemLevel = level + 1;
	}
	else {
		itemLevel = level + 2;
	}

	if (itemLevel < 1) {
		itemLevel = 1;
	}

	return itemLevel;
}

void Monster::turnUpdate() {
	currentHP += getREG();

	if (currentHP > maxHP) {
		currentHP = maxHP;
	}
}