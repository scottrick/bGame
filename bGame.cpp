//***********************************************************
//
// Scott Atkins
//
// bGame Project
// 
// July 24th, 2006
// 
//***********************************************************

#include "Board.h"
#include <ctime>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <GL/glut.h>
using namespace std;

//default window size;
const int startingWindowX = 1024;
const int startingWindowY = 768;

float windowX = startingWindowX;
float windowY = startingWindowY;

//glut functions
void gameDisplay();
//void menuOptions(int menuNumber);
void reshapeDisplay(GLsizei W, GLsizei H);
void setViewport(int left, int right, int bottom, int top);
//void setWindow(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
void gameKeyboard(unsigned char key, int x, int y);
//void setViewpoint(float eyeX, float eyeY, float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ);
//void myMouseClick(int button, int state, int x, int y);
void mouseMove(int x, int y);
void mousePassiveMove(int x, int y);
void specialKeyHandler(int key, int x, int y);
void renderBitmapString(float x, float y, float z, void *font, string theString);
void loopFunc();
void initializeGame();
void createHelpMessageList();

//test functions
void testMessages();
void testBoard();

//other function declarations
void initializeGlut(int argc, char** argv);
void drawInfo();
void drawPlayerInfo();
void drawEquipmentInfo();
void drawMessages();
void drawBorderBarriers();
void drawMap();
void drawHelp();
void drawPlayerNameScreen();
void drawHighScores();

//VARS for CLOCK
long previousTime, currentTime;
int deltaTime, accumulator, secondAccumulator, currentFPScount, previousFPScount;

//show info
bool SHOW_INFO = false;
int LIGHTING_MODE = 1;
int MAP_MODE = DRAW_NORMAL;  // DRAW_NORMAL = no map info;  DRAW_INFO = level information

#define FRAMES_PER_SECOND 25
#define MILLI_PER_FRAME 1000 / FRAMES_PER_SECOND

bool turnProcessing = false; //true while player object is moving, while monsters are moving, attacking, etc
							//while true, player cannot take actions
int frameProcessingCount = 0; //counter which tells when to allow player action again

int MOUSE_X = 0;
int MOUSE_Y = 0;

//used when getting the character's name at the startup
string currentNameString = "";
string currentNameStringSuffix = "|";
int nameStringSuffixCounter = 0;

bool havePlayerName = false;

bool PAUSED = false;  //game paused when PAUSED = TRUE
bool DISPLAY_HELP = false; //help screen displayed instead of map when this bool is TRUE
bool DISPLAY_HIGH_SCORES = false;

//frame counter
long frameCount = 0;

//the GAME!
Board* theBoard;

//help message list
MessageList* helpMessageList = new MessageList(20);

void gameDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glMatrixMode (GL_PROJECTION);    /* prepare for and then */ 
    glLoadIdentity ();               /* define	 the projection */
    glFrustum (-1.0, 1.0, -1.0, 1.0, -20.0, 20.0); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (havePlayerName) {
		if ((DISPLAY_HIGH_SCORES) || (theBoard->getGameOver())) {
			setViewport(0, 0.25f * windowX, 0, windowY);
			drawHighScores();
		}
		else {
			//draw player information (top left)
			setViewport(0, 0.25f * windowX, (1.0f - PLAYER_INFO_SIZE) * windowY, windowY);
			drawPlayerInfo();

			//draw equipment (middle left)
			setViewport(0, 0.25f * windowX, MESSAGE_INFO_SIZE * windowY, (1.0f - PLAYER_INFO_SIZE) * windowY);
			drawEquipmentInfo();

			//draw messages (bottom left)
			setViewport(0, 0.25f * windowX, 0, MESSAGE_INFO_SIZE * windowY);
			drawMessages();
		}
	}
	
	if (!havePlayerName) {
		setViewport(0, windowX, 0, windowY);
		drawPlayerNameScreen();		
	}
	//if DISPLAY_HELP not true, draw the map
	else if (DISPLAY_HELP) {
		setViewport(0.25f * windowX, windowX, 0, windowY);
		drawHelp();
	}
	else {
		//draw map
		setViewport(0.25f * windowX, windowX, 0, windowY);
		drawMap();
	}

	//draw tech info (top right, over some of map)
	if (SHOW_INFO) {
		setViewport(0, windowX, 0, windowY);
		drawInfo();
	}

	glFlush();
	glutSwapBuffers();
}


