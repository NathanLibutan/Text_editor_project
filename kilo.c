#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
	//uses original instance of the termios to set attr
	
}

void enableRawMode(){

	tcgetattr(STDIN_FILENO, &orig_termios);
	//gets attr at current state of orig_termios
	atexit(disableRawMode);
	//auto exit rawmode when program ends
	struct termios raw = orig_termios;

	raw.c_iflag &= ~(ICRNL | IXON);
	//input flag, disables ctrl s and q,
	raw.c_oflag &= ~(OPOST);
	//turns off all output processing, ex carriage return and newline
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	//bitwise operator ^
	//ICANON disables the canon mode

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	//applies terminal changes to the terminal
}


int main() {

	enableRawMode();

	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q'){
		if (iscntrl(c)) {
			printf("%d\n", c);
		}

		else {
			printf("%d ('%c')\n", c, c);
		}
	}

	return 0;

}
