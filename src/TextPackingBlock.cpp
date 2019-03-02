#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SYM.h"
#include "HaffTree.h"
#include "TextPackingBlock.h"

char * findFileExtention(const char * file, char * fileExtention)
{
	for (unsigned int i = 0; i < strlen(file); i++)
	{
		if (file[i+1] == '.')
		{
			for (int j = 0; file[i]; j++)
			{
				fileExtention[j] = file[i + 1];
				i++;
			}
		}
	}
	return fileExtention;
}

unsigned char pack(unsigned char *buf)
{
	union CODE code;
	code.byte.b1 = buf[0] - '0';
	code.byte.b2 = buf[1] - '0';
	code.byte.b3 = buf[2] - '0';
	code.byte.b4 = buf[3] - '0';
	code.byte.b5 = buf[4] - '0';
	code.byte.b6 = buf[5] - '0';
	code.byte.b7 = buf[6] - '0';
	code.byte.b8 = buf[7] - '0';
	return code.ch;
}

int Coding(char *origFileName, char *fp_101Name, SYM *SYMarray, int count)
{
	FILE *origFile = fopen(origFileName, "rb");
	if (!origFile)
	{
		printf("cannot open %s", origFileName);
		exit(-1);
	}
	FILE *fp_101 = fopen(fp_101Name, "wb");
	if (!fp_101)
	{
		printf("cannot open %s", fp_101Name);
		exit(-2);
	}
	char ch = 0;
	unsigned long tail = 0;
	while ((ch = fgetc(origFile)) != -1)
	{
		for (int i = 0; i < count; i++)
			if (SYMarray[i].ch == ch)
			{
				fputs(SYMarray[i].code, fp_101);
				tail += strlen(SYMarray[i].code);
				break;
			}
	}
	tail = (8 - tail % 8) % 8;
	for (int i = 0; i < tail; i++)
		fputc('0', fp_101);
	fclose(fp_101);
	fclose(origFile);
	return tail;
}

void MakePackedFile(char * packedFileName, char * fp_101Name, char *origFile, unsigned long origSize, int SYMcount, struct SYM * SYMarray, int tailLength)
{
	FILE *packedFile = fopen(packedFileName, "wb");
	if (!packedFile)
	{
		printf("cannot open %s", packedFileName);
		exit(-3);
	}
	FILE *fp_101 = fopen(fp_101Name, "rb");
	if (!fp_101)
	{
		printf("cannot open %s", fp_101Name);
		exit(-4);
	}

	fwrite("HAF", 3 * sizeof(char), 1, packedFile);

	fwrite(&SYMcount, sizeof(int), 1, packedFile);
	for (int i = 0; i < SYMcount; i++)
	{
		fwrite(&SYMarray[i].ch, sizeof(char), 1, packedFile);
		fwrite(&SYMarray[i].freq, sizeof(float), 1, packedFile);
	}
	fwrite(&tailLength, sizeof(char), 1, packedFile);

	char origExtension[5] = { 0 };
	findFileExtention(origFile, origExtension);
	fwrite(origExtension, 4 * sizeof(char), 1, packedFile);
	fwrite(&origSize, 1 * sizeof(long), 1, packedFile);
	unsigned char buf[8] = { 0 };
	int i = 0;
	while (fread(buf, 8, 1, fp_101))
	{
		fputc(pack(buf), packedFile);
		i++;
	}
	fclose(packedFile);
	fclose(fp_101);
}