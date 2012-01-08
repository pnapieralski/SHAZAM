/* Shazam.cpp 
 *
 * A cool game
 *
 * Phillip Napieralski
 * Last Updated: November 30, 2008
*/
#include "Shazam.h"
#include "Console8.h"

// You must include the fmod directory in your project
#include <fmod.hpp>
#include <../../api/inc/fmod_errors.h>

Shazam shazam;

/* this num divides the mouse sensitivity to make it smaller */
static float SCALE = 10.0f; 

void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        cerr << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << endl;
    }
}


void Display()                                              // glutDisplayFunc()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	shazam.drawUniverse();

	glutSwapBuffers();
}

void Idle()                                                     //glutIdleFunc()
{
	glutSetCursor( GLUT_CURSOR_NONE );
	shazam.updateGame();
	glutPostRedisplay();
}


static bool warped = false;
void PassiveMotion(int x, int y)							//glutPassiveMotionFunc()
{
	if( !warped )
	{
		int gW = g_screenWidth >> 1; /* faster divide by 2 */
		int gH = g_screenHeight >> 1;
		int deltaX = x - gW;
		int deltaY = y - gH;

		shazam.playerPitch( -deltaY / SCALE );
		shazam.playerYaw( -deltaX / SCALE );
		warped = true;
		glutWarpPointer( gW, gH );
	}
	else
		warped = false;

	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)             // glutKeyboardFunc()
{
	switch( key )
	{
	case 27:		/* ESC */
		exit(0); 
		break;

	case 'W':
	case 'w':
		shazam.movePlayerForward(.1f);
		break;
	case 'S':
	case 's':
		shazam.movePlayerForward(-.1f);
		break;

	case 'A':
	case 'a':
		shazam.playerRoll( .1f );
		break;
	case 'D':
	case 'd':
		shazam.playerRoll( -.1f );
		break;

	case 'Y':
	case 'y':
		if( shazam.getMode() == PLAYING && (shazam.hasWon() || shazam.isPlayerDead()) )
			shazam.startGame();
		break;
	case 'N':
	case 'n':
		if( shazam.getMode() == PLAYING && (shazam.isPlayerDead() || shazam.hasWon()) )
		{
			shazam.goToMainMenu();
			exit(0); // TEMP until a main menu is implemented! 
		}
		break;
	default:break;
	}
	
	glutPostRedisplay();
}

void Special(int key, int x, int y)                         // glutSpecialFunc()
{
	switch( key )
	{
	case GLUT_KEY_F1:
		shazam.toggleHitBoxes();
		break;

	case GLUT_KEY_UP:	/* up arrow */
		if( SCALE > 10.0f )
			SCALE -= 1.0f;
		break;
	
	case GLUT_KEY_DOWN:	/* down arrow */
		SCALE += 1.0f;
		break;
	default: break;
	}

	glutPostRedisplay();
}


void Reshape(int width, int height)
{
	g_screenWidth = width;
	g_screenHeight = height;
	glViewport(0, 0, g_screenWidth, g_screenHeight);
	glutPostRedisplay();
}

void main(int argc, char **argv)
{
	FMOD::System *fmodSystem;
	FMOD::Sound *music;
	FMOD::Channel *channel = 0;
	FMOD_RESULT result;
	unsigned int      version;
	
	// Create/init the system object
	result = FMOD::System_Create(&fmodSystem);
	ERRCHECK(result);

	result = fmodSystem->getVersion(&version);
	ERRCHECK(result);

	if( version < FMOD_VERSION) {
		cerr << "Error! Old version of FMOD!" << endl;
	}

	result = fmodSystem->init(16, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);

	result = fmodSystem->createSound("music.ogg", FMOD_HARDWARE, 0, &music);
	ERRCHECK(result);

	//result = music->setMode(FMOD_LOOP);
	//ERRCHECK(result);

	char *windowName = "SPACE HUNTER | ATTACK and ZAP ALIEN MASSES";
	
	ConsoleMode( 105, 20 );
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100, 100);

	glutCreateWindow( windowName );
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	glutSpecialFunc(Special);
	glutKeyboardFunc(Keyboard);
	glutPassiveMotionFunc(PassiveMotion);
	glutMotionFunc(PassiveMotion);

	/* Here's our INTRO Message */
	cout << "Welcome to SHAZAM!" << endl;
	cout << "-----------------------" << endl;
	cout << endl;
	cout << "STORY: See README.txt\n" << endl;
	cout << "HINT: Your ship can only carry up to 100% fuel at anytime,\
 so save those power-ups until you need them!" << endl;
	cout << endl;
	cout << "CONTROLS:" << endl;
	cout << "  W - Accelerate forward" << endl;
	cout << "  S - Accelerate backward" << endl;
	cout << "  A - Roll left" << endl;
	cout << "  D - Roll right" << endl;
	cout << "  ESC - Exit the game" << endl;
	cout << "  F1 - Toggle hit boxes" << endl;
	cout << "  Use the up and down arrow keys to modify mouse sensitivity" << endl;
	//cout << "  Mouse1 - Fire weapon" << endl;
	cout << "\nUse your mouse to control the ship's view" << endl;

	cout << "\nAre you ready [Press ENTER to continue]? ";
	ConsoleKeyPressWait();
	
	cout << "\n" << endl;

	glEnable(GL_DEPTH_TEST);

	fmodSystem->playSound(FMOD_CHANNEL_FREE, music, false, &channel);
	ERRCHECK(result);

	shazam.startGame();
	shazam.updateGame();
	glutFullScreen();
	glutMainLoop();




}
