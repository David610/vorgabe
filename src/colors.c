#include "../lib/colors.h"
#include <stdio.h>

static unsigned int use_colored_output = 0;

void set_color(int index)
{
	if (use_colored_output) {
		printf("\033[%c;3%cm", '0' + index % 2, '1' + index % 6);
	}
}

void reset_color()
{
	if (use_colored_output) {
		printf("\033[0m");
	}
}
