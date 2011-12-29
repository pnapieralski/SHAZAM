/** @file support3d.h
 *  @description Defines constants and classes used to support 3D graphics.
*/
#ifndef __VECTOR3_H_
#define __VECTOR3_H_

#include <cmath>

const double PI = atan(1.0) * 4.0, 
			 TWO_PI = 2.0 * PI,
			 RADIANS_PER_DEGREE = PI / 180.0,
			 DEGREES_PER_RADIAN = 180.0 / PI;

class Point3
{ 
public: 
   double x, y, z;

   Point3() {x = y = z = 0;} 
   // Constructs the origin, point (0, 0, 0).

   Point3(double xx, double yy, double zz) { x = xx; y = yy; z = zz; }
   // Constructs the point (xx, yy, zz).

   void set(double dx, double dy, double dz) {x = dx; y = dy; z = dz; }
   // Makes this point (dx, dy, dz).

   void set(Point3& p) { x = p.x; y = p.y; z = p.z; }
   // Makes this point a copy of p.
}; 

class Vector3
{ 
public: 
	double x, y, z;

	// Constructs the zero vector, (0, 0, 0).
	Vector3() { x = y = z = 0; } 

	// Constructs the vector (xx, yy, zz).
	Vector3(double xx, double yy, double zz) { x = xx; y = yy; z = zz; }

	// Constructs a copy of v.
	Vector3(const Vector3& v) { x = v.x; y = v.y; z = v.z; }


	static double angleBetween( Vector3 a, Vector3 b )
	{
		return acos( a.dot(b) / (a.length() * b.length()) );
	}

	void set(double dx, double dy, double dz) { x = dx; y = dy; z = dz; } 

	void set(Vector3& v){ x = v.x; y = v.y; z = v.z;}

	void flip() { x = -x; y = -y; z = -z; } 

	void setDiff(Point3& a, Point3& b)
	{ 
		x = a.x - b.x; y = a.y - b.y; z = a.z - b.z; 
	}

	void normalize() 
	{		
		double sizeSq = x * x + y * y + z * z;
		if(sizeSq < 0.0000001)
		{
			cerr << "\nnormalize() sees vector (0,0,0)!";
			return; // does nothing to zero vectors;
		}
		double scaleFactor = 1.0/(double)sqrt(sizeSq);
		x *= scaleFactor; y *= scaleFactor; z *= scaleFactor;
	}

	Vector3 cross(Vector3 b) 
	{
	   Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
	   return c;
	}

	double dot(Vector3 b) 
	{ 
		return x * b.x + y * b.y + z * b.z; 
	}

	double length()
	{
		return sqrt(x*x + y*y + z*z);
	}

	friend Vector3 operator+(const Vector3& v1, const Vector3& v2)
	{
		Vector3 sum(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
		return sum; 
	}

	// Subtract two vectors : v1 - v2
	friend Vector3 operator-(const Vector3& v1, const Vector3& v2)
	{
		Vector3 diff(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
		return diff; 
	}
	
	friend Vector3 operator-(const Vector3& v)
	{
		Vector3 neg(-v.x, -v.y, -v.z);
		return neg; 
	}

	friend Vector3 operator*(double scalar, const Vector3& v)
	{
		Vector3 sum(scalar * v.x, scalar * v.y, scalar * v.z);
		return sum; 
	}

	friend Vector3 operator*(const Vector3& v, double scalar)
	{
		return scalar * v; 
	}
}; 

#endif