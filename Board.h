//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#ifndef BOARD
#define BOARD

#include "HighscoreList.h"
#include "Effect.h"
#include "def.h"
#include "BoardLocation.h"
#include "Utility.h"
#include "MessageList.h"
#include "Item.h"
#include "Player.h"
#include "Monster.h"
#include "Effect.h"
#include <vector>
#include <list>
using namespace std;

class Board {
public:
	Board();
	Board(string playerName);
	~Board();

	Player* getPlayer();

	MessageList* getMessageList();

	void toggleFog();
	bool getFog();
	bool getCheated();
	bool getGameOver();

	void draw(int drawOption);
	void drawMonsterInfo(int x, int y);

	void turnUpdate();
	void frameUpdate(float deltaTime);

	bool movePlayer(int direction);
	bool playerPasses();

	void updateMonsters();

	HighscoreList* getHighscores();

	float calculateVisionLighting(int locationX, int locationY);
private:
	void loadHighscores();
	void checkForNewHighscore();

	void renderBitmapString(float x, float y, float z, void *font, string theString, SColor* theColor);
	void updateMonster(Monster* thisMonster);

	Monster* createAppropriateMonster();

	void playerAttack(int direction);

	void monsterAttack(Monster* thisMonster, int direction);
	void moveMonster(Monster* thisMonster, int direction);
	int findRandomMonsterMove(Monster* thisMonster);

	//returns direction player is located, if player is next to the monster;  0 if not next to
	int checkIfPlayerNextToMonster(Monster* thisMonster);

	//returns direction towards player, if the player is near the monster;  0 if not near
	int checkIfPlayerNearToMonster(Monster* thisMonster);

	void placeRandomMonster();

	void drawInfoBorder();

	void initialize();

	string getRandomAttackVerb();

	void drawGameOver();

	void initializeBoard();
	void addWelcomeMessages();

	int monsterCount;

	int deadLight;

	Player* thePlayer;

	int tip;

	bool fog, cheated, gameOver, playerWin, newScore;

	SColor *mountains, *lake, *grass, *plains;

	BoardLocation* locations[BOARD_SIZE][BOARD_SIZE];
	vector<Monster*> monsters;
	vector<Monster*> dragons; //points to the four dragons

	HighscoreList* highscores;

	MessageList* theMessageList;

	SColor *playerTakeDamageColor, *monsterMissesColor, *playerHitMonsterColor, *playerMissMonsterColor, *playerKillsMonsterColor, *playerPassesColor;
};

#endif