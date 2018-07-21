#include <util/Vector3D.h>


Vector3D::Vector3D()
{
}

Vector3D::Vector3D(float x, float y, float z)
		{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3D::Vector3D(VectorFloat v)
		{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}

void Vector3D::mix(Vector3D other, float otherFrac) {
	float keepFrac = 1.f - otherFrac;

	x = x * keepFrac + other.x * otherFrac;
	y = y * keepFrac + other.y * otherFrac;
	z = z * keepFrac + other.z * otherFrac;
}

void Vector3D::sub(Vector3D other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

Vector3D Vector3D::operator-(const Vector3D& b) {
         Vector3D vector3d;
         vector3d.x = x - b.x;
         vector3d.y = y - b.y;
         vector3d.z = z - b.z;
         return vector3d;
      }

Vector3D::~Vector3D()
{
	// TODO Auto-generated destructor stub
}

