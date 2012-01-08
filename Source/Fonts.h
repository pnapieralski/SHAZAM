/*
[]~~~~~~~~~~~~~~~~~~~~~~~~~~~ fonts.h ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[]
[]                                                                 []
[]                          Font Class                             []
[]                                                                 []
[]                       James L. Richards                         []
[]                Last Update: September 14, 2006                  []
[]_________________________________________________________________[]
*/

/* November 30, 2008
   Phillip Napieralski
   - MODIFICATION: Drawing fonts no longer sets the cursor to invisible
 */

#include "glut.h"
#include <string>

using namespace std;

#ifndef FONTS
#define FONTS

//          Pointers to GLUT Bitmapped Fonts
//          --------------------------------
const void* BITMAP_8X13 = GLUT_BITMAP_8_BY_13;
const void* BITMAP_9X15 = GLUT_BITMAP_9_BY_15;
const void* TIMES_ROMAN_10 = GLUT_BITMAP_TIMES_ROMAN_10;
const void* TIMES_ROMAN_24 = GLUT_BITMAP_TIMES_ROMAN_24;
const void* HELVETICA_10 = GLUT_BITMAP_HELVETICA_10;
const void* HELVETICA_12 = GLUT_BITMAP_HELVETICA_12;
const void* HELVETICA_18 = GLUT_BITMAP_HELVETICA_18;

class Font
{
public:
	Font() { currentFont = (void *)BITMAP_8X13; }
	// Makes BITMAPES_8X13 the default font.

	Font(const void * f) { currentFont = (void *)f; }
	// Makes f the default font.

	int width(unsigned char ch);
	// Returns the width of character ch.
	
	void draw(float x, float y, unsigned char ch);
	// Displays the character ch at screen position (x,y).

	void draw(float x, float y, string str);
	// Displays the string str at screen position (x,y).

private:
	void* currentFont; // address of the current font
};

int Font::width(unsigned char ch)
{
	return glutBitmapWidth(currentFont, ch);
}

void Font::draw(float x, float y, unsigned char ch)
{
	//glutSetCursor(GLUT_CURSOR_NONE);
	glRasterPos2f(x, y);
	glutBitmapCharacter(currentFont, ch);
	glFlush();
	//glutSetCursor(GLUT_CURSOR_TEXT);
}

void Font::draw(float x, float y, string str)
{
	//glutSetCursor(GLUT_CURSOR_NONE);
	glRasterPos2f(x, y);
	for (int k = 0; k < int(str.size()); k++)
		glutBitmapCharacter(currentFont, str[k]);
	glFlush();
	//glutSetCursor(GLUT_CURSOR_TEXT);
}

#endif
