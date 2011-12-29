/* camera.h
   _____________________________________________________________________________
 
                 Camera Class Definition and Implementation

   A Camera object is a synthetic camera that can be used for 3D viewing using
   OpenGL (based on a Camera class described in "Computer Graphics Using OpenGL,
   3rd Edition" by F.S. Hill Jr. and Stephen M. Kelley, Chapter 7).

   James L. Richards
   Last Update: November 4, 2008

   Phillip Napieralski
   November 15, 2008
   - Added update(), setDefault() methods. Modified constructor
   
   November 26, 2008
   - Added accel*() functions that add a more spaceship kind of feel to the camera
   - Modification: Made it so yaw/pitch/roll & move do NOT update the modelviewmatrix...
     this must be done by calling the update() method
   _____________________________________________________________________________
*/

#include <windows.h>  
#include <gl/Gl.h>
#include <gl/Glu.h>
#include <gl/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

#include "Vector3.h" // definitions for Point3 and Vector3

#ifndef CAMERA
#define CAMERA


//______________________________________________________________________________

	class Camera
	{
	public:
		Camera(){ setDefault(); }

		Camera(Point3 position,    // position position
				Point3 look,  // the "look at" point
				Vector3 up,  // the up direction
				float vAng,                  // viewing angle
				float asp,                // aspect ratio
				float nearD,                  // near plane distance
				float farD);                 // far plane distance
		// Constructs a camera object with default view volume settings.

		void set(Point3 position, Point3 look, Vector3 up); 
		// Repositions and aims this camera using the new position position, the
		// look vector, and the up vector.

		void setShape(float vAng, float asp, float nearD, float farD);
		// Establishes the view volume shape for a perspective view using a
		// viewing angle (vAng), an aspect ratio (aspect), the distance to the 
		// near plane (nearD), and the distance to the far plane (farD).

		void getShape(float& vAng, float& asp, float& nearD, float& farD);
		// Returns the view volume shape settings for viewing angle (vAng),
		// aspect ratio (aspect), distance to the near plane (nearD), and 
		// distance to the far plane (farD).

		void roll(float angle);
		// Sets the roll angle of rotation about the camera's z axis.

		void pitch(float angle);
		// Sets the pitch angle of rotation about the camera's x axis.

		void yaw(float angle);
		// Sets the yaw angle of rotation about the camera's y axis

		void move(float delU, float delV, float delN);
		// Repositions the camera position by adding delU, delV, and delN to the x,
		// y, and z coordinates of the position repectively. The camera orientation
		// is not changed.

		void moveTo(Point3 pt);
		// Repositions the camera to the point pt (in world coordinates)
		// without changing its uvn orientation.

		Point3 getPosition();
		// Returns the current position position.

		Vector3 getLookDirection();
		// Returns a unit vector in the direction the camera's heading (aim).


		/* Added by Phil,
		 * Move to point over time (using velocity as our guide) */
		void moveForward( float amt )
		{
			move(0, 0, -amt);
		}

		void moveRight( float amt )
		{
			move(amt,0,0);
		}

		void accelForward( float amt ); /* if amt negative, accel backwards */

		void accelRoll( float amt );

		void accelPitch( float amt );

		void accelYaw( float amt );

		void stopMoving();

		void update();

		/* Sets the camera to the default position/look/up */
		void setDefault();

		void setLookAt(Point3 look);

		void applyShape();

		/* Modify just the near plane */
		void setNearPlane( double nn ){ nearDist = nn; };

		Vector3 getUpVector();

		float getAcceleration(){ return acceleration; }

		float getVelocity(){ return velocity; }

		Vector3 getLookVelocity(){ return lookVelocity; }

		Point3  position;
	private:
		Vector3 u, v, n;

		float acceleration, velocity; /* movement vel and accel */
		Vector3 lookAccel; /* roll, pitch, yaw accel */
		Vector3 lookVelocity;

		double  viewAngle, aspect, nearDist, farDist; // view volume shape

		void setModelViewMatrix(); 
		// Loads the modelview matrix with camera settings.
	};

//______________________________________________________________________________
//
//                             Camera Implementation
//______________________________________________________________________________

	void Camera::setModelViewMatrix()
	{ 
		float m[16];
		Vector3 eVec(position.x, position.y, position.z); // a vector version of position 
		m[0] =  u.x; m[4] =  u.y; m[8]  =  u.z;  m[12] = -eVec.dot(u);
		m[1] =  v.x; m[5] =  v.y; m[9]  =  v.z;  m[13] = -eVec.dot(v);
		m[2] =  n.x; m[6] =  n.y; m[10] =  n.z;  m[14] = -eVec.dot(n);
		m[3] =  0;   m[7] =  0;   m[11] =  0;    m[15] = 1.0;
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(m); // load OpenGL’s modelview matrix
	}

//..............................................................................

Camera::Camera(Point3 position, Point3 look, Vector3 up,
				float vAng, float asp, float nearD, float farD)
{
	set(position, look, up);
	setShape(vAng, asp, nearD, farD);
}

//..............................................................................

void Camera::set(Point3 position, Point3 look, Vector3 up)
{	
	this->position.set(position); // store the given position position
	n.set(position.x - look.x, position.y - look.y, position.z - look.z); // make n
	u.set(up.cross(n)); // make u = up X n
	n.normalize(); u.normalize(); // make n and u unit length
	v.set(n.cross(u));  // make v =  n X u
	setModelViewMatrix(); // tell OpenGL 
}