void initializeGlut(int argc, char** argv) {
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(startingWindowX, startingWindowY);	
	glutCreateWindow("bGame");
//	glutFullScreen();

	//do a depth test to remove hidden surfaces
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glutIdleFunc(loopFunc);
	glutDisplayFunc(gameDisplay);
	glutReshapeFunc(reshapeDisplay);
	glutKeyboardFunc(gameKeyboard);
	glutPassiveMotionFunc(mousePassiveMove);
	glutMotionFunc(mouseMove);
	glutSpecialFunc(specialKeyHandler);
		
	glutMainLoop();
}

void loopFunc() {
	//if the game is paused, do not update the game state information	
	if (PAUSED) {
		gameDisplay();
		currentFPScount++;

		currentTime = GetTickCount();
		accumulator = 0;
		secondAccumulator = 0;
		currentFPScount = 0;
		previousFPScount = 0;

		return;
	}

	if (currentTime == 0) {
		currentTime = GetTickCount();
		accumulator = 0;
		secondAccumulator = 0;
		currentFPScount = 0;
		previousFPScount = 0;
	}
	
	previousTime = currentTime;
	currentTime = GetTickCount();

	deltaTime = currentTime - previousTime;
	accumulator += deltaTime;
	secondAccumulator += deltaTime;
	
	//if one second has passed, then update FPS counter
	if (secondAccumulator >= 1000) {
		secondAccumulator -= 1000;
		previousFPScount = currentFPScount;
		currentFPScount = 0;
	}

	while (accumulator >= MILLI_PER_FRAME) {
		if (turnProcessing) { //turn is in progress
			frameProcessingCount++;

			if (havePlayerName) {
				if (frameProcessingCount >= TURN_FRAME_LENGTH) {
					frameProcessingCount = 0;
					turnProcessing = false;
					
					//update player
					theBoard->getPlayer()->turnUpdate();

					//update board
					theBoard->turnUpdate();
				}

				//if the turn is half way processed, begin moving the monsters
				//this is where the monsters will attack
				if (frameProcessingCount == (TURN_FRAME_LENGTH / 2)) {
					theBoard->updateMonsters();
				}
			}
		}

		/*
		if (frameCount < 12) {
			if (frameCount % 3 == 0){
				theBoard->testEffect();
			}
		}
		*/

		if (!havePlayerName) {
			//cout << nameStringSuffixCounter << endl;
			nameStringSuffixCounter++;

			//every so many frames switch the suffix so it appears to be a blinking cursor
			if (nameStringSuffixCounter > 8) {
				nameStringSuffixCounter = 0;
				
				if (currentNameStringSuffix == "|") {
					currentNameStringSuffix = "";
				}
				else {
					currentNameStringSuffix = "|";
				}
			}
		}

		//increment to next frame
		frameCount++;

		if (havePlayerName) {
			//update game info here
			theBoard->frameUpdate();
		}

		//cout << "updating gamestate information" << endl;
		accumulator -= MILLI_PER_FRAME;
	}

	//cout << "rendering..." << endl;
	gameDisplay();
	currentFPScount++;
}

void specialKeyHandler(int key, int x, int y) {
	//cout << "SPECIAL key pressed " << key << endl;

	switch (key) {
		case 101:
			if (!turnProcessing) {
				turnProcessing = theBoard->movePlayer(NORTH);
				//turnProcessing = true;
			}
			break;
		case 103:
			if (!turnProcessing) {
				turnProcessing = theBoard->movePlayer(SOUTH);
				//turnProcessing = true;
			}
			break;
		case 102:
			if (!turnProcessing) {
				turnProcessing = theBoard->movePlayer(EAST);
				//turnProcessing = true;
			}
			break;
		case 100:
			if (!turnProcessing) {
				turnProcessing = theBoard->movePlayer(WEST);
				//turnProcessing = true;
			}
			break;
		default:
			break;
	}
}

