/*
[]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ console8.h ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[]
[]                                                                            []
[]                          Console Function Library                          []
[]                                                                            []
[]   The functions and color constants defined in this file can be used to    []
[]   change characteristics of the console window for an application.         []
[]                                                                            []
[]                            James L. Richards                               []
[]                       Last Update: August 25, 2008                         []
[]____________________________________________________________________________[]
*/

#include <conio.h>
#include <string>
using namespace std;

// Color Constants
// ================
const char 
   CONSOLE_BLACK = '0',    
   CONSOLE_BLUE = '1',        
   CONSOLE_GREEN = '2',       
   CONSOLE_AQUA = '3',
   CONSOLE_RED = '4',      
   CONSOLE_PURPLE = '5',      
   CONSOLE_BROWN = '6',       
   CONSOLE_LT_GRAY = '7',
   CONSOLE_GRAY = '8',     
   CONSOLE_LT_BLUE = '9',     
   CONSOLE_LT_GREEN  = 'A',   
   CONSOLE_LT_AQUA = 'B',
   CONSOLE_LT_RED = 'C',   
   CONSOLE_LT_PURPLE = 'D',   
   CONSOLE_YELLOW = 'E',      
   CONSOLE_WHITE = 'F';

void ConsoleClear()
////////////////////////////////////////////////////////////////////////////////
//	POSTCONDITION: Repaints the console window using the established background
//	color.
{
	system("cls");
}

void ConsoleTitle(const string& header)
////////////////////////////////////////////////////////////////////////////////
//	POSTCONDITION: Puts a header in the title bar for the console window.
{
	string cmd = "title " + header;
	system(cmd.data());
}

void ConsoleMode(int nCols, int nLines)
////////////////////////////////////////////////////////////////////////////////
//	POSTCONDITION: Makes the text area of the console window nCols wide and 
//	nLines high with no scrollbars.
{
	string cmd = "mode con ";
	char colStr[33], linStr[33];
	_itoa_s(nCols, colStr, 10);
	_itoa_s(nLines, linStr, 10);
	cmd = cmd + "cols=" + colStr + " lines=" + linStr;
	system(cmd.data());
}

void ConsoleColors(char background = CONSOLE_BLACK, 
				   char foreground = CONSOLE_WHITE)
////////////////////////////////////////////////////////////////////////////////
//	POSTCONDITION: Sets the background and foreground colors for displaying text
//	and repainting the console window.
{
	string cmd = string("color ") + background + foreground;
	system(cmd.data());
}

char ConsoleKeyPressWait()
////////////////////////////////////////////////////////////////////////////////
//	POSTCONDITION: Waits for a key to be pressed and returns the character
//  corresponding to that key.
{
	while (!_kbhit()) /* wait */;
	return _getch();
}

bool ConsoleKeyPressed()
////////////////////////////////////////////////////////////////////////////////
//	POSTCONDITION: Returns true if a key has been pressed; otherwise, false is
//	returned.
{
   bool keyWasPressed = _kbhit() > 0;
   if (keyWasPressed) _getch();
   return keyWasPressed;
}

bool ConsoleKeyPressed(char ch)
////////////////////////////////////////////////////////////////////////////////
//	POSTCONDITION: Returns true if a key has been pressed and ch is the
//	character corresponding to the pressed key; otherwise, false is returned
//	and ch is meaningless.
{
   bool keyWasPressed = _kbhit() > 0;
   if (keyWasPressed) ch = _getch();
   return keyWasPressed;
}
