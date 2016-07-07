#include "Board.h"

//create new board with random player name
Board::Board() {
	thePlayer = new Player();

	initialize();
}

//create new board with given player name!
Board::Board(string playerName) {
	thePlayer = new Player(playerName);

	initialize();
}

//deallocate everythning
Board::~Board() {
	//cout << "board destroyed.." << endl;

	//deallocate all board locations
	for (int x = 0; x < BOARD_SIZE; x++) {
		for (int y = 0; y < BOARD_SIZE; y++) {
			//cout << "deleting location " << x << " " << y << endl;
			delete locations[x][y];
		}
	}

	delete thePlayer;

	delete mountains;
	delete lake;
	delete grass;
	delete plains;

	for (unsigned int i = 0; i < monsters.size(); i++) {
		delete monsters.at(i);
	}

	delete theMessageList;

	delete playerTakeDamageColor;
	delete monsterMissesColor;
	delete playerHitMonsterColor;
	delete playerMissMonsterColor;
	delete playerKillsMonsterColor;
	delete playerPassesColor;
}

void Board::initialize() {
	//testPlayer();
	//testEffect2();
	//testMonster();
	//testItem();
	//testBoardLocation();

	gameOver = false;
	playerWin = false;
	//set base terrain colors

	mountains = new SColor(0.5f, 0.23f, 0.1f);
	grass = new SColor(0.29f, 0.44f, 0.14f); //old grass color
	plains = new SColor(0.86f, 0.86f, 0.44f); //old plains color
    lake = new SColor(0.0f, 0.28f, 0.98f); //old lake color

	playerTakeDamageColor = new SColor(0.96f, 0.11f, 0.10f);
	monsterMissesColor = new SColor(0.18f, 0.12f, 0.99f);
	playerHitMonsterColor = new SColor(0.11f, 0.99f, 0.08f);
	playerMissMonsterColor = new SColor(0.7f, 0.9f, 0.5f);
	playerKillsMonsterColor = new SColor(0.9f, 0.9f, 0.04f);
	playerPassesColor = new SColor(0.95f, 0.95f, 0.95f);
	
	theMessageList = new MessageList(MESSAGE_LIST_SIZE);

	initializeBoard();	

	addWelcomeMessages();
}

void Board::testMonster() {
	Monster* temp;
	
	//test generate 100 monsters
	for (int i = 0; i < 1000000; i++) {
		temp = new Monster(i % 6 + 1);
		//temp->print();
		delete temp;
	} 
}

void Board::testItem() {
	Item* temp;

	for (int i = 0; i < 10000000; i++) {
		temp = new Item(i % 7);
		//temp->print();
		delete temp;
	}
}

void Board::addWelcomeMessages() {
	Message* messageOne = new Message("  bGame!", new SColor(1.0f, 1.0f, 1.0f));
	Message* messageTwo = new Message("Your quest is to slay the four dragons.", new SColor(1.0f, 1.0f, 1.0f));
	Message* messageThree = new Message("Created by Scott Atkins, 2006", new SColor(0.1f, 0.1f, 0.85f));
	Message* messageFour = new Message("", new SColor(1.0f, 1.0f, 1.0f));
	Message* messageFive = new Message("Press 'H / h' for help.", new SColor(0.9f, 0.9f, 0.9f));
	//Message* messageSix = new Message("", new SColor(1.0f, 1.0f, 1.0f));

	theMessageList->addMessage(messageOne);
	theMessageList->addMessage(messageThree);
	theMessageList->addMessage(messageFour);
	theMessageList->addMessage(messageTwo);
	theMessageList->addMessage(messageFive);
	//theMessageList->addMessage(messageSix);
}

