#include <inttypes.h> // C99 7.8 for PRIu64
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>

struct termios original_termios;

void terminal_use_raw(void)
{
	// save original settings
	tcgetattr(0, &original_termios);

	struct termios new = original_termios;
	new.c_lflag &= ~ICANON; // disable buffered I/O
	new.c_lflag &= ~ECHO; // disable echo
	tcsetattr(0, TCSANOW, &new);
}

void terminal_reset(void)
{
	tcsetattr(0, TCSANOW, &original_termios);
}

uint64_t us_timestamp(void)
{
	struct timespec t;
	if (clock_gettime(CLOCK_MONOTONIC, &t) < 0)
	{
		perror("clock_gettime");
		exit(EXIT_FAILURE);
	}
	return (t.tv_sec * 1000000U) + (t.tv_nsec / 1000);
}

int main()
{
	int c;
	uint64_t s, s_prev = us_timestamp();

	terminal_use_raw();
	atexit(terminal_reset);

	while ((c = getchar()) != EOF)
	{
		s = us_timestamp();
		printf("%" PRIu64 "\t\t%c\n", s-s_prev, c);
		s_prev = s;
	}

	return 0;
}
