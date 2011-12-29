/* Shazam.h
 * This file holds the glory for this program 
 *
 * November 16, 2008
 * Phillip Napieralski
 *  - Wrote the code
 */

#pragma once 

#include "Player.h" /* includes basically everything */

enum ShazamMode { MAIN_MENU = 0, PLAYING }; /* Not used currently */

#define ENEMY_POV_WINDOW_HEIGHT		150
#define ENEMY_POV_WINDOW_WIDTH		ENEMY_POV_WINDOW_HEIGHT*(4.0f/3.0f)
#define NUM_PLAYERS					1 /* We want multiplayer support later eh? :) */

#define STARTING_FUEL				100

class Shazam
{
private:
	Camera topCam;
	ShazamMode mode;
	Universe universe; /* Create the universe */
	Player player;
	Font font;
	Font bigFont; 
	float timer;
	bool oob;     /* out of bounds (out of gray box) */
	bool playerDead;
	bool won;

	void drawWonGame()
	{
		string sz = "NICE! You win! Go again [Y/N]?";
		int w = int((g_screenWidth >> 1) - ((float)sz.length()*(float)font.width(' '))/2 - 1);
		int h = (g_screenHeight >> 1) + 50;

		glColor3f( 0.5f, 0.1f, 1.0f );
		bigFont.draw( w, h, sz );
	}

	void drawLostGame()
	{
		string sz = "Sorry! You exploded! Try again [Y/N]?";
		int w = int((g_screenWidth >> 1) - ((float)sz.length()*(float)font.width(' '))/2 - 1);
		int h = (g_screenHeight >> 1) + 50;

		glColor3f( 0.5f, 1.0f, 0 );
		bigFont.draw( w, h, sz );
	}

	void drawOOBWarning()
	{
		string sz = "Turn back now! You will explode if you continue on this route!";
		float costimer = cosf(timer);
		int w = int((g_screenWidth >> 1) - ((float)sz.length()*(float)font.width(' '))/2 - 1);
		int h = (g_screenHeight >> 1) + 50;

		glColor3f( abs(costimer) , 0, 0 );
		font.draw( w, h, sz );

		timer += 0.05f;
		if( timer >= 360.0f )
			timer = -360.0f;
	}

	void drawObjectPov( Object obj )
	{
		setProjectionTo2D();
		glColor3f(0,0,0);

		/* Draw a black canvas for our preview window */
		glRecti(g_screenWidth, g_screenHeight,
			g_screenWidth-ENEMY_POV_WINDOW_WIDTH, g_screenHeight-ENEMY_POV_WINDOW_HEIGHT);

		/* Clear all depth information so we can draw on said black canvas */
		glClear( GL_DEPTH_BUFFER_BIT );

		glColor3f(1,1,1);
		glBegin(GL_LINE_LOOP);
			glVertex2f( g_screenWidth-ENEMY_POV_WINDOW_WIDTH,
						g_screenHeight-ENEMY_POV_WINDOW_HEIGHT);
			glVertex2f( g_screenWidth-ENEMY_POV_WINDOW_WIDTH,
						g_screenHeight );
			glVertex2f( g_screenWidth,
						g_screenHeight );
			glVertex2f( g_screenWidth, g_screenHeight-ENEMY_POV_WINDOW_HEIGHT );
		glEnd();

		setViewport( g_screenWidth - ENEMY_POV_WINDOW_WIDTH, g_screenWidth, 
			g_screenHeight - ENEMY_POV_WINDOW_HEIGHT, g_screenHeight );

		obj.setLookAt( player.position );
		obj.setNearPlane( 2.0f ); /* Make sure we don't see just the objects insides :p */
		obj.applyShape();
		obj.update();
		if( oob )
		{
			glColor3f( 0.5f, 0.0f, 0.0f );
			glPushMatrix();
				universe.drawOOBGrid3D( UNIVERSE_SIZE + DEAD_ZONE );
			glPopMatrix();
		}
		player.drawBasedOnCamera();
		universe.draw();
		universe.drawStars();
	}

public:
	Shazam()
	{
		timer = 0; won = oob = false;
		font = Font(BITMAP_8X13); bigFont = Font(TIMES_ROMAN_24); 
		player.resetScore();
	};

	ShazamMode getMode(){ return mode; }
	
	bool isPlayerDead()
	{
		return playerDead;
	}

	void killPlayer()
	{
		won = false;
		playerDead = true;
		player.stopMoving();
	}

	void revivePlayer()
	{
		won = false;
		playerDead = false;
		player.setFuel( STARTING_FUEL );
	}

	void startGame()
	{ 
		//cout << "Loading..." << endl;
		player.resetScore();
		oob = false;
		revivePlayer();
		mode = PLAYING; 

		universe.generate();
		player.setDefault();
	}

	void goToMainMenu()
	{ 
		mode = MAIN_MENU; 
	}

	void winGame()
	{
		drawWonGame();
		won = true;
		player.stopMoving();
	}

	void drawUniverse()
	{
		player.applyShape();
		player.update(); 
		setViewport( 0, g_screenWidth, 0, g_screenHeight);
		if( oob )
		{
			glColor3f( 0.5f, 0.0f, 0.0f );
			glPushMatrix();
				universe.drawOOBGrid3D( UNIVERSE_SIZE + DEAD_ZONE );
			glPopMatrix();
		}
		universe.draw();
		universe.drawStars();

		/* draw player HUD */
		player.drawHud();
		if( oob && !playerDead )
			drawOOBWarning();
		if( playerDead && !won )
			drawLostGame();
		if( won )
			drawWonGame();

		drawObjectPov( universe.getFirstBadGuy() );

	}

	void updateGame()
	{
		if( universe.checkCollisionBadGuys( player ) >= 0 )		
			player.increaseScore( 10 );

		if( universe.checkCollisionPowerUps( player ) >= 0 )
		{
			player.increaseFuel( 40 );
		}

		if( universe.checkOOB( player ) )
		{
			if( universe.checkDeadZone( player ) )
			{
				if( !playerDead )
					player.increaseFuel( -10 ); /* lose fuel FAST */
			}
			oob = true;
		}
		else
		{
			oob = false;
		}

		if( player.checkFuel() <= 0.0f )
			killPlayer();

		/* Travelling faster uses more fuel! */
		if( !playerDead && !won)
			player.increaseFuel( abs(player.getVelocity()) * -0.1f - 0.015f );

		if( universe.getNumBadGuys() <= 0 && !playerDead )
			winGame();

		universe.updateRotations();
		//player.addRotation( 0, 0, player.getLookVelocity().x );
	}

	bool hasWon(){ return won; }

	void playerYaw( float amt )
	{ 
		//player.addRotation( 0.0f, amt, 0.0f );
		player.yaw(amt);
	}
	void playerPitch( float amt )
	{
		//player.addRotation( amt, 0.0f, 0.0f );
		player.pitch( amt ); 
	}

	void playerRoll( float amt )
	{
		player.accelRoll( amt );
	}

	void playerRoll2( float amt )
	{
		player.roll( amt );
	}

	void movePlayerForward( float amt )
	{	
		if( !playerDead && !won)
			player.accelForward( amt ); 
	}
	void movePlayerRight( float amt )
	{ 
		if( !playerDead && !won )
			player.moveRight( amt ); 
	}

	void toggleHitBoxes()
	{
		universe.toggleHitBoxVisible();
	}

};