void Board::initializeBoard() {
	monsterCount = 0;
	fog = true;
	cheated = false;
	
	float random;

	list<BoardLocation*> unfinished;

	//first, create all the board locations
	for (int x = 0; x < BOARD_SIZE; x++) {
		for (int y = 0; y < BOARD_SIZE; y++) {
			locations[x][y] = new BoardLocation(x, y, true);
			
			//set all border locations color and impassability
			if ((x == 0) || (y == 0) || (x == (BOARD_SIZE - 1)) || (y == (BOARD_SIZE - 1))) { 
				locations[x][y]->setPassable(false);
				locations[x][y]->setColor(mountains->generateSimilarColor());
				locations[x][y]->setEdge(true);
				locations[x][y]->setType(MOUNTAINS);
			}
			else { //random chance location is assigned a base attribute (mountain, lake, plains, grass)
				locations[x][y]->setEdge(false);

				random = Utility::generateRandomFloat(1.0f);
				
				if (random <= GRASS_CHANCE) { //this square is seeded grass
					locations[x][y]->setColor(grass->generateSimilarColor());
					locations[x][y]->setPassable(true);
					locations[x][y]->setType(GRASS);
					locations[x][y]->setCreated();
				}
				else if (random <= GRASS_CHANCE + LAKE_CHANCE) { //this square is seeded lake
					locations[x][y]->setColor(lake->generateSimilarColor());
					locations[x][y]->setPassable(false);
					locations[x][y]->setType(LAKE);
					locations[x][y]->setCreated();
				}
				else if (random <= GRASS_CHANCE + LAKE_CHANCE + PLAINS_CHANCE) { //this square is seeded plains
					locations[x][y]->setColor(plains->generateSimilarColor());
					locations[x][y]->setPassable(true);
					locations[x][y]->setType(PLAINS);
					locations[x][y]->setCreated();
				}
				else if (random <= GRASS_CHANCE + LAKE_CHANCE + PLAINS_CHANCE + MOUNTAINS_CHANCE) { //this suare is seeded mountains
					locations[x][y]->setColor(mountains->generateSimilarColor());
					locations[x][y]->setPassable(false);
					locations[x][y]->setType(MOUNTAINS);
					locations[x][y]->setCreated();
				}
				else { //else add the item to the list of unfinished board locations
					unfinished.push_back(locations[x][y]);
				}
			}
		}
	}

	//second, connect all the locations together!
	for (int x = 0; x < BOARD_SIZE; x++) {
		for (int y = 0; y < BOARD_SIZE; y++) {
			//connect location x,y

			//connect north
			if (y > 0) {
				locations[x][y]->setNorth(locations[x][y - 1]);
			}

			//connect south
			if (y < (BOARD_SIZE - 1)) {
				locations[x][y]->setSouth(locations[x][y + 1]);
			}

			//connect west
			if (x > 0) {
				locations[x][y]->setWest(locations[x - 1][y]);
			}

			//connect east
			if (x < (BOARD_SIZE - 1)) {
				locations[x][y]->setEast(locations[x + 1][y]);
			}
		}
	}

	BoardLocation* current;

	//deal with the unfinished list
	while (unfinished.size() > 0) {
		current = unfinished.front();
		unfinished.pop_front();

		random = Utility::generateRandomFloat(1.0f);
		
		if (random <= 0.15f) { //location inherits attributes of north location
			if ((current->getNorth()->getCreated()) && (!current->getNorth()->getEdge())) {
				current->setColor(current->getNorth()->getColor()->generateSimilarColor());
				current->setPassable(current->getNorth()->getPassable());
				current->setType(current->getNorth()->getType());
				current->setCreated();
			}
			else {
				unfinished.push_back(current);
			}
		}
		else if (random <= 0.3f) { //location inherits attributes of south location
			if ((current->getSouth()->getCreated()) && (!current->getSouth()->getEdge())) {
				current->setColor(current->getSouth()->getColor()->generateSimilarColor());
				current->setPassable(current->getSouth()->getPassable());
				current->setType(current->getSouth()->getType());
				current->setCreated();
			}
			else {
				unfinished.push_back(current);
			}
		}
		else if (random <= 0.45f) { //location inherits attributes of east location
			if ((current->getEast()->getCreated()) && (!current->getEast()->getEdge())) {
				current->setColor(current->getEast()->getColor()->generateSimilarColor());
				current->setPassable(current->getEast()->getPassable());
				current->setType(current->getEast()->getType());
				current->setCreated();
			}
			else {
				unfinished.push_back(current);
			}
		}
		else if (random <= 0.6f) { //location inherits attributes of west location
			if ((current->getWest()->getCreated()) && (!current->getWest()->getEdge())) {
				current->setColor(current->getWest()->getColor()->generateSimilarColor());
				current->setPassable(current->getWest()->getPassable());
				current->setType(current->getWest()->getType());
				current->setCreated();
			}
			else {
				unfinished.push_back(current);
			}
		}
		else { //location does'nt inherit any attributes at this time
			unfinished.push_back(current);
		}
	}

	//set al board locations random elevation
	for (int x = 0; x < BOARD_SIZE; x++) {
		for (int y = 0; y < BOARD_SIZE; y++) {
			locations[x][y]->initializeTerrain();
		}
	}

	Monster* newMonster;
	int monsterNum = 0;
	
	//place four dragons
	
	//dragon ONE int bottom left quadrant
	newMonster = new Monster(DRAGON_LEVEL, "Green Dragon", true, new SColor(0.1f, 0.9f, 0.1f));
	int xx = 0;
	int yy = 0;
	int quarter = BOARD_SIZE / 4;

	while (!(locations[xx][yy]->getPassable())) {
		xx = (int)((0.25f) * (float)BOARD_SIZE) + (Utility::generateRandomInt((quarter * 2) - 1) - quarter);
		yy = (int)((0.75f) * (float)BOARD_SIZE) + (Utility::generateRandomInt((quarter * 2) - 1) - quarter);
	}

	//place dragon at passable location just found
	newMonster->setLocation(locations[xx][yy]);
	monsters.push_back(newMonster);
	dragons.push_back(newMonster);
	locations[xx][yy]->setMonsterNum(monsterNum);
	locations[xx][yy]->setOccupied(true);
	monsterNum++;
	monsterCount++;

	//dragon TWO in bottom right quadrant
	newMonster = new Monster(DRAGON_LEVEL, "Bronze Dragon", true, new SColor(0.85f, 0.7f, 0.35f));
	
	xx = 0;
	yy = 0;

	while (!(locations[xx][yy]->getPassable())) {
		xx = (int)((0.75f) * (float)BOARD_SIZE) + (Utility::generateRandomInt((quarter * 2) - 1) - quarter);
		yy = (int)((0.75f) * (float)BOARD_SIZE) + (Utility::generateRandomInt((quarter * 2) - 1) - quarter);
	}

	//place dragon at passable location just found
	newMonster->setLocation(locations[xx][yy]);
	monsters.push_back(newMonster);
	dragons.push_back(newMonster);
	locations[xx][yy]->setMonsterNum(monsterNum);
	locations[xx][yy]->setOccupied(true);
	monsterNum++;
	monsterCount++;

	//dragon THREE int top left quadrant
	newMonster = new Monster(DRAGON_LEVEL, "Blue Dragon", true, new SColor(0.1f, 0.1f, 0.9f));
	
	xx = 0;
	yy = 0;
	
	while (!(locations[xx][yy]->getPassable())) {
		xx = (int)((0.25f) * (float)BOARD_SIZE) + (Utility::generateRandomInt((quarter * 2) - 1) - quarter);
		yy = (int)((0.25f) * (float)BOARD_SIZE) + (Utility::generateRandomInt((quarter * 2) - 1) - quarter);
	}

	//place dragon at passable location just found
	newMonster->setLocation(locations[xx][yy]);
	monsters.push_back(newMonster);
	dragons.push_back(newMonster);
	locations[xx][yy]->setMonsterNum(monsterNum);
	locations[xx][yy]->setOccupied(true);
	monsterNum++;
	monsterCount++;

	//dragon FOUR int top right quadrant
	newMonster = new Monster(DRAGON_LEVEL, "Black Dragon", true, new SColor(0.2f, 0.2f, 0.2f));
	
	xx = 0;
	yy = 0;
	
	while (!(locations[xx][yy]->getPassable())) {
		xx = (int)((0.75f) * (float)BOARD_SIZE) + (Utility::generateRandomInt((quarter * 2) - 1) - quarter);
		yy = (int)((0.25f) * (float)BOARD_SIZE) + (Utility::generateRandomInt((quarter * 2) - 1) - quarter);
	}

	//place dragon at passable location just found
	newMonster->setLocation(locations[xx][yy]);
	monsters.push_back(newMonster);
	dragons.push_back(newMonster);
	locations[xx][yy]->setMonsterNum(monsterNum);
	locations[xx][yy]->setOccupied(true);
	monsterNum++;
	monsterCount++;

	//place random monster!
	for (int y = 0; y < BOARD_SIZE; y++) {
		for (int x = 0; x < BOARD_SIZE; x++) {
			//if location is passable, chance monster will be drawn here
			if ((locations[x][y]->getPassable()) && (locations[x][y]->getMonsterNum() < 0)) {
				if (Utility::generateRandomFloat(1.0f) <= MONSTER_CHANCE) {
					
					//initially all monsters are level 1
					newMonster = new Monster(1);

					//newMonster = createAppropriateMonster();
					newMonster->setLocation(locations[x][y]);
					monsters.push_back(newMonster);
					
					//set monsterNum
					locations[x][y]->setMonsterNum(monsterNum);

					//set occupied
					locations[x][y]->setOccupied(true);

					//increase monsterNum
					monsterNum++;

					//increase monster count
					monsterCount++;
				}	
			}
		}
	}
	
	//place the player at a location not already occupied
	while (thePlayer->getLocation() == NULL) {
		xx = Utility::generateRandomInt(BOARD_SIZE - 1);
		yy = Utility::generateRandomInt(BOARD_SIZE - 1);

		if ((!(locations[xx][yy]->getOccupied())) && (locations[xx][yy]->getPassable())) { //check to see if location is occupied and passable
			thePlayer->setLocation(locations[xx][yy]);
			locations[xx][yy]->setOccupied(true);
		}
	}
}

