/* Player.h
 * Defines the player object (and it's cool hud!)
 *
 * November 16, 2008
 * Phillip Napieralski
 * - Wrote the code
 */

#include "Universe.h" /* For object class */
#include "Fonts.h"
#include <iomanip>
#include <sstream>

static int   g_screenWidth = 640,
			 g_screenHeight = 480;

void setProjectionTo2D()
{
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D( (GLdouble)0, (GLdouble)g_screenWidth, 
		(GLdouble)0, (GLdouble)g_screenHeight );
}

void setViewport( int l, int r, int b, int t )
{
	glViewport(l, b, r-l, t-b);
}

class Gun : public Object
{
private:
public:
	Gun();
};

class Bullet : public Object
{
private:
	int ttl; /* Time to live */
public:
	Bullet();
};

class Player : public Object
{
private:
	VNCMesh playerMesh;
	Font font;
	float fuel;
	int score;

	void drawCrosshair()
	{
		const float SPACING = 10.0f;
		int sw = g_screenWidth >> 1; /* faster div by 2 */
		int sh = g_screenHeight >> 1;
		
		glColor3f( 0.6f, 0.6f, 0.6f );
		glBegin( GL_LINES );
			glVertex2f( sw, sh + SPACING );
			glVertex2f( sw, sh - SPACING );
			glVertex2f( sw + SPACING, sh );
			glVertex2f( sw - SPACING, sh );
		glEnd();
	}

	void drawScore()
	{
		setProjectionTo2D();
	
		glColor3f( 0.8f, 0.8f, 0.8f );

		/* Draw GUI stuff */
		ostringstream oss;
		oss << fixed << setprecision(0) << "Bad guys left: " << NUM_BADGUYS-(score/10) << "    Score: " << score;

		font.draw(50, 50, oss.str() );

		oss.str(""); /* reset it */
		oss << "Position: (" << position.x << ", " << position.y << ", " << position.z << ") ";
		
		font.draw(50, 30, oss.str());

		oss.str(""); /* reset it */
		if( fuel < 30 )
			glColor3f(0.95f,0.0f,0.0f);
		
		oss << "    Fuel: " << fuel << "%";
		font.draw(50 + 30*font.width(' '), 50, oss.str());
		
	}

public:
	Player()
	{
		playerMesh.read("player.3vnc");
		Camera();
		Object();
		setMesh(playerMesh);

		//font = Font(HELVETICA_18); 

		setDefault(); /* Setup camera */ 
		fuel = 100; 
	}
	
	void drawHud()
	{
		drawScore();
		drawCrosshair();
	}

	void resetScore(){ score = 0; }
	void increaseScore( int amt ){ score += amt; }
	void increaseFuel( float amt )
	{ 
		fuel += amt; 
		if( fuel > 100 )
			fuel = 100;
	}

	float checkFuel(){ return fuel; }
	void setFuel(float amt){ fuel = amt; }

};


