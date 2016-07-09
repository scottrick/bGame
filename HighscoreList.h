//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#ifndef HIGHSCORE_LIST
#define HIGHSCORE_LIST

#include "Def.h"
#include "Highscore.h"
#include "Player.h"
#include <vector>
using namespace std;

class HighscoreList {
public:
	//default constructor;  simply creates the list from the filename HIGHSCORE_FILENAME
	HighscoreList();

	~HighscoreList();

	bool compare(Highscore* score);

	Highscore* getHighscore(int place);	
private:
	void writeScores();
	void readScores();
	void generateNewHighscores();

	vector<Highscore*> highscores;	
};

#endif