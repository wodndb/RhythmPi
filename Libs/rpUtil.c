#include "rpUtil.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

//////
// TO DO:
// 1. GPIO
//////

////////////////////////////////////////////////////////////
//                                                        //
// Rhythm Pi Custom Input Functions (Keyboard)            //
//                                                        //
////////////////////////////////////////////////////////////

///
// Function Description
//
// Name: rp_kbhit
//
// Arguments: void
// 
// Description: check if keyboard is hitted.
//
// Returns: int
///
int rp_kbhit(void) {
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF) {
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}

///
// Function Description
//
// Name: rp_getch
//
// Arguments: void
// 
// Description: return character from standard input buffer (stdin)
//
// Returns: int
///
int rp_getch(void) {
	struct termios oldt, newt;
	int ch;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	
	return ch;
}

///
// Function Description
//
// Name: rp_fpurge_stdin
//
// Arguments: void
// 
// Description: empty standard input buffer (stdin)
//
// Returns: void
///
void rp_fpurge_stdin(void) {
	struct termios oldt, newt;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	
	while(getchar() != EOF) {}

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