void Board::draw(int drawOption) {
	float squareSize = 2.0f / (float)BOARD_SIZE;
	float halfSquareSize = squareSize / 2;
	float xStart = -1.0f + halfSquareSize;
	float yStart = 1.0f - halfSquareSize;

	//GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    //GLfloat mat_shininess[] = { 15.0 };
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	
	
	float lightingValue; 
	float lightValues[4];

	//draw the monsters
	Monster* thisMonster;

	glMatrixMode(GL_MODELVIEW);

	for (unsigned int i = 0; i < monsters.size(); i++) {
		thisMonster = monsters[i];

		//only draw monsters if they are within light distance, or the player is cheating!!
		if ((!fog) || ((lightingValue = calculateVisionLighting(thisMonster->getLocation()->getX(), thisMonster->getLocation()->getY())) > 0.0f)) {

			if (!fog) {
				lightingValue = 1.0f;
			}

			lightValues[0] = lightingValue;
			lightValues[1] = lightingValue;
			lightValues[2] = lightingValue;
			lightValues[3] = lightingValue;

			if (!thisMonster->getDead()) {
				glLoadIdentity();
				glLightfv(GL_LIGHT0, GL_DIFFUSE, lightValues);
				glLightfv(GL_LIGHT0, GL_SPECULAR, lightValues);
				glTranslatef(xStart + (thisMonster->getLocation()->getX() * squareSize), yStart - thisMonster->getLocation()->getY() * squareSize, 0);
				glScalef(halfSquareSize, halfSquareSize, halfSquareSize);
				
				monsters[i]->draw(drawOption);
			}
		}
	}

	//draw the board squares
	for (int y = 0; y < BOARD_SIZE; y++) {
		for (int x = 0; x < BOARD_SIZE; x++) {
			
			//only draw locations if they are within light distance, or they are cheating!
			if ((!fog) || ((lightingValue = calculateVisionLighting(x, y)) > 0.0f)) {
				
				if (!fog) {
					lightingValue = 1.0f;
				}

				lightValues[0] = lightingValue;
				lightValues[1] = lightingValue;
				lightValues[2] = lightingValue;
				lightValues[3] = lightingValue;
				
				glLoadIdentity();
				glLightfv(GL_LIGHT0, GL_DIFFUSE, lightValues);
				glLightfv(GL_LIGHT0, GL_SPECULAR, lightValues);
				glTranslatef(xStart + (x * squareSize), (yStart - y * squareSize), 0);
				glScalef(halfSquareSize, halfSquareSize, halfSquareSize);
				locations[x][y]->draw();
			}
		}
	}

	lightValues[0] = 1.0f;
	lightValues[1] = 1.0f;
	lightValues[2] = 1.0f;
	lightValues[3] = 1.0f;
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightValues);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightValues);
				

	//draw the player
	glLoadIdentity();
	glTranslatef(xStart + (thePlayer->getLocation()->getX() * squareSize), yStart - (thePlayer->getLocation()->getY() * squareSize), 0);
	glScalef(halfSquareSize, halfSquareSize, halfSquareSize);
	thePlayer->draw(drawOption);

	glLoadIdentity();

	//draw gameover screen if game is over
	if (gameOver) {
		drawGameOver();		
		return;
	}

	if (thePlayer->getNewItemAvailable()) {
		glPushMatrix();
		//glScalef(1.0f, 1.0f, 0.05f);
		glScalef(0.5f, 0.15f, 0.05f);
		glTranslatef(0.0f, (0.5f - PLAYER_INFO_SIZE), -6.0f);
		//glScalef(2.0f, 1.0f, 1.0f);
		thePlayer->getNewItem()->drawNewItem();
		glPopMatrix();
	}
}

