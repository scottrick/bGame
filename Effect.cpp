#include "Effect.h"

//constructor;  creates nothing, since no effect type was given
Effect::Effect() {
	effectType = 0;
	color = new SColor();
	intensity = 0;

	initialize();
}

Effect::Effect(int newEffectType, int newIntensity, SColor* newColor) {
	effectType = newEffectType;
	color = newColor;
	intensity = newIntensity;

	initialize();
}

Effect::~Effect() {
	delete color;

	for (unsigned int i = 0; i < gibs.size(); i++) {
		delete gibs.at(i);
	}
}

void Effect::initialize() {
	if (effectType == MONSTER_DEATH) {
		int numberOfGibs = 1 + Utility::generateRandomInt(4) + (Utility::generateRandomInt(1) + 4) * 3 * intensity;

		Gib* newGib;

		float velocity;

		for (int i = 0; i < numberOfGibs; i++) {
			newGib = new Gib(color->generateSimilarColor());
			
			velocity = Utility::generateRandomFloat(26.0f) + 2.0f;
			float randomAngle = Utility::generateRandomFloat(360.0f);

			float randomX = cos(randomAngle) * velocity;
			float randomY = sin(randomAngle) * velocity;

			newGib->setVelocityX(randomX);
			newGib->setVelocityY(randomY);
			newGib->setVelocityZ(-(Utility::generateRandomFloat(10.0f)) - 10.0f);
			
			gibs.push_back(newGib);
		}
	
		//cout << "Effect created with " << numberOfGibs << " gibs" << endl;
	}
}	

void Effect::frameUpdate(float deltaTime) {
	for (unsigned int i = 0; i < gibs.size(); i++) {
		gibs.at(i)->frameUpdate(deltaTime);

		//remove gib if it has expired
		if (gibs.at(i)->getExpired()) {
			delete gibs.at(i);
			gibs.erase(gibs.begin() + i);

			//decrement since one item was removed from the list
			i--;
		}
	}
}

void Effect::draw() {	
	for (unsigned int i = 0; i < gibs.size(); i++) {
		glPushMatrix();

		gibs.at(i)->draw();

		glPopMatrix();
	}
}

bool Effect::getExpired() {
	if (gibs.size() == 0) {
		return true;
	}
	else return false;
}