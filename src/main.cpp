#include <stdio.h>
#include <string.h>
#include "comp.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("No files!\n");
		return -1;
	}

	if (strcmp(argv[1], "c") == 0)
		comp(argv[2]);
	if (strcmp(argv[1], "d") == 0)
		decomp(argv[2]);

	return 0;
}