void Board::drawGameOver() {
	glPushMatrix();
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glScalef(0.8f, 0.5f, 0.05f);
	glTranslatef(0.0f, 0.0f, -10.0f);
	
	//draw background
	glBegin(GL_QUADS);
	glColor4f(0.1f, 0.1f, 0.1f, 0.85f);
	glVertex3f(-1.0f, -1.0f, 0.12f);
	glVertex3f(-1.0f, 1.0f, 0.12f);
	glVertex3f(1.0f, 1.0f, 0.12f);
	glVertex3f(1.0f, -1.0f, 0.12f);
		
	//draw outer border
	glColor3f(0.8f, 0.4f, 0.11f);      
	glVertex3f(-1.0f, -1.0f, 0.1f);
	glVertex3f(-0.98f, -1.0f, 0.1f);
	glVertex3f(-0.98f, 0.98f, 0.1f);
	glVertex3f(-1.0f, 0.98f, 0.1f);
	glVertex3f(-1.0f, 0.98f, 0.1f);
	glVertex3f(-1.0f, 1.0f, 0.1f);
	glVertex3f(0.98f, 1.0f, 0.1f);
	glVertex3f(0.98f, 0.98f, 0.1f);	
	glVertex3f(0.98f, 1.0f, 0.1f);
	glVertex3f(1.0f, 1.0f, 0.1f);
	glVertex3f(1.0f, -0.98f, 0.1f);
	glVertex3f(0.98f, -0.98f, 0.1f);
	glVertex3f(1.0f, -0.98f, 0.1f);
	glVertex3f(1.0f, -1.0f, 0.1f);
	glVertex3f(-0.98f, -1.0f, 0.1f);
	glVertex3f(-0.98f, -0.98f, 0.1f);

	//draw inner border
	glColor3f(0.91f, 0.44f, 0.13f);
	glVertex3f(-0.98f, -0.98f, 0.1f);
	glVertex3f(-0.97f, -0.98f, 0.1f);
	glVertex3f(-0.97f, 0.97f, 0.1f);
	glVertex3f(-0.98f, 0.97f, 0.1f);
	glVertex3f(-0.98f, 0.97f, 0.1f);
	glVertex3f(-0.98f, 0.98f, 0.1f);
	glVertex3f(0.97f, 0.98f, 0.1f);
	glVertex3f(0.97f, 0.97f, 0.1f);	
	glVertex3f(0.97f, 0.98f, 0.1f);
	glVertex3f(0.98f, 0.98f, 0.1f);
	glVertex3f(0.98f, -0.97f, 0.1f);
	glVertex3f(0.97f, -0.97f, 0.1f);
	glVertex3f(0.98f, -0.97f, 0.1f);
	glVertex3f(0.98f, -0.98f, 0.1f);
	glVertex3f(-0.97f, -0.98f, 0.1f);
	glVertex3f(-0.97f, -0.97f, 0.1f);		
	glEnd();

	glDisable(GL_BLEND);

	SColor* temp;

	temp = new SColor(0.9f, 0.9f, 0.9f);
	
	string endMessage;

	if (playerWin) {
		endMessage = "Congratulations!  You have defeated the four Dragons!";
	}
	else {
		endMessage = "You have failed to defeat the four Dragons.";
	}

	renderBitmapString(-0.95f, 0.85f, -0.1f, GLUT_BITMAP_TIMES_ROMAN_24, endMessage, temp);

	endMessage = "You have not made it into the highscores.";

	renderBitmapString(-0.95f, 0.3f, -0.1f, GLUT_BITMAP_HELVETICA_18, endMessage, temp);

	//draw player score info in the middle

    renderBitmapString(-0.95f, -0.3f, -0.1f, GLUT_BITMAP_HELVETICA_18, "Do you wish to play again?  (Y / N)" , temp);

	delete temp;

	glPopMatrix();
}

MessageList* Board::getMessageList() {
	return theMessageList;
}

void Board::testBoardLocation() {
	BoardLocation* temp;
	
	for (int i = 0; i < 9999999; i++) {
		//cout << "test board location" << endl;
		temp = new BoardLocation(4, 1, false);
		temp->addEffect(new Effect(MONSTER_DEATH, 3, new SColor(1, 1, 1)));
		delete temp;
	}
}

void Board::testPlayer() {
	
	for (int i = 0; i < 100000; i++) {
		delete thePlayer;
		thePlayer = new Player();
	}
}

Player* Board::getPlayer() {
	return thePlayer;
}

void Board::frameUpdate() {
	if (getGameOver() && (thePlayer->getLIGHT() < (BOARD_SIZE * 2))) {
		thePlayer->setLight(thePlayer->getLIGHT() + 1);
	}

	//randomly kill off monsters until they are all dead
	if (getGameOver() && (Utility::generateRandomFloat(1.0f) < 0.1f)) {
		Monster* target = monsters.at(Utility::generateRandomInt((int)monsters.size() - 1));

		if (!target->getDead()) {
			target->getLocation()->addEffect(new Effect(MONSTER_DEATH, (target->getLevel() * Utility::generateRandomInt(5)), target->getColor()->generateSameColor()));
			target->signalDead();
		}
	}

	//update the monsters
	for (unsigned int i = 0; i < monsters.size(); i++) {
		monsters[i]->frameUpdate();
	}

	//update the player
	thePlayer->frameUpdate();

	for (int x = 0; x < BOARD_SIZE; x++) {
		for (int y = 0; y < BOARD_SIZE; y++) {
			locations[x][y]->frameUpdate();
		}
	}
}	

void Board::drawMonsterInfo(int x, int y) {
	if (locations[x][y]->getMonsterNum() >= 0) { //if monsterNum is >= 0, then there is a monster
		drawInfoBorder();
		monsters[locations[x][y]->getMonsterNum()]->drawInfo();
	}

	if ((locations[x][y]->getMonsterNum() == -1) && (locations[x][y]->getOccupied())) { //if monsterNum == -1 and occupied, player is there
		drawInfoBorder();
		thePlayer->drawPlayerInfo();
	}
}

