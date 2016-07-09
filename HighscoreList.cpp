//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#include "HighscoreList.h"

HighscoreList::HighscoreList() {
	//create new highscore list from file!
	
	//test if file exists
	ifstream infile(HIGHSCORE_FILENAME.c_str());
	
	if (infile.is_open()) {
		infile.close();

		//cout << "file exists!" << endl;
	
		readScores();
	}
	else {
		infile.close();

		//cout << "file does not exists, creating... " << endl;
		generateNewHighscores();
		
		writeScores();
	}	
	
}

HighscoreList::~HighscoreList() {
	for (int i = 0; i < (int)highscores.size(); i++) {
		delete highscores.at(i);
	}
}

void HighscoreList::writeScores() {
	ofstream outfile;
	outfile.open(HIGHSCORE_FILENAME.c_str());

	outfile << "# bGame highscores" << endl << "# Created by Scott Atkins, 2006" << endl;

	outfile.close();

	for (unsigned int i = 0; i < highscores.size(); i++) {
		highscores.at(i)->write();
	}
}

void HighscoreList::readScores() {
	//cout << "reading scores..." << endl;

	ostringstream stringBuffer;
    int dkills, kills, actions;
	char buffer[100];

	ifstream in;
	in.open(HIGHSCORE_FILENAME.c_str());

	MessageList* newList;

	string current;

	//read past first two lines of blather
	in.getline(buffer, 100);
	in.getline(buffer, 100);
	
	Highscore* newHighscore;

	in.getline(buffer, 100);
		
	for (int i = 0; i < 5; i++) {
		if (string(buffer) == "# BEGIN") {
			//cout << "MATCH" << endl;

			newHighscore = new Highscore();

			newList = new MessageList();
			
			//add name to newmessagelist
			in.getline(buffer, 100);
			newList->addMessage(new Message(string(buffer), NULL));

			//get and set dragonkillcount, killcount, and actions
			in >> dkills;
			in >> kills;
			in >> actions;

			newHighscore->setDragonsSlain(dkills);
			newHighscore->setKillCount(kills);
			newHighscore->setActions(actions);

			in.getline(buffer, 100);
			//cout << dkills << " " << kills << " " << actions << endl;

			stringBuffer.str("");
			stringBuffer.clear();
			stringBuffer << dkills << " / 4 Dragons Slain    " << kills << " Kills    " << actions << " Actions";
			newList->addMessage(new Message(stringBuffer.str(), NULL));

			//add the rest of the player info strings
			in.getline(buffer, 100);
			//cout << buffer << endl;
			newList->addMessage(new Message(string(buffer), NULL));
			in.getline(buffer, 100);
			//cout << buffer << endl;
			newList->addMessage(new Message(string(buffer), NULL));
			in.getline(buffer, 100);
			//cout << buffer << endl;
			newList->addMessage(new Message(string(buffer), NULL));
			in.getline(buffer, 100);
			//cout << buffer << endl;
			newList->addMessage(new Message(string(buffer), NULL));

			//set the playerlist
			newHighscore->setPlayerMessageList(newList);

			in.getline(buffer, 100);
			//cout << string(buffer) << endl;
			if (string(buffer) != "# EQUIPONE") {
				//cout << "BAD FILE" << endl;
				generateNewHighscores();
				return;
			}

			//start reading equip one info
			newList = new MessageList();
			in.getline(buffer, 100);
			while (string(buffer) != "# EQUIPTWO") {
				newList->addMessage(new Message(string(buffer), NULL));
				in.getline(buffer, 100);
			}
			//cout << "done reading equip1" << endl;
			newHighscore->setEquipOneMessageList(newList);

			//start reading equip two info
			newList = new MessageList();
			in.getline(buffer, 100);
			while (string(buffer) != "# EQUIPTHREE") {
				newList->addMessage(new Message(string(buffer), NULL));
				in.getline(buffer, 100);
			}
			//cout << "done reading equip2" << endl;
			newHighscore->setEquipTwoMessageList(newList);

			//start reading equip three info
			newList = new MessageList();
			in.getline(buffer, 100);
			while (string(buffer) != "# EQUIPFOUR") {
				newList->addMessage(new Message(string(buffer), NULL));
				in.getline(buffer, 100);
			}
			//cout << "done reading equip3" << endl;
			newHighscore->setEquipThreeMessageList(newList);

			//start reading equip four info
			newList = new MessageList();
			in.getline(buffer, 100);
			while (string(buffer) != "# END") {
				newList->addMessage(new Message(string(buffer), NULL));
				in.getline(buffer, 100);
			}
			//cout << "done reading equip2" << endl;
			newHighscore->setEquipFourMessageList(newList);

			//add highscore to list!
			highscores.push_back(newHighscore);

			//read next list for next loop
			in.getline(buffer, 100);
		}				


	}

	in.close();
}

//used to create a new highscore list
void HighscoreList::generateNewHighscores() {
	Player* temp;
	int dkills = 3;
		
	for (unsigned int i = 0; i < highscores.size(); i++) {
		delete highscores.at(i);
	}
	highscores.clear();
	highscores.empty();

	int kills = Utility::generateRandomInt(300) + 50;

	for (int i = 0; i < 5; i++) {
		if (i > 1) {
			temp = new Player(dkills, Utility::generateRandomInt(300) + 50);
		}
		else {
			kills += Utility::generateRandomInt(300) + 50;
			temp = new Player(dkills, kills);
		}
		highscores.push_back(new Highscore(temp));
		delete temp;

		dkills--;
		if (dkills < 1) {
			dkills = 1;
		}
	}

	writeScores();
}

Highscore* HighscoreList::getHighscore(int place) {
	//keep place inbetween 1 and 5
	if (place < 1) {
		place = 1;
	}

	if (place > 5) {
		place = 5;

	}

	return highscores.at(place - 1);
}

//returns true if score is a new highscore and updates the highscore list appropriately, false if not
bool HighscoreList::compare(Highscore* score) {
	vector<Highscore*>::iterator theIterator = highscores.begin();

	//compare new score with all five of the old highscores
	for (int i = 0; i < 5; i++) {	
		//more dragons, yes highscore!
		if (score->getDragonsSlain() > highscores.at(i)->getDragonsSlain()) {
			highscores.insert(theIterator, 1, score);

			writeScores();

			return true;
		}
		
		//same num of dragons, and less actions = new highscore!
		if ((score->getDragonsSlain() == highscores.at(i)->getDragonsSlain()) && (score->getActions() < highscores.at(i)->getActions())) {
			highscores.insert(theIterator, 1, score);

			writeScores();

			return true;
		}

		theIterator++;
	}

	return false;	
}