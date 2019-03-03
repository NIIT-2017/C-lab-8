#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int packFile(struct SYM *buf, int tail, char* filename, long symCounter)
{
	FILE *fp_101 = fopen("101.101", "rb");
	if (fp_101 == NULL)														// check file opening
	{
		puts("ERROR! Cannot open .101 file!");
		return 1;
	}

	FILE *fp_compressed = fopen("Compressed.pivrar", "wb");
	if (fp_compressed == NULL)												// check file opening
	{
		puts("ERROR! Cannot create compressed file!");
		return 1;
	}

	fwrite("pivrar", sizeof(char), 6, fp_compressed);						// write signature
	fwrite(&symCounter, sizeof(long), 1, fp_compressed);					// write symbols count

	for(int i=0;i<256;i++)													// symbols and codes
	{
		/*if (buf[i].freq == 0.0)
		{
			fwrite(&buf[i].freq, sizeof(float), 1, fp_compressed);
			break;
		}
		else
		{*/
			fwrite(&buf[i].freq, sizeof(float), 1, fp_compressed);
			fwrite(&buf[i].ch, sizeof(unsigned char), 1, fp_compressed);
			
		//}
	}
	// fwrite("t", sizeof(char), sizeof("t"), fp_compressed);
	fwrite(&tail, sizeof(int), 1, fp_compressed);							// tail
	

	int j;
	for (int i = 0; i < strlen(filename); i++)								// take .XXX from filename
	{
		if (filename[i] == '.')
		{
			for (j = 0; j < strlen(filename) && i< strlen(filename); j++)
			{
				filename[j] = filename[i];
				i++;
			}
			filename[j] = '\0';
		}
	}
	int filenameLenght= strlen(filename);
	fwrite(&filenameLenght, sizeof(int), 1, fp_compressed);
	fwrite(filename, sizeof(char), sizeof(filename), fp_compressed);		// write .XXX
	

	char arr[8];															// pack
	while (!feof(fp_101))
	{
		fread(arr, sizeof(char), 8, fp_101);
		char letter=pack(arr);
		fwrite(&letter, sizeof(char), 1, fp_compressed);
	}


	fclose(fp_101);
	fclose(fp_compressed);
}


unsigned char pack(char buf[])
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