void gameKeyboard(unsigned char key, int x, int y) {
	cout << "key pressed " << (int)key << endl;

	//EXIT
	if (key == 3) { // c + ctrl
		exit(0);
	}

	//special key handling while getting the player's name
	if (!havePlayerName) {
		//special case for BACKSPACE (delete last character)
		if (key == 8) {
			cout << "DELETING LAST CHAR" << endl;
			//currentNameString.erase(currentNameString.size() - 1);
			currentNameString = currentNameString.substr(0, currentNameString.size() - 1);
			return;
		}

		//special case for ENTER (accept the name)
		if (key == 13) {
			//player accepts the current name
			havePlayerName = true;

			initializeGame();
		}

		//enforce maximum name size
		if (currentNameString.size() > 20) {
			return;
		}

		string temp(1, key);
		currentNameString.append(temp);

		return;
	}

	//toggle info display
	if ((key == 105) || (key == 73)) {
		SHOW_INFO = !SHOW_INFO;
	}

	//player pressed Y or y
	if ((key == 121) || (key == 89)) {
		
		//if the game is over and the player chooses yes, reset the game
		if (theBoard->getGameOver()) {
			string oldName = theBoard->getPlayer()->getName();

			delete theBoard;

			theBoard = new Board(oldName);
		}
		else {
			theBoard->getPlayer()->playerPressedYesOrNo(true);
		}
	}

	//player pressed N or n
	if ((key == 110) || (key == 78)) {
		
		//if player pressed no while game is over, they are quitting
		if (theBoard->getGameOver()) {
			exit(0);
		}
		else {
			theBoard->getPlayer()->playerPressedYesOrNo(false);
		}
	}	

	printf("key: %d", key);

	//change map display mode
	if ((key == 109) || (key == 77)) {
		// i
		MAP_MODE = (MAP_MODE + 1) % 2;
	}

	//none of the below commands work if the game is over
	if (theBoard->getGameOver()) {
		return;
	}

	//toggle highscore display
	if ((key == 115) || (key == 83)) {
		DISPLAY_HIGH_SCORES = !DISPLAY_HIGH_SCORES;
	}

	//toggle vision FOG;  note once this is toggled OFF at any time during the game, the highscore will note that you cheated!
	if ((key == 102) || (key == 70)) {
		theBoard->toggleFog();
	}

	//switch lighting mode
	if ((key == 108) || (key == 76)) {
		LIGHTING_MODE = (LIGHTING_MODE + 1) % 3;
	}

	//toggle HELP screen and toggle game paused
	if ((key == 104) || (key == 72)) {
		cout << "display help toggled to " << DISPLAY_HELP << endl;

		DISPLAY_HELP = !DISPLAY_HELP; //toggle display help boolean
		if (DISPLAY_HELP) {
			PAUSED = TRUE;
		}
		else {
			PAUSED = FALSE;
		}
	}	

	//pause game if help hasn't already paused it
	if ((key == 112) || (key == 80)) {
		//toggle PAUSED only if help display isn't true
		if (!DISPLAY_HELP) {
			PAUSED = !PAUSED;
		}
	}

	//player pressed spacebar;   pass turn
	if (key == 32) {
		if (!turnProcessing) {
			turnProcessing = theBoard->playerPasses();
		}
	}

}

void mousePassiveMove(int x, int y) {
	MOUSE_X = x;
	MOUSE_Y = y;
}

void mouseMove(int x, int y) {
	MOUSE_X = x;
	MOUSE_Y = y;
}

void setViewport(int left, int right, int bottom, int top) {
	//cout << "set " << left << " " << right << " " << bottom << " " << top << endl;
	glViewport(left, bottom, right - left, top - bottom);
}

//draw high scores - will replace the left information bar
void drawHighScores() {
	renderBitmapString(-.95f, 0.0f, -0.1f, GLUT_BITMAP_HELVETICA_18, "highscores");
}

