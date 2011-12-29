/* Universe.h
 * Creates a universe in which objects (good and bad) will roam
 *
 * November 16, 2008
 * Phillip Napieralski
 * - Wrote the code
 */

#include "Object.h"
#include <vector>

static int UNIVERSE_SIZE = 100;
static int DEAD_ZONE = 75;
static int NUM_BADGUYS = UNIVERSE_SIZE / 5;
static int NUM_POWERUPS = NUM_BADGUYS / 4;

#define NUM_STARS UNIVERSE_SIZE*25

class Star
{
public:
	Star(Point3 pos, Point3 col)
	{
		position = pos;
		color = col;
	}
	Point3 position;
	Point3 color;
};

class Universe
{
private:
	vector<Object> powerUps;
	vector<Object> badGuys;
	vector<Star> stars;
	VNCMesh powerUpMesh;
	VNCMesh badGuyMesh;

	Random rnd;

	bool failed; /* true if something goes wrong */
	void fail(){ failed = true; }

public:

	Universe()
	{
		failed = false;
		badGuyMesh.read("turtle.3vnc");
		//badGuyMesh.setOffset( Vector3(-1,0,-1) ); /* for badguy.3vnc */
		badGuyMesh.setOffset( Vector3( -.5f,.25f,-.5f ) ); /* for turtle.3vnc */
		badGuyMesh.applyOffset();

		powerUpMesh.read("powerUp.3vnc");
		//powerUpMesh.setOffset( Vector3(0,0,-.5f) );
		//powerUpMesh.applyOffset();
	}

	void generate(bool d = false)
	{
		unsigned k = 0;
		float universeSize = UNIVERSE_SIZE / 2.0f; /* bounds of universe */

		if(d)
			cout << "Generating universe..." << endl;

		/* Start fresh */
		badGuys.clear();
		powerUps.clear();
		stars.clear();
		/* Create good objects =) */
		for( unsigned i = 0; i < NUM_POWERUPS; i++ )
			powerUps.push_back(Object(powerUpMesh));

		/* Create bad objects =( */ 
		for( unsigned i = 0; i < NUM_BADGUYS; i++ )
			badGuys.push_back(Object(badGuyMesh));

		for( unsigned i = 0; i < NUM_STARS; i++ )
			stars.push_back(Star(Point3(0,0,0),Point3(1,1,1)));

		if( d )
			cout << "Spawning bad guys..." << endl; 

		/* Generate bad guys and power ups */
		for ( k = 0; k < NUM_BADGUYS; k++ )
		{
			bool okay = false; /* Okay to place there? */
			int ttl = 0;

			while( !okay )
			{
				int tx = rnd.RandomInt(-universeSize, universeSize+1);
				int ty = rnd.RandomInt(-universeSize, universeSize+1);
				int tz = rnd.RandomInt(-universeSize, universeSize+1);
				int rx = rnd.RandomInt(0,360);
				int ry = rnd.RandomInt(0,360);
				int rz = rnd.RandomInt(0,360);

				badGuys[k].rotation = Vector3(rx, ry, rz);
				badGuys[k].position = Point3(tx, ty, tz);

				badGuys[k].scale = Vector3(25, .5, .5);	
				/* Currently, checkCollision (in the object class) finds the radius of the sphere by merely using scale.x
				   So, to increase the spacing between objects, we do some number in the first position
				   and reset it later, kind of a hack, I know :p */


				/* Make sure no collision w/ other objects occurs */
				for( int i = 0; i < k; i++ )
				{
					if( !badGuys[i].checkCollision( badGuys[k] ) )
					{
						//badGuys[k].scale = Vector3( 1, .5, .5 );
						badGuys[k].scale = Vector3( 1,2,1);
						okay = true;
					}
				}

			/* Ok, we tried x times to get a non-colliding pos... Give up!*/
				if( ++ttl > 500 )
				{
					okay = true;
					//badGuys[k].scale = Vector3( 1, .5, .5 );
					badGuys[k].scale = Vector3( 1,2,1);
				}
			}
		}

		if(d)
			cout << "Spawning power ups..." << endl;
		for ( k = 0; k < NUM_POWERUPS; k++ )
		{
			int tx = rnd.RandomInt(-universeSize, universeSize+1);
			int ty = rnd.RandomInt(-universeSize, universeSize+1);
			int tz = rnd.RandomInt(-universeSize, universeSize+1);
			int rx = rnd.RandomInt(0,360);
			int ry = rnd.RandomInt(0,360);
			int rz = rnd.RandomInt(0,360);
			double s = 0.2 + rnd.RandomNum() * 0.8;
			powerUps[k].rotation = Vector3(rx, ry, rz);
			powerUps[k].position = Point3(tx, ty, tz);
			powerUps[k].scale = Vector3(s, s, s);
		}
	
		/* Make the stars span a little bit more area than the objects */
		universeSize = UNIVERSE_SIZE + DEAD_ZONE;
			
		/* Generate stars */
		for( k = 0; k < NUM_STARS; k++ )
		{
			/* Our stars color */
			float c = rnd.RandomInt(100, 255);

			/* make it between 0.0f and 1.0f */
			c /= 255;

			int tx = rnd.RandomInt(-universeSize, universeSize+1);
			int ty = rnd.RandomInt(-universeSize, universeSize+1);
			int tz = rnd.RandomInt(-universeSize, universeSize+1);
			stars[k].position.set( tx, ty, tz );
			stars[k].color.set( c, c, c );
		}

	}