void Camera::setLookAt( Point3 lookAt )
{
	set( position, lookAt, v );
}

//..............................................................................

void Camera::setShape(float vAng, float asp, float nearD, float farD)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(vAng, asp, nearD, farD);
	viewAngle = vAng;
	aspect = asp;
	nearDist = nearD;
	farDist = farD;
}

void Camera::applyShape()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( viewAngle, aspect, nearDist, farDist );
}

//..............................................................................

void Camera::getShape(float& vAng, float& asp, float& nearD, float& farD)
{
	vAng = viewAngle;
	asp = aspect;
	nearD = nearDist;
	farD = farDist;
}

//..............................................................................

void Camera::roll(float angle)
{
	float cs = cos(PI/180 * angle);
	float sn = sin(PI/180 * angle);
	Vector3 t = u; // save old u
	u.set(cs*t.x + sn*v.x, cs*t.y + sn*v.y, cs*t.z + sn*v.z);
	v.set(-sn*t.x + cs*v.x, -sn*t.y + cs*v.y, -sn*t.z + cs*v.z);
	//setModelViewMatrix(); /* Removed nov 26 2008 by Phil */
}

//..............................................................................

void Camera::pitch(float angle)
{
	float cs = cos(PI/180 * angle);
	float sn = sin(PI/180 * angle);
	Vector3 t = n; // save old n
	n.set(cs*t.x - sn*v.x, cs*t.y - sn*v.y, cs*t.z - sn*v.z);
	v.set(sn*t.x + cs*v.x, sn*t.y + cs*v.y, sn*t.z + cs*v.z);
	//setModelViewMatrix(); /* Removed nov 26 2008 by Phil */
}

//..............................................................................

void Camera::yaw(float angle)
{
	float cs = cos(PI/180 * angle);
	float sn = sin(PI/180 * angle);
	Vector3 t = u; // save old u
	u.set(cs*t.x - sn*n.x, cs*t.y - sn*n.y, cs*t.z - sn*n.z);
	n.set(sn*t.x + cs*n.x, sn*t.y + cs*n.y, sn*t.z + cs*n.z);
	//setModelViewMatrix(); /* Removed nov 26 2008 by Phil */
}

//..............................................................................

void Camera::move(float delU, float delV, float delN)
{
	position.x += delU * u.x + delV * v.x + delN * n.x;
	position.y += delU * u.y + delV * v.y + delN * n.y;
	position.z += delU * u.z + delV * v.z + delN * n.z;
	//setModelViewMatrix(); /* Removed nov 26 2008 by Phil */
}

//..............................................................................

void Camera::moveTo(Point3 pt)
{
	position = pt;
	setModelViewMatrix();
}

//..............................................................................

Point3 Camera::getPosition()
{
	return position;
}

//..............................................................................

Vector3 Camera::getLookDirection()
{
	return -n;
}

//..............................................................................


/* Phillip's Additions */
void Camera::update()
{
	moveForward( velocity );
	roll( lookVelocity.x );
	pitch( lookVelocity.y );
	yaw( lookVelocity.z );

	//setShape( viewAngle, aspect,  nearDist, farDist );
	setModelViewMatrix();
}

void Camera::setDefault()
{
	Point3 position = Point3(0.0,0.0,1.0);    // position position
	Point3 look = Point3(0.0,0.0,0.0);  // the "look at" point
	Vector3 up = Vector3(0.0,1.0,0.0);  // the up direction
	float vAng = 30.0;                  // viewing angle
	float asp = 4.0/3.0;                // aspect ratio
	float nearD = 0.01;                  // near plane distance
	float farD = 1000.0; 

	set(position, look, up);
	setShape(vAng, asp, nearD, farD);

	/* set to no movement */
	velocity = 0.0f;
	acceleration = 0.0f;

	lookVelocity = Vector3(0,0,0);
	lookAccel = Vector3(0,0,0);
}

void Camera::accelForward( float amt )
{
	const float LIMIT = 0.5f;
	if( velocity > LIMIT && amt > 0 ) /* cap our acceleration off */
		return ;
	if( velocity < -LIMIT && amt < 0 )
		return ;
	acceleration = amt;
	velocity += acceleration;
	
	//moveForward( velocity );
}

/* roll/pitch/yaw, but slowly and continuously */
void Camera::accelRoll( float amt )
{
	const float LIMIT = 0.5f;
	if( lookVelocity.x > LIMIT && amt > 0 ) /* cap our acceleration off */
		return ;
	if( lookVelocity.x < -LIMIT && amt < 0 )
		return ;
	lookAccel.x = amt;
	lookVelocity.x += lookAccel.x;

	//roll( lookVelocity.x );
}

void Camera::accelPitch( float amt )
{
	const float LIMIT = 0.5f;
	if( lookVelocity.y > LIMIT && amt > 0 ) /* cap our acceleration off */
		return ;
	if( lookVelocity.y < -LIMIT && amt < 0 )
		return ;
	lookAccel.y = amt;
	lookVelocity.y += lookAccel.y;

	//pitch( lookVelocity.y );
}

void Camera::accelYaw( float amt )
{
	lookAccel.z = amt;
	lookVelocity.z += lookAccel.z;

	//yaw( lookVelocity.z );

}

void Camera::stopMoving()
{
	lookAccel = Vector3( 0,0,0 );
	lookVelocity = Vector3( 0,0,0 );
	acceleration = 0;
	velocity = 0;
}

Vector3 Camera::getUpVector()
{
	return u;
}

#endif