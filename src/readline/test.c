#include <stdio.h>
#include <unistd.h>
#include <termios.h>

int main()
{
	long  c;
	struct termios termios;

	tcgetattr(STDIN_FILENO, &termios);
	c = 0;
	termios.c_lflag = termios.c_lflag & (~(ECHO | ICANON));
	tcsetattr(STDIN_FILENO,TCSANOW,&termios);
	while(1)
	{
		read(1, &c,6);
		printf("%ld\n", c);
		if (c == 4283163)
			puts("UP");
		if (c == 4348699)
			puts("DOWN");
		c = 0;
	}
}