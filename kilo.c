#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void die(const char *s){
	perror(s);
	exit(1);
}

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
	//uses original instance of the termios to set attr
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) die("tcsetattr");
	
}

void enableRawMode(){
	
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcsetattr");
	//gets attr at current state of orig_termios
	atexit(disableRawMode);
	//auto exit rawmode when program ends
	struct termios raw = orig_termios;

	raw.c_iflag &= ~(BRKINT |ICRNL | INPCK | ISTRIP |IXON);
	//input flag, disables ctrl s and q, also disables ctrl c v..., really just ICRNL and IXON doing the most here
	//BRKINT, INPCK, ISTRIP, CS8 are all from termoios, setting these all for rawmode convention
	raw.c_oflag &= ~(OPOST);
	//turns off all output processing, ex carriage return and newline
	raw.c_cflag |= (CS8);
	//bitmask for setting eight  bits per byte, really dependent on type of system, but set just in case
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	//bitwise operator ^
	//ICANON disables the canon mode
	//
	//
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;


	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die ("tcsetattr");
	//applies terminal changes to the terminal
}


int main() {

	enableRawMode();

	//the read function reads in one byte of input and keeps doing that, returns the value of bytes it read
	while (1){
		char c = '\0';
		if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
		if (iscntrl(c)) {
			printf("%d\r\n", c);
		}

		else {
			printf("%d ('%c')\r\n", c, c);
		}

		if (c == 'q') break;
	}

	return 0;

}
