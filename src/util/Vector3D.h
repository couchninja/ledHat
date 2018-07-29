#ifndef SRC_UTIL_VECTOR3D_H_
#define SRC_UTIL_VECTOR3D_H_

// needed to subclass VectorFloat
#include<math.h>
#include<stdint.h>
#include "helper_3dmath.h"
#include<WString.h>

class Vector3D: public VectorFloat {
public:
	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(VectorFloat v);
	virtual ~Vector3D();

	void lerp(Vector3D other, float otherFrac);
	void sub(Vector3D other);
	float dot(Vector3D other);
	void scalef(float s);
	String toString();
	Vector3D operator-(const Vector3D& other);
	Vector3D operator+(const Vector3D& other);
};

#endif /* SRC_UTIL_VECTOR3D_H_ */