void drawInfo() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glBegin(GL_QUADS);
	glColor4f(0.1f, 0.1f, 0.1, 0.7f);
	glVertex3f(0.7f, 0.75f, -0.4f);
	glVertex3f(0.7f, 1.0f, -0.4f);
	glVertex3f(1.0f, 1.0f, -0.4f);
	glVertex3f(1.0f, 0.75f, -0.4f);

	//draw outer border
	glColor3f(0.8f, 0.4f, -0.45f);      
	glVertex3f(0.7f, 0.75f, -0.45f);
	glVertex3f(0.705f, 0.75f, -0.45f);
	glVertex3f(0.705f, 0.995f, -0.45f);
	glVertex3f(0.7f, 0.995f, -0.45f);
	glVertex3f(0.7f, 0.995f, -0.45f);
	glVertex3f(0.7f, 1.0f, -0.45f);
	glVertex3f(0.995f, 1.0f, -0.45f);
	glVertex3f(0.995f, 0.995f, -0.45f);	
	glVertex3f(0.995f, 1.0f, -0.45f);
	glVertex3f(1.0f, 1.0f, -0.45f);
	glVertex3f(1.0f, 0.755f, -0.45f);
	glVertex3f(0.995f, 0.755f, -0.45f);
	glVertex3f(1.0f, 0.755f, -0.45f);
	glVertex3f(1.0f, 0.75f, -0.45f);
	glVertex3f(0.705f, 0.75f, -0.45f);
	glVertex3f(0.705f, 0.755f, -0.45f);
	glEnd();

	glDisable(GL_BLEND);

	//draw information
	glColor3f(0.9f, 0.9f, 0.9f);
		
	ostringstream buffer;

	buffer << previousFPScount << " FPS";
	renderBitmapString(0.75f, 0.95f, -0.5f, GLUT_BITMAP_HELVETICA_18, buffer.str());

	buffer.str("");
	buffer.clear();
    buffer << "Frame " << frameCount;
	renderBitmapString(0.75f, 0.9f, -0.5f, GLUT_BITMAP_HELVETICA_18, buffer.str());

	buffer.str("");
	buffer.clear();
	buffer << "(" << MOUSE_X << ", " << MOUSE_Y << ")";
	renderBitmapString(0.75f, 0.85f, -0.5f, GLUT_BITMAP_HELVETICA_18, buffer.str());

	buffer.str("");
	buffer.clear();
	buffer << "Cheated?  " << theBoard->getCheated();
	renderBitmapString(0.75f, 0.8f, -0.5f, GLUT_BITMAP_HELVETICA_18, buffer.str());
}	

void drawPlayerInfo() {
	//first, draw the border around the player info area
	glBegin(GL_QUADS);

	//draw background
	glColor3f(0.1f, 0.1f, 0.1f);
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

	theBoard->getPlayer()->drawPlayerInfo();
}

void drawEquipmentInfo() {
	theBoard->getPlayer()->drawEquipment();
}

void drawMessages() {
	//first, draw the border around the message area
	glBegin(GL_QUADS);

	//draw background
	glColor3f(0.1f, 0.1f, 0.1f);
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
	
	Message* currentMessage;
	
	vector<Message*> messageList = theBoard->getMessageList()->getMessages();
	
	for (int i = 0; i < (int)theBoard->getMessageList()->getMessages().size(); i++) {
		currentMessage = messageList.at(i);
		
		glColor3f(currentMessage->getColor()->getR(), currentMessage->getColor()->getG(), currentMessage->getColor()->getB());
		renderBitmapString(-0.9f, 0.88f - (i * 1.88f / (float)MESSAGE_LIST_SIZE), 0.0f, GLUT_BITMAP_HELVETICA_12, currentMessage->getMessage());
	}
}