void Board::updateMonsters() {
	//cout << "monsters size = " << monsters.size() << endl;
	//cout << "Dragons size = " << dragons.size() << endl;

	for (unsigned int i = 0; i < monsters.size(); i++) {		
		updateMonster(monsters.at(i));
		monsters.at(i)->turnUpdate();
	}
}

void Board::updateMonster(Monster *thisMonster) {
	//do nothing if the mosnter is dead
	if (thisMonster->getDead()) {
		return;
	}
	//cout << "Updating " << thisMonster->getName() << endl;

	//FIRST, if monster can attack teh player, it does so
	if (int direction = checkIfPlayerNextToMonster(thisMonster)) {
		//cout << "    Player is " << direction << " of the monster ^" << endl;

		monsterAttack(thisMonster, direction);
		return;
	}

	//SECOND, if monster can move towards a player, it does so
	if (int direction = checkIfPlayerNearToMonster(thisMonster)) {
		//cout << "    Player is *near* " << direction << " of the monster ^" << endl;
		
		moveMonster(thisMonster, direction);
		return;
	}

	//THIRDLY, if the monster cannot attack or move towards a player, it has a chance to move randomly
	moveMonster(thisMonster, findRandomMonsterMove(thisMonster));
}

void Board::moveMonster(Monster* thisMonster, int direction) {
	//cout << "    " << thisMonster->getName() << " is moving " << direction << "." << endl;

	//return if monster not moving
	if (direction == 0) {
		return;
	}

	//signal the monster is moving
	thisMonster->signalMoving(direction);
	
	//set the monster to the new location, and remove it from its old
	BoardLocation* currentLocation = thisMonster->getLocation();
	int monsterNum = currentLocation->getMonsterNum();

	//remove monster info from current location
	currentLocation->setOccupied(false);
	currentLocation->setMonsterNum(-1);

	//add monster info to new location
	thisMonster->setLocation(currentLocation->getDirection(direction));
	thisMonster->getLocation()->setOccupied(true);	
	thisMonster->getLocation()->setMonsterNum(monsterNum);
}	

//returns a random direction to move;  if ZERO is returned, then no move is possible
int Board::findRandomMonsterMove(Monster* thisMonster) {
	//first check to see if monster will move (MONSTER_RANDOM_MOVE chance)
	if (Utility::generateRandomFloat(1.0f) > MONSTER_RANDOM_MOVE) {
		//monster will not move
		return 0;
	}

	vector<int> openLocations;

	if (thisMonster->getLocation()->getNorth()->getAvailable()) {
		openLocations.push_back(1);
	}

	if (thisMonster->getLocation()->getSouth()->getAvailable()) {
		openLocations.push_back(2);
	}

	if (thisMonster->getLocation()->getEast()->getAvailable()) {
		openLocations.push_back(3);
	}

	if (thisMonster->getLocation()->getWest()->getAvailable()) {
		openLocations.push_back(4);
	}

	//if no moves are available
	if (openLocations.size() == 0) {
		return 0;
	}	
	else return openLocations.at(Utility::generateRandomInt((int)openLocations.size() - 1));
}

int Board::checkIfPlayerNextToMonster(Monster *thisMonster) {
	int playerX = thePlayer->getLocation()->getX();
	int playerY = thePlayer->getLocation()->getY();
	
	if (thePlayer->getLocation()->getX() == thisMonster->getLocation()->getX()) {
		if ((thePlayer->getLocation()->getY() - thisMonster->getLocation()->getY()) == 1) {
			//player is SOUTH of the monster
			return SOUTH;
		}

		if ((thePlayer->getLocation()->getY() - thisMonster->getLocation()->getY()) == -1) {
			//player is NORTH of ths monster
			return NORTH;
		}
	}

	if (thePlayer->getLocation()->getY() == thisMonster->getLocation()->getY()) {
		if ((thePlayer->getLocation()->getX() - thisMonster->getLocation()->getX()) == 1) {
			//player is EAST of the monster
			return EAST;
		}

		if ((thePlayer->getLocation()->getX() - thisMonster->getLocation()->getX()) == -1) {
			//player is WEST of ths monster
			return WEST;
		}
	}

	//otherwise, player is not next to the monster
	return 0;
}

