#include "Header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	FILE *fp;
	int ac;
	int count = 0;
	char buf[256];
	struct SYM * psym[512];
	struct SYM *root = NULL;
	struct SYM arr[256];
	struct stat st;
	char filename[] = "2.bmp";
	char fileExt[] = "bmp";
	char file101[] = "1.101";
	char fileOut[] = "1.dsg";
	char newFile101[] = "2.101";
	fp = fopen(argv[1], "rt");
	if (!fp)
	{
		perror("File: ");
		return 1;
	}
	while ((ac = fgetc(fp)) != EOF)
	{
		arr[(int)ac].count++;
		count++;
	}
	for (int i = 255; i >= 0; i--)
	{
		arr[i].symbol = (char)i;
		arr[i].freq = (float)arr[i].count / count;
		arr[i].code[0] = 0;
		psym[i] = (struct SYM*)malloc(sizeof(struct SYM));
		psym[i] = &arr[i];	
	}
	qsort(psym, 256, sizeof(struct SYM*), compareByFreq);
	int i = 0;
	while (psym[i]->freq > 0)
	{
		i++;
	}
	int countSymbols = i;
	root = buildTree(psym, countSymbols,countSymbols);
	makeCodes(root);
	int tail = fillCodeFile(filename,file101, arr);
	stat(filename, &st);
	long fileSize = st.st_size;
	writeSignature(fileOut, countSymbols, psym, tail, fileSize, fileExt,count);
	writeContent(file101, fileOut);
	return 0;
}