//////////////////////////////////////
//
// bGame Project
//
// Scott Atkins, 2006
//
//////////////////////////////////////

#ifndef XVECTOR
#define XVECTOR

class XVector {
public:
	XVector();
	XVector(float xx, float yy, float zz);
	~XVector();

	float getX();
	float getY();
	float getZ();

	void setX(float xx);
	void setY(float yy);
	void setZ(float zz);

	XVector crossProduct(XVector crossThis);
private:
	float x, y, z;
};

#endif