int Board::checkIfPlayerNearToMonster(Monster* thisMonster) {
	int playerX = thePlayer->getLocation()->getX();
	int playerY = thePlayer->getLocation()->getY();
	int monsterX = thisMonster->getLocation()->getX();
	int monsterY = thisMonster->getLocation()->getY();

	//eight possible locations for player to be
	if (monsterX == playerX) {
		//location FIVE
		if (monsterY + 2 == playerY) {
			if (thisMonster->getLocation()->getSouth()->getAvailable()) {
				return SOUTH;
			}
		}

		//location ONE
		if (monsterY - 2 == playerY) {
			if (thisMonster->getLocation()->getNorth()->getAvailable()) {
				return NORTH;
			}
		}
	}

	if (monsterY == playerY) {
		//location THREE
		if (monsterX + 2 == playerX) {
			if (thisMonster->getLocation()->getEast()->getAvailable()) {
				return EAST;
			}
		}	
	
		//location SEVEN
		if (monsterX - 2 == playerX) {
			if (thisMonster->getLocation()->getWest()->getAvailable()) {
				return WEST;
			}
		}
	}

	if (monsterX + 1 == playerX) {
		//location FOUR
		if (monsterY + 1 == playerY) {
			if (thisMonster->getLocation()->getEast()->getAvailable() && thisMonster->getLocation()->getSouth()->getAvailable()) {
				int random = Utility::generateRandomInt(1);

				if (random == 0) {
					return EAST;
				}
				else if (random == 1) {
					return SOUTH;
				}
				else {
					cout << "Bad Random Value - Board::checkIfPlayerNearToMonster" << endl;
				}
			}

			if (!thisMonster->getLocation()->getEast()->getAvailable() && thisMonster->getLocation()->getSouth()->getAvailable()) {
				return SOUTH;
			}

			if (thisMonster->getLocation()->getEast()->getAvailable() && !thisMonster->getLocation()->getSouth()->getAvailable()) {
				return EAST;
			}
		}

		//location TWO
		if (monsterY - 1 == playerY) {
			if (thisMonster->getLocation()->getEast()->getAvailable() && thisMonster->getLocation()->getNorth()->getAvailable()) {
				int random = Utility::generateRandomInt(1);

				if (random == 0) {
					return EAST;
				}
				else if (random == 1) {
					return NORTH;
				}
				else {
					cout << "Bad Random Value - Board::checkIfPlayerNearToMonster" << endl;
				}
			}

			if (!thisMonster->getLocation()->getEast()->getAvailable() && thisMonster->getLocation()->getNorth()->getAvailable()) {
				return NORTH;
			}

			if (thisMonster->getLocation()->getEast()->getAvailable() && !thisMonster->getLocation()->getNorth()->getAvailable()) {
				return EAST;
			}
		}
	}

	if (monsterX - 1 == playerX) {

		//location SIX
		if (monsterY + 1 == playerY) {
			if (thisMonster->getLocation()->getWest()->getAvailable() && thisMonster->getLocation()->getSouth()->getAvailable()) {
				int random = Utility::generateRandomInt(1);

				if (random == 0) {
					return WEST;
				}
				else if (random == 1) {
					return SOUTH;
				}
				else {
					cout << "Bad Random Value - Board::checkIfPlayerNearToMonster" << endl;
				}
			}

			if (!thisMonster->getLocation()->getWest()->getAvailable() && thisMonster->getLocation()->getSouth()->getAvailable()) {
				return SOUTH;
			}

			if (thisMonster->getLocation()->getWest()->getAvailable() && !thisMonster->getLocation()->getSouth()->getAvailable()) {
				return WEST;
			}
		}

		//location EIGHT
		if (monsterY - 1 == playerY) {
			if (thisMonster->getLocation()->getWest()->getAvailable() && thisMonster->getLocation()->getNorth()->getAvailable()) {
				int random = Utility::generateRandomInt(1);

				if (random == 0) {
					return WEST;
				}
				else if (random == 1) {
					return NORTH;
				}
				else {
					cout << "Bad Random Value - Board::checkIfPlayerNearToMonster" << endl;
				}
			}

			if (!thisMonster->getLocation()->getWest()->getAvailable() && thisMonster->getLocation()->getNorth()->getAvailable()) {
				return NORTH;
			}

			if (thisMonster->getLocation()->getWest()->getAvailable() && !thisMonster->getLocation()->getNorth()->getAvailable()) {
				return WEST;
			}
		}
	}
	
	//otherwise, player is not near to the monster
	return 0;
}	

void Board::monsterAttack(Monster* thisMonster, int direction) {
	//cout << "    " << thisMonster->getName() << " is attacking " << direction << "." << endl;
	//thisMonster->print();

	thisMonster->signalAttacking(direction);

	//generate monster attack roll: 1 - 20
	int monsterRoll = Utility::generateRandomInt(19) + 1; 	

	//cout << "monsterRoll = " << monsterRoll << endl;
	
	int modifiedRoll = monsterRoll + thisMonster->getAR();

	//cout << "modifiedRoll = " << modifiedRoll << endl;

	//check to see if the monster hits the player, natural 20 always hits, natural 1 always misses
	if (((modifiedRoll >= thePlayer->getAC()) && (monsterRoll != 1)) || (monsterRoll == 20)) {
		//cout << "Monster Hits" << endl;

		int damage = thisMonster->getRandomDmg();
		
		damage = damage - thePlayer->getDR();

		//minimum damage caused on a successful hit is 1
		if (damage < 1) {
			damage = 1;
		}

		//cout << "MONSTER ACTUAL DMG = " << damage << endl;

		thePlayer->changeHealthBy(-damage);
		thePlayer->signalAttacked(true);

		ostringstream buffer;

		buffer << thisMonster->getName() << " " << getRandomAttackVerb() << "s you for " << damage << " damage.";

		theMessageList->addMessage(new Message(buffer.str(), playerTakeDamageColor->generateSameColor()));

		if (thePlayer->isDead()) {
			//add player dead effect
			thePlayer->getLocation()->addEffect(new Effect(MONSTER_DEATH, 42, thePlayer->getColor()->generateSameColor()));
			
			gameOver = true;			
		}	
		else {
			//add player hit effect
			thePlayer->getLocation()->addEffect(new Effect(MONSTER_DEATH, 0, thePlayer->getColor()->generateSameColor()));
		}
	}
	else {
		//cout << "Monster Misses" << endl;

		thePlayer->signalAttacked(false);

		ostringstream buffer;

		buffer << thisMonster->getName() << " " << getRandomAttackVerb() << "s at you and misses.";

		theMessageList->addMessage(new Message(buffer.str(), monsterMissesColor->generateSameColor()));
	}
}