void createHelpMessageList() {
	Message* msg = new Message("~~~~ bGame HELP ~~~~", new SColor(0.1f, 0.1f, 1.0f));
	helpMessageList->addMessage(msg);

	msg = new Message("   The object of the game is to defeat the four dragons.  They are the Blue, Red, Bronze, and Green Dragons.", new SColor(0.9f, 0.9f, 0.9f));
	helpMessageList->addMessage(msg);

	msg = new Message("~~~~ Controls ~~~~", new SColor(0.4f, 0.99f, 0.01f));
	helpMessageList->addMessage(msg);

	msg = new Message("   ARROW KEYS  - Control you character by moving to an adjacent empty location, or attacking an occupied one.", new SColor(0.9f, 0.9f, 0.9f));
	helpMessageList->addMessage(msg);

	msg = new Message("   SPACEBAR    - Do nothing and let the monsters move and/or attack.", new SColor(0.9f, 0.9f, 0.9f));
	helpMessageList->addMessage(msg);

	msg = new Message("   'Y / N'     - Yes and No.  Used to respond when asked a question.", new SColor(0.9f, 0.9f, 0.9f));
	helpMessageList->addMessage(msg);

	msg = new Message("   'F / f'     - Toggles vision fog.  Turning the map fog off is considered cheating!", new SColor(0.9f, 0.9f, 0.9f));
	helpMessageList->addMessage(msg);

	msg = new Message("   'M / m'     - Displays monster information on the map.", new SColor(0.9f, 0.9f, 0.9f));
	helpMessageList->addMessage(msg);

	msg = new Message("   'P / p'     - Pauses the game.", new SColor(0.9f, 0.9f, 0.9f));
	helpMessageList->addMessage(msg);

	msg = new Message("   'S / s'     - Displays the highscores.", new SColor(0.9f, 0.9f, 0.9f));
	helpMessageList->addMessage(msg);

	msg = new Message("   'H / h'     - Brings up this help information.", new SColor(0.9f, 0.9f, 0.9f));
	helpMessageList->addMessage(msg);

	msg = new Message("   Ctrl + C    - Quits the game.", new SColor(0.9f, 0.9f, 0.9f));
	helpMessageList->addMessage(msg);

	msg = new Message("~~~~ Other Controls ~~~~", new SColor(0.86f, 0.18f, 0.92f));
	helpMessageList->addMessage(msg);

	msg = new Message("   'I / i'     - Displays technical information.", new SColor(0.9f, 0.9f, 0.9f));
	helpMessageList->addMessage(msg);

	msg = new Message("   'L / l'     - Cycles through the three lighting modes: Flat Shading, Smooth Shading, and No Lighting.", new SColor(0.9f, 0.9f, 0.9f));
	helpMessageList->addMessage(msg);

	msg = new Message("~~~~ Other Info ~~~~", new SColor(0.02f, 0.97f, 0.81f));
	helpMessageList->addMessage(msg);

	msg = new Message("   bGame v0.9", new SColor(0.9f, 0.9f, 0.9f));
	helpMessageList->addMessage(msg);
		
	msg = new Message("   Created, Designed, and Programmed by Scott Atkins, 2006", new SColor(0.9f, 0.9f, 0.9f));
	helpMessageList->addMessage(msg);

	msg = new Message("   Thanks to Sam Liebo, Official Beta Tester (tm)", new SColor(0.9f, 0.9f, 0.9f));
	helpMessageList->addMessage(msg);
}

void drawPlayerNameScreen() {
	glPushMatrix();
	glScalef(0.75f, 0.5f, 1.0f);

	//draw background
	glBegin(GL_QUADS);
	glColor3f(0.1f, 0.1f, 0.1f);
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
	
	//draw border around the player name text box
	//outer box
	glVertex3f(-.95f, -0.15f, 0.1f);
	glVertex3f(-0.93f, -0.15f, 0.1f);
	glVertex3f(-0.93f, 0.13f, 0.1f);
	glVertex3f(-0.95f, 0.13f, 0.1f);
	glVertex3f(-0.95f, 0.13f, 0.1f);
	glVertex3f(-0.95f, 0.15f, 0.1f);
	glVertex3f(0.93f, 0.15f, 0.1f);
	glVertex3f(0.93f, 0.13f, 0.1f);	
	glVertex3f(0.93f, 0.15f, 0.1f);
	glVertex3f(0.95f, 0.15f, 0.1f);
	glVertex3f(0.95f, -0.13f, 0.1f);
	glVertex3f(0.93f, -0.13f, 0.1f);
	glVertex3f(0.95f, -0.13f, 0.1f);
	glVertex3f(0.95f, -0.15f, 0.1f);
	glVertex3f(-0.93f, -0.15f, 0.1f);
	glVertex3f(-0.93f, -0.13f, 0.1f);

	//inner box
	glColor3f(0.81f, 0.34f, 0.03f);
	glVertex3f(-0.93f, -0.13f, 0.1f);
	glVertex3f(-0.92f, -0.13f, 0.1f);
	glVertex3f(-0.92f, 0.12f, 0.1f);
	glVertex3f(-0.93f, 0.12f, 0.1f);
	glVertex3f(-0.93f, 0.12f, 0.1f);
	glVertex3f(-0.93f, 0.13f, 0.1f);
	glVertex3f(0.92f, 0.13f, 0.1f);
	glVertex3f(0.92f, 0.12f, 0.1f);	
	glVertex3f(0.92f, 0.13f, 0.1f);
	glVertex3f(0.93f, 0.13f, 0.1f);
	glVertex3f(0.93f, -0.12f, 0.1f);
	glVertex3f(0.92f, -0.12f, 0.1f);
	glVertex3f(0.93f, -0.12f, 0.1f);
	glVertex3f(0.93f, -0.13f, 0.1f);
	glVertex3f(-0.92f, -0.13f, 0.1f);
	glVertex3f(-0.92f, -0.12f, 0.1f);	
	glEnd();

	glColor3f(0.9f, 0.9f, 0.9f);
	renderBitmapString(-0.95f, 0.85f, -0.1f, GLUT_BITMAP_TIMES_ROMAN_24, "Welcome to Dragon Quest!");

	glColor3f(0.8f, 0.8f, 0.8f);
	renderBitmapString(-0.95f, 0.3f, -0.1f, GLUT_BITMAP_HELVETICA_18, "  Enter Your Name:");

	glColor3f(0.6f, 0.6f, 0.75f);
	renderBitmapString(-0.9f, 0.0f, -0.1f, GLUT_BITMAP_HELVETICA_18, (currentNameString + currentNameStringSuffix));

	glColor3f(0.8f, 0.8f, 0.8f);
	renderBitmapString(-0.95f, -0.3f, -0.1f, GLUT_BITMAP_HELVETICA_18, "  Press ENTER when finished.  Leave blank for a random name.");


	glPopMatrix();
}	

