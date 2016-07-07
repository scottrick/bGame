#include "Gib.h"

//default constructor
Gib::Gib() {
	initialize();
}

Gib::Gib(SColor* newColor) {
	color = newColor;

	initialize();
}

Gib::~Gib() {
	delete color;
}

void Gib::initialize() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	velocityX = 0.0f;
	velocityY = 0.0f;
	velocityZ = 0.0f;
	//accelerateZ = 0.0f;
	scale = Utility::generateRandomFloat(0.5f) + 0.22f;

	degrees = (Utility::generateRandomFloat(MONSTER_SPIN_MAX - MONSTER_SPIN_MIN) + MONSTER_SPIN_MIN) * 4.0f;
	rotationX = Utility::generateRandomFloat(2.0f) - 1.0f;
	rotationY = Utility::generateRandomFloat(2.0f) - 1.0f;
	rotationZ = Utility::generateRandomFloat(2.0f) - 1.0f;
	currentDegrees = 0.0f;
}

void Gib::frameUpdate() {
	x += velocityX;
	y += velocityY;
	z += velocityZ;

	//don't let gibes go too far out otherwise the camera can't see them
	if (z < -5.0f) {
		z = -5.0f;
	}

	velocityZ += GRAVITY;
	//accelerateZ += GRAVITY;

	currentDegrees += degrees;
	if (degrees > 360.0f) {
		degrees -= 360.0f;
	}
}

//returns TRUE once this gib has run its course, and can be deleted (when its below the game board)
bool Gib::getExpired() {
	if (z >= 5.0f) {
		return true;
	}
	else return false;
}

void Gib::setVelocityX(float newVelocityX) {
	velocityX = newVelocityX;
}

void Gib::setVelocityY(float newVelocityY) {
	velocityY = newVelocityY;
}

void Gib::setVelocityZ(float newVelocityZ) {
	velocityZ = newVelocityZ;
}

void Gib::setScale(float newScale) {
	scale = newScale;
}

void Gib::draw() {
	glTranslatef(x, y, z);	
	glScalef(scale, scale, scale);
	glRotatef(currentDegrees, rotationX, rotationY, rotationZ);
	
	glColor3f(color->getR(), color->getG(), color->getB());
	glutSolidTetrahedron();
}

void Gib::print() {
	cout << "GIB " << this << endl;
}

float Gib::getVelocityX() {
	return velocityX;
}