void Board::playerAttack(int direction) {
	//cout << "Player attacking " << direction << endl;

	thePlayer->signalAttacking(direction);

	Monster* target = monsters.at(thePlayer->getLocation()->getDirection(direction)->getMonsterNum());

	//cout << "TARGET: " << endl;
	//target->print();

	int playerRoll = Utility::generateRandomInt(19) + 1;

	int modifiedRoll = playerRoll + thePlayer->getAR();

	//see if player hits;  natural 20 always hits, natural 1 always misses
	if (((modifiedRoll >= target->getAC()) && (playerRoll != 1)) || (playerRoll == 20)) {
		//cout << "Player HITS" << endl;

		int damage = thePlayer->getRandomDmg();

		damage = damage - target->getDR();

		//minimum damage on successful hit is 1
		if (damage < 1) {
			damage = 1;
		}

		//cout << "PLAYER ACTUAL DMG = " << damage << endl;

		target->changeHealthBy(-damage);
		target->signalAttacked(true);

		ostringstream buffer;

		buffer << "You " << getRandomAttackVerb() << " the " << target->getName() << " for " << damage << " damage.";

		theMessageList->addMessage(new Message(buffer.str(), playerHitMonsterColor->generateSameColor()));

		//cout << "Player attack message added" << endl;

		//if the target is dead..
		if (target->getCurrentHP() <= 0) {
			thePlayer->incrementKillCount();

			//decrement monsterCount
			monsterCount--;

			if (target->isDragon()) {
				thePlayer->incrementDragonsSlain();
			}

			//monster vector stil contains the monster, but the monster is flagged as dead
			//if monster is removed from vector, all the other monster nums get messed up
			//not the best system, but I don't want to change everything now lol

			//start MONSTER_DEATH effect at monster board location
			if (target->isDragon()) {
				//big explosion if dragon
				target->getLocation()->addEffect(new Effect(MONSTER_DEATH, 42, target->getColor()->generateSameColor()));

				//check if player has won!
				if (thePlayer->getDragonsSlain() >= 4) {
					gameOver = true;
					playerWin = true;					
				}
			}
			else {
				target->getLocation()->addEffect(new Effect(MONSTER_DEATH, target->getLevel(), target->getColor()->generateSameColor()));
			}

			target->signalDead();

			//determine if the monster drops an item
			//dragons always drop items
			if ((target->isDragon()) || (Utility::generateRandomFloat(1.0f) <= ITEM_DROP_CHANCE)) {
				//an item drops!!

				thePlayer->signalNewItemAvailable(new Item(target->generateItemLevel()));
			}

			//add death message to message list
			buffer.str("");
			buffer.clear();

			buffer << "You have slain the " << target->getName() << "." << endl;

			theMessageList->addMessage(new Message(buffer.str(), playerKillsMonsterColor->generateSameColor()));
		}
		else { //target is not dead, but still add a small dmg effect
			target->getLocation()->addEffect(new Effect(MONSTER_DEATH, 0, target->getColor()->generateSameColor()));
		}
	}	else {
		//cout << "Player MISS" << endl;

		target->signalAttacked(false);

		ostringstream buffer;

		buffer << "You " << getRandomAttackVerb() << " at the " << target->getName() << " and miss.";

		theMessageList->addMessage(new Message(buffer.str(), playerMissMonsterColor->generateSameColor()));
	}
}

bool Board::movePlayer(int direction) {
	//cout << "movePlayer() " << direction << endl;

	//player cannot take an action until they decide whether to keep the new item or not
	//player cannot take an action if they are dead  :P
	if ((thePlayer->getNewItemAvailable()) || (thePlayer->isDead())) {
		return false;
	}

	bool moved = false;
	
	BoardLocation* currentLocation = thePlayer->getLocation();

	if (direction == NORTH) {
		//attack monster!
		if (currentLocation->getNorth()->getOccupied()) {
			playerAttack(NORTH);

			moved = true;
		}
		else if (((currentLocation->getNorth()->getPassable()) && (!(currentLocation->getNorth()->getOccupied()))) || ((currentLocation->getNorth()->getType() == LAKE) && (thePlayer->getWW() > 0))) {
			thePlayer->setLocation(currentLocation->getNorth());
			thePlayer->signalMoving(NORTH);

			currentLocation->setOccupied(false);
			thePlayer->getLocation()->setOccupied(true);

			moved = true;
		}
	}

	if (direction == SOUTH) {
		//attack monster!
		if (currentLocation->getSouth()->getOccupied()) {
			playerAttack(SOUTH);

			moved = true;
		}
		else if (((currentLocation->getSouth()->getPassable()) && (!(currentLocation->getSouth()->getOccupied()))) || ((currentLocation->getSouth()->getType() == LAKE) && (thePlayer->getWW() > 0))){
			thePlayer->setLocation(currentLocation->getSouth());
			thePlayer->signalMoving(SOUTH);

			currentLocation->setOccupied(false);
			thePlayer->getLocation()->setOccupied(true);

			moved = true;
		}
	}

	if (direction == EAST) {
		//attack monster!
		if (currentLocation->getEast()->getOccupied()) {
			playerAttack(EAST);

			moved = true;
		}
		else if (((currentLocation->getEast()->getPassable()) && (!(currentLocation->getEast()->getOccupied()))) || ((currentLocation->getEast()->getType() == LAKE) && (thePlayer->getWW() > 0))) {
			thePlayer->setLocation(currentLocation->getEast());
			thePlayer->signalMoving(EAST);

			currentLocation->setOccupied(false);
			thePlayer->getLocation()->setOccupied(true);

			moved = true;
		}
	}

	if (direction == WEST) {
		//attack monster!
		if (currentLocation->getWest()->getOccupied()) {
			playerAttack(WEST);

			moved = true;
		}
		else if (((currentLocation->getWest()->getPassable()) && (!(currentLocation->getWest()->getOccupied()))) || ((currentLocation->getWest()->getType() == LAKE) && (thePlayer->getWW() > 0))) {
			thePlayer->setLocation(currentLocation->getWest());
			thePlayer->signalMoving(WEST);

			currentLocation->setOccupied(false);
			thePlayer->getLocation()->setOccupied(true);

			moved = true;
		}
	}

	return moved;
}

