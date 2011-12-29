/* @file Object.h
 * @description This describes ANY object in the space, including the player 
 * @date November 16, 2008
 * @author Phillip Napieralski
 */

#pragma once

#include <vector>
#include "Random.h"
#include "Camera.h"
#include "mesh2.h"

enum OBJECT_TYPE { OBJECT_POWERUP = 0, OBJECT_BADGUY, OBJECT_PLAYER, OBJECT_WEAPON1, OBJECT_WEAPON2, OBJECT_WEAPON3, OBJECT_WEAPON4 };

/* Every object is it's own Camera => Potentially useful design idea */
class Object : public Camera
{
private:
	Random rnd;
	VNCMesh *mesh;	/* This is a pointer to an already loaded mesh (ideally) */
	bool isVisible, hitBoxVisible;
	int rotationAxis; /* random int between 0-5 determining how it should rotate initially*/

	/* Denotes the 2 points that the box lies between */
	Point3 box1; /* STILL UNUSED: Used for bounding box collision detection */
	Point3 box2; 

	void generateHitBoxes()
	{

	}

	void drawHitBox()
	{
		glutWireSphere(scale.x, 8, 8);
	}

public:
	/* position is inherited from Camera */
	Vector3 rotation;
	Vector3 scale;

	Object() 
	{ 
		Camera(); 
		hitBoxVisible = false;
		rotation = Vector3(0,0,0);
		scale = Vector3(1,1,1); 
		isVisible = true;  
		rotationAxis = rnd.RandomInt(0, 6);
	};

	Object( VNCMesh &someMesh )
	{ 
		Camera(); 
		hitBoxVisible = false;
		rotation = Vector3(0,0,0);
		scale = Vector3(1,1,1); 
		isVisible = true;
		rotationAxis = rnd.RandomInt(0, 5);
		setMesh(someMesh); 
	};

	/* Uses basic spherical collision detection */
	bool checkCollision( Object obj )
	{
		Vector3 tk;
		float length;

		tk.setDiff( getCenter(), obj.getCenter() );

		length = sqrt(tk.x*tk.x + tk.y*tk.y + tk.z*tk.z);

		if( length < (obj.getRadius() + getRadius()) )
			return true;

		return false;
	}

	Point3 getCenter()
	{ 
		Point3 cent;
		if(mesh)
			cent = mesh->getCenter(); 
		else
			cent = Point3(0,0,0);

		cent.x += position.x;
		cent.y += position.y;
		cent.z += position.z;

		return cent;
	}

	float getRadius()
	{ 
		return scale.x; 
	} /* Scale is uniform right now */
	
	void setMesh( VNCMesh &someM )
	{ 
		mesh = &someM;
	}

	void toggleVisible(){ isVisible = !isVisible; }
	void setVisible(bool bb){ isVisible = bb; }

	void toggleHitBoxVisible(){ hitBoxVisible = !hitBoxVisible; }
	void setHitBoxVisible(bool bb){ hitBoxVisible = bb; }

	void draw( Vector3 hitBoxColor = Vector3(1.0f,0,0) )
	{
		if( isVisible ) 
		{
			glPushMatrix();
				glTranslatef( position.x, position.y, position.z );
				glRotatef( rotation.x, 1.0f, 0.0f, 0.0f );
				glRotatef( rotation.y, 0.0f, 1.0f, 0.0f );
				glRotatef( rotation.z, 0.0f, 0.0f, 1.0f );

				if( hitBoxVisible )
				{
					glColor3f(hitBoxColor.x, hitBoxColor.y, hitBoxColor.z);
					drawHitBox();
				}

				glScalef( scale.x, scale.y, scale.z );
				glTranslatef( -0.5f, -0.5f, -0.5f );
				mesh->draw(TRUE);
			glPopMatrix();
		}
	}

	/* drawBasedOnCamera() draws the objects and uses the rotation that the camera
	   has done to rotate the object nicely */
	void drawBasedOnCamera()
	{
		const int LENGTH = 5;
		//rotation.y = 180*angleBetween( getLookDirection(),Vector3(1,0,0));
		//rotation.x = -180*angleBetween( getLookDirection(),Vector3(0,1,0));
		//rotation.z = -180*angleBetween( getUpVector(),Vector3(0,0,1));
		draw();
		glPushMatrix();
			glColor3f( 1,1,1 );
			glTranslatef( position.x, position.y, position.z );
			glBegin( GL_LINES );
				glVertex3f( 0,0,0 );
				glVertex3f( LENGTH*getLookDirection().x, LENGTH*getLookDirection().y, LENGTH*getLookDirection().z );
			glEnd();
		glPopMatrix();
	}

	void doRotation()
	{
		switch( rotationAxis )
		{
		case 0:
			rotation.x += 2.0;
			break;
		case 1:
			rotation.y += 1.0; 
			break;
		case 2:
			rotation.z += 1.5; 
			break;
		case 3:
			rotation.x += 0.67;
			rotation.y += 1.3; 
			break;
		case 4:
			rotation.x += 0.75;
			rotation.z += 1.1;
			break;
		case 5:
			rotation.y += 3.0;
			break;
		default: rotation.x += 1.0;
		}
	}

	void addRotation( float x, float y, float z )
	{
		rotation.x += x;
		rotation.y += y;
		rotation.z += z;
	}
};