//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#include "XTriangle.h"

XTriangle::XTriangle() {
	vertexOne = new XVertex(0.5, 0.0, 0.0);
	vertexTwo = new XVertex(0.0, 0.5, 0.0);
	vertexThree = new XVertex(-0.5, 0.0, 0.0);

	calculateNormal();
}

XTriangle::XTriangle(XVertex* one, XVertex* two, XVertex* three) {
	vertexOne = one;
	vertexTwo = two;
	vertexThree = three;

	calculateNormal();
}

XTriangle::~XTriangle() {
	//cout << "XTriangle destructor" << endl;
	delete vertexOne;
	delete vertexTwo;
	delete vertexThree;
}

void XTriangle::calculateNormal() {
	XVector vectorOne, vectorTwo;
	vectorOne = XVector(vertexTwo->getX() - vertexOne->getX(), vertexTwo->getY() - vertexOne->getY(), vertexTwo->getZ() - vertexOne->getZ());
	vectorTwo = XVector(vertexThree->getX() - vertexOne->getX(), vertexThree->getY() - vertexOne->getY(), vertexThree->getZ() - vertexOne->getZ());

	vectorOne = vectorOne.crossProduct(vectorTwo);

	normalVector = vectorOne;
}

XVector XTriangle::calculateNormal(XVertex* vertexOne, XVertex* vertexTwo, XVertex* vertexThree) {
	XVector vectorOne, vectorTwo;
	vectorOne = XVector(vertexTwo->getX() - vertexOne->getX(), vertexTwo->getY() - vertexOne->getY(), vertexTwo->getZ() - vertexOne->getZ());
	vectorTwo = XVector(vertexThree->getX() - vertexOne->getX(), vertexThree->getY() - vertexOne->getY(), vertexThree->getZ() - vertexOne->getZ());

	vectorOne = vectorOne.crossProduct(vectorTwo);

	return vectorOne;
}

XVector XTriangle::getNormal() {
	return normalVector;
}

XVertex* XTriangle::getVertexOne() {
	return vertexOne;
}

XVertex* XTriangle::getVertexTwo() {
	return vertexTwo;
}

XVertex* XTriangle::getVertexThree() {
	return vertexThree;
}

void XTriangle::setAll(XVertex* one, XVertex* two, XVertex* three) {
	vertexOne = one;
	vertexTwo = two;
	vertexThree = three;
}