void Board::drawInfoBorder() {
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	//draw outer border
	glColor3f(0.8f, 0.4f, 0.11f);      
	glVertex3f(-1.0f, -1.0f, -0.1f);
	glVertex3f(-0.98f, -1.0f, -0.1f);
	glVertex3f(-0.98f, 0.98f, -0.1f);
	glVertex3f(-1.0f, 0.98f, -0.1f);
	glVertex3f(-1.0f, 0.98f, -0.1f);
	glVertex3f(-1.0f, 1.0f, -0.1f);
	glVertex3f(0.98f, 1.0f, -0.1f);
	glVertex3f(0.98f, 0.98f, -0.1f);	
	glVertex3f(0.98f, 1.0f, -0.1f);
	glVertex3f(1.0f, 1.0f, -0.1f);
	glVertex3f(1.0f, -0.98f, -0.1f);
	glVertex3f(0.98f, -0.98f, -0.1f);
	glVertex3f(1.0f, -0.98f, -0.1f);
	glVertex3f(1.0f, -1.0f, -0.1f);
	glVertex3f(-0.98f, -1.0f, -0.1f);
	glVertex3f(-0.98f, -0.98f, -0.1f);

	//draw inner border
	glColor3f(0.91f, 0.44f, 0.13f);
	glVertex3f(-0.98f, -0.98f, -0.1f);
	glVertex3f(-0.97f, -0.98f, -0.1f);
	glVertex3f(-0.97f, 0.97f, -0.1f);
	glVertex3f(-0.98f, 0.97f, -0.1f);
	glVertex3f(-0.98f, 0.97f, -0.1f);
	glVertex3f(-0.98f, 0.98f, -0.1f);
	glVertex3f(0.97f, 0.98f, -0.1f);
	glVertex3f(0.97f, 0.97f, -0.1f);	
	glVertex3f(0.97f, 0.98f, -0.1f);
	glVertex3f(0.98f, 0.98f, -0.1f);
	glVertex3f(0.98f, -0.97f, -0.1f);
	glVertex3f(0.97f, -0.97f, -0.1f);
	glVertex3f(0.98f, -0.97f, -0.1f);
	glVertex3f(0.98f, -0.98f, -0.1f);
	glVertex3f(-0.97f, -0.98f, -0.1f);
	glVertex3f(-0.97f, -0.97f, -0.1f);	
	glEnd();
}

string Board::getRandomAttackVerb() {
	return ATTACK_VERBS[Utility::generateRandomInt(ATTACK_VERBS_SIZE - 1)];
}

void Board::testEffect() {
	//cout << "testEffect() Start" << endl;
	int x = (Utility::generateRandomInt(BOARD_SIZE - 1));
	int y = (Utility::generateRandomInt(BOARD_SIZE - 1));

	locations[x][y]->addEffect(new Effect(MONSTER_DEATH, Utility::generateRandomInt(12) + 1, new SColor()));
	//cout << "testEffect() END " << endl;
}

void Board::testLocations() {
	for (int x = 0; x < BOARD_SIZE; x++) {
		for (int y = 0; y < BOARD_SIZE; y++) {
			cout << "size " << locations[x][y]->getEffectsSize() << endl;
		}
	}
}

void Board::testEffect2() {
	Effect* temp;
	for (int i = 0; i < 100000; i++) {
		temp = new Effect(MONSTER_DEATH, 4, new SColor());
		delete temp;
	}
}

bool Board::playerPasses() {
	theMessageList->addMessage(new Message("You rest for a moment.", playerPassesColor->generateSameColor()));

	return true;
}

//creates a monster near the players avg equipment level!
Monster* Board::createAppropriateMonster() {
	//create a monster of the players average equipment level, or one level higher
	int newLevel = thePlayer->getAvgEquipLevel() + Utility::generateRandomInt(1);
	
	//one is min monster level
	if (newLevel < 1) {
		newLevel = 1;
	}

	//six is max monster level
	if (newLevel > 6) {
		newLevel = 6;
	}

	//cout << "player equip avg level = " << thePlayer->getAvgEquipLevel() << endl;

	return new Monster(newLevel);
}

void Board::placeRandomMonster() {
	//new monster to place
	Monster* newMonster = createAppropriateMonster();

	int x, y, playerX, playerY;
	playerX = thePlayer->getLocation()->getX();
	playerY = thePlayer->getLocation()->getY();
	bool goodNewLocation = false;
	int tries = 0;

	while(!goodNewLocation) {
		tries++;

		x = Utility::generateRandomInt(BOARD_SIZE - 1);
		y = Utility::generateRandomInt(BOARD_SIZE - 1);
		
		//first check to see if the location is even available
		if (locations[x][y]->getAvailable()) {
			
			//then check if the location is a distance from the player
			if((abs(playerX - x) + abs(playerY - y)) > 12) {
				goodNewLocation = true;
			}
		}

		//if good location cannot be found in 12 tries, give up trying
		if (tries > 12) {
			delete newMonster;
			return;
		}
	}

	//now that we have good location, assign new monster to this location
	newMonster->setLocation(locations[x][y]);
	monsters.push_back(newMonster);
					
	//set monsterNum
	locations[x][y]->setMonsterNum((int)monsters.size() - 1);

	//set occupied
	locations[x][y]->setOccupied(true);

	//increase monsterCount
	monsterCount++;
}	

//do turn board maintenance
void Board::turnUpdate() {
	//add new monsters if the currentMonsterCount is getting low
	//cout << "monsterCount " << monsterCount << endl;

	//add another monster if needed
	if (monsterCount < (BOARD_SIZE - 3) * 2) {
		//cout << "NEW MONSTER" << endl;
		
		//place new monster half the time if this condition is met
		if (Utility::generateRandomInt(1) == 0) {
			placeRandomMonster();
		}
	}
}

//toggles the fog setting;  if this function is called, it means the player cheated!
void Board::toggleFog() {
	fog = !fog;
	cheated = true;
}

float Board::calculateVisionLighting(int locationX, int locationY) {
	int playerX = thePlayer->getLocation()->getX();
	int playerY = thePlayer->getLocation()->getY();

	//playerX becomes the distance from the location to the player
	playerX = abs(playerX - locationX) + abs(playerY - locationY);
	
	//special case on the player
	if (playerX == 0) {
		return 1.0f;
	}

	if (playerX <= thePlayer->getLIGHT()) {
		//don't let distance get greater than the light array lol
		if (playerX > 20) {
			playerX = 20;
		}

		return LIGHT_FADEOUT[playerX - 1];
	}
	else {
		return 0.0f;
	}	
}

bool Board::getFog() {
	return fog;
}

bool Board::getCheated() {
	return cheated;
}

bool Board::getGameOver() {
	return gameOver;
}

void Board::renderBitmapString(float x, float y, float z, void *font, string theString, SColor* theColor) { 
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
	strcpy(line, theString.substr(0, 74).c_str());
	
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