#include <inttypes.h> // C99 7.8 for PRIu64
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

struct termios original_termios;

// Make terminal send input characters immediately.
//
// Returns the character code designating EOF.
cc_t terminal_use_raw(void)
{
	// save original settings
	tcgetattr(STDIN_FILENO, &original_termios);

	struct termios raw = original_termios;

	// disable buffered I/O and echo
	raw.c_lflag &= ~(ICANON | ECHO);
	// one byte at a time, no timer
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;

	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
	return raw.c_cc[VEOF];
}

void terminal_reset(void)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
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
	char c;
	uint64_t s, s_prev = us_timestamp();
	cc_t RAW_EOF;

	RAW_EOF = terminal_use_raw();
	atexit(terminal_reset);

	while (read(STDIN_FILENO, &c, 1) == 1 && c != RAW_EOF)
	{
		s = us_timestamp();
		printf("%" PRIu64 "\t\t0x%x\n", s-s_prev, c);
		s_prev = s;
	}

	return 0;
}