	/* Draw a simple grid defining our boundaries */
	void drawOOBGrid3D( int size )
	{
		Point3 start(-size,-size,-size);
		Point3 end(size,size,size);
		const float SPACING = 25.0f;


		for( int k = 0; k < 4; k++)
		{
		glRotatef( 90*k, 1, 0, 0 );
			glPushMatrix();
			glBegin( GL_LINES );
				for( int i = 0; i <= 2*size; i+= SPACING )
				{
					glVertex3f( start.x + i, start.y + 2*size, start.z ); /* xz plane */
					glVertex3f( start.x + i, start.y, start.z );
					glVertex3f( start.x + 2*size, start.y + i, start.z);
					glVertex3f( start.x, start.y + i, start.z);
				}
			glEnd();
			glPopMatrix();
		}

		glPushMatrix();
		glBegin( GL_LINES );
			for( int i = 0; i <= 2*size; i+= SPACING )
			{
					glVertex3f( start.x , start.y + 2*size, start.z + i );
					glVertex3f( start.x, start.y, start.z + i );
					glVertex3f( start.x , start.y + i, start.z + 2*size );
					glVertex3f( start.x, start.y + i, start.z );

					glVertex3f( start.x+2*size, start.y + 2*size, start.z + i );
					glVertex3f( start.x+2*size, start.y, start.z + i );
					glVertex3f( start.x+2*size, start.y + i, start.z + 2*size );
					glVertex3f( start.x+2*size, start.y + i, start.z );
			}
		glEnd();
		glPopMatrix();
	}


	bool hasFailed(){ return failed; }

	/* Draw stars */
	void drawStars( )
	{
		glPushMatrix();
			glBegin( GL_POINTS );
				for( unsigned i = 0; i < stars.size(); i++ )
				{
					glColor3f( stars[i].color.x, stars[i].color.y, stars[i].color.z );
					glVertex3f(stars[i].position.x, stars[i].position.y, stars[i].position.z );
				}
			glEnd();
		glPopMatrix();
	}

	void draw()
	{
		glPushMatrix();
		for( unsigned i = 0; i < powerUps.size(); i++ )
		{
			/* Pass it a (0,1,0) color vector */
			powerUps[i].draw( Vector3(0,1.0f,0) );
		}
		glPopMatrix();

	
		glPushMatrix();
		for( unsigned i = 0; i < badGuys.size(); i++ )
		{
			badGuys[i].draw();
		}
		glPopMatrix();

		glColor3f( 0.2f, 0.2f, 0.2f );
		drawOOBGrid3D( UNIVERSE_SIZE );
	}

	/* returns the index of which badguy we collided with */
	int checkCollisionBadGuys( Object obj )
	{
		for( unsigned i = 0; i < badGuys.size(); i++ )
			if( badGuys[i].checkCollision( obj ) )
			{
				badGuys.erase( badGuys.begin() + i); /* remove the badguy */
				return i;
			}
		return -1;
	}

	
	int checkCollisionPowerUps( Object obj )
	{
		for( unsigned i = 0; i < powerUps.size(); i++ )
			if( powerUps[i].checkCollision( obj ) )
			{
				powerUps.erase( powerUps.begin() + i );
				return i;
			}
		return -1;
	}
	
	void updateRotations()
	{
		for( unsigned i = 0; i < powerUps.size(); i++ )
			powerUps[i].doRotation();
	
		for( unsigned i = 0; i < badGuys.size(); i++ )
			badGuys[i].doRotation();
	}

	void toggleHitBoxVisible()
	{
		for( unsigned i = 0; i < badGuys.size(); i++ )
			badGuys[i].toggleHitBoxVisible();
		for( unsigned i = 0; i < powerUps.size(); i++ )
			powerUps[i].toggleHitBoxVisible();
	}

	bool checkOOB( Object obj )
	{
		if( obj.position.x < UNIVERSE_SIZE && obj.position.x > -UNIVERSE_SIZE &&
			obj.position.y < UNIVERSE_SIZE && obj.position.y > -UNIVERSE_SIZE &&
			obj.position.z < UNIVERSE_SIZE && obj.position.z > -UNIVERSE_SIZE )
			return false;
		return true;
	}

	bool checkDeadZone( Object obj )
	{
		if( obj.position.x < UNIVERSE_SIZE+DEAD_ZONE && obj.position.x > -UNIVERSE_SIZE-DEAD_ZONE &&
			obj.position.y < UNIVERSE_SIZE+DEAD_ZONE && obj.position.y > -UNIVERSE_SIZE-DEAD_ZONE &&
			obj.position.z < UNIVERSE_SIZE+DEAD_ZONE && obj.position.z > -UNIVERSE_SIZE-DEAD_ZONE )
			return false;
		return true;
	}

	bool checkBounds( Object obj, int dist )
	{
		if( obj.position.x < dist && obj.position.x > -dist &&
			obj.position.y < dist && obj.position.y > -dist &&
			obj.position.z < dist && obj.position.z > -dist )
			return false;
		return true;
	}

	int getNumBadGuys(){ return badGuys.size(); }

	Object getFirstBadGuy()
	{
		if( !badGuys.empty() )
			return badGuys[0];
		return Object(badGuyMesh); /* Merely a placeholder */
	}

};