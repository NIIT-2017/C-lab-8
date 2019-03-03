#include "HeaderD.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

int main()
{
	struct SYM *psym[512];
	struct SYM * root = NULL;
	char newFile101[] = "2.101";
	char fileIn[] = "11.dsg";
	char fileOut[] = "1.bmp";
	int number = readConten(fileIn,newFile101,psym);
	root = buildTree(psym, number+1, number+1);
	makeCodes(root);
	fromNumberToSymbol(newFile101, psym, fileOut);
	return 0;
}