void drawHelp() {
	Message* currentMessage;
	
	vector<Message*> messageList = helpMessageList->getMessages();
	
	for (unsigned int i = 0; i < messageList.size(); i++) {
		currentMessage = messageList.at(i);
		
		glColor3f(currentMessage->getColor()->getR(), currentMessage->getColor()->getG(), currentMessage->getColor()->getB());
		renderBitmapString(-0.95f, 0.9f - (i * 0.08f) , 0.0f, GLUT_BITMAP_HELVETICA_12, currentMessage->getMessage());
	}	
}

void drawMap() {
	switch (LIGHTING_MODE) {
		case 0:
			glShadeModel(GL_SMOOTH);
			glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
			break;
		case 1:
			glShadeModel(GL_FLAT);
			glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
			break;
		case 2:
			//in this case, no lighting is actually used
			break;
		default:
			cout << "BAD LIGHTING MODE: bGame::gameDisplay()";
			break;
	}

	if ((LIGHTING_MODE == 0) || (LIGHTING_MODE == 1)) {
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_LIGHT0);
	}
	if (LIGHTING_MODE == 2) {
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHT0);
	}
	
	theBoard->draw(MAP_MODE);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);

	//draw hover information if mouse is hovering over a monster//dragon//theplayer
	if (MOUSE_X > (int)(0.25f * (float)windowX)) {
		float ySquareSize = ((float)windowY / (float)BOARD_SIZE);
		float xSquareSize = ((float)windowX * 0.75f) / (float)BOARD_SIZE;
		int xPos = (int)(((float)MOUSE_X - (0.25f * (float)windowX)) / xSquareSize);
		int yPos = (int)((float)MOUSE_Y / ySquareSize);

		//if location mouse is over is visible, or fog is off, then draw monster info
		if ((!theBoard->getFog()) || (theBoard->calculateVisionLighting(xPos, yPos) > 0.0f)) {
			//cout << xPos << " " << yPos << endl;
			
			int left = MOUSE_X;
			int right = left + 0.35f * (float)windowX;

			int bottom = (windowY - MOUSE_Y);
			int top = bottom + 0.18f * (float)windowY;

			if (top > windowY) {
				int reduceBy = top - windowY;
				top -= reduceBy;
				bottom -= reduceBy;
			}

			if (right > windowX) {
				int reduceBy = right - windowX;
				left -= reduceBy;
				right -= reduceBy;
			}

			glLoadIdentity();
			setViewport(left, right, bottom, top);
			glTranslatef(0.0f, 0.0f, -0.5f);
			theBoard->drawMonsterInfo(xPos, yPos);

		}
	}
}

void reshapeDisplay(GLsizei W, GLsizei H) {	
	windowX = (float) W;
	windowY = (float) H;
}

void renderBitmapString(float x, float y, float z, void *font, string theString) {  
	char line[200];
	strcpy(line, theString.substr(0, 199).c_str());
	
	char *c;
	glRasterPos3f(x, y, z);

	for (c =line; *c != '\0'; c++) {
	 glutBitmapCharacter(font, *c);
	}
}

void initializeGame() {
	srand(time(NULL));

	createHelpMessageList();

	if (currentNameString == "") {
		theBoard = new Board();
	}
	else {
		theBoard = new Board(currentNameString);
	}
}

int main(int argc, char** argv) {
	initializeGlut(argc, argv);		
}