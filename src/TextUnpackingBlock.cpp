#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SYM.h"
#include "HaffTree.h"
#include "TextPackingBlock.h"
#include "TextUnpackingBlock.h"

unsigned char * unpack(unsigned char symb, unsigned char *buf)
{
	union CODE decode;
	decode.ch = symb;
	buf[0] = decode.byte.b1 + '0';
	buf[1] = decode.byte.b2 + '0';
	buf[2] = decode.byte.b3 + '0';
	buf[3] = decode.byte.b4 + '0';
	buf[4] = decode.byte.b5 + '0';
	buf[5] = decode.byte.b6 + '0';
	buf[6] = decode.byte.b7 + '0';
	buf[7] = decode.byte.b8 + '0';

	return buf;
}

void MakeDecompressedFile(SYM * SYMarray, const char * packedFileName, const char * unpacked101File, int * SYMcount, int * tail, char * extention, unsigned long * origFileSize)
{
	FILE * packedFile = fopen(packedFileName, "rb");
	if (!packedFile)
	{
		printf("cannot open %s", packedFileName);
		exit(-10);
	}
	FILE *UP101File = fopen(unpacked101File, "wb");
	if (!UP101File)
	{
		printf("cannot open %s", unpacked101File);
		exit(-11);
	}
	fseek(packedFile, 0, SEEK_END);
	unsigned long size = ftell(packedFile);
	fseek(packedFile, 0, SEEK_SET);

	char check[4] = { 0 };
	fread(check, 3 * sizeof(char), 1, packedFile);
	if (strcmp(check, "HAF") != 0)
	{
		printf("%s is wrong file to unpacking", packedFileName);
		exit(-12);
	}

	fread(SYMcount, 1 * sizeof(int), 1, packedFile);
	for (int i = 0; i < *SYMcount; i++)
	{
		fread(&SYMarray[i].ch, sizeof(char), 1, packedFile);
		fread(&SYMarray[i].freq, sizeof(float), 1, packedFile);
	}
	fread(tail, 1 * sizeof(char), 1, packedFile);
	fread(extention, 4 * sizeof(char), 1, packedFile);
	fread(origFileSize, 1 * sizeof(long), 1, packedFile);

	size -= ftell(packedFile);

	unsigned char symb = 0;
	unsigned char buf[8] = { 0 };

	for (long i = 0; i < size-1; i++)
	{
		symb = fgetc(packedFile);
		unpack(symb, buf);
		for (int j = 0; j < 8; j++)
		{
			fputc(buf[j], UP101File);
		}
	}
	symb = fgetc(packedFile);
	unpack(symb, buf);
	for (int j = 0; j < 8 - *tail; j++)
	{
		fputc(buf[j], UP101File);
	}

	fclose(UP101File);
	fclose(packedFile);
}

void Decoding(const char * unpacked101File, int SYMcount, SYM * SYMarray, char * extention, char * fileName)
{
	FILE *UP101File = fopen(unpacked101File, "r");
	if (!UP101File)
	{
		printf("Cannot open %s", unpacked101File);
		exit(-13);
	}
	strcat(fileName, extention);
	FILE *decodedFile = fopen(fileName, "w");
	if (!decodedFile)
	{
		printf("Cannot open %s", fileName);
		exit(-14);
	}
	char arrTmp[216] = { 0 };
	while (!feof(UP101File))
	{
		for (int i = 0; i < 216; i++)
		{
			arrTmp[i] = fgetc(UP101File);

			int flag = 0;
			for (int j = 0; j < SYMcount; j++)
			{
				if (strcmp(arrTmp, SYMarray[j].code) == 0)
				{
					fputc(SYMarray[j].ch, decodedFile);

					for (int k = 0; k <= i; k++)
					{
						arrTmp[k] = 0;
					}
					flag = 1;
					break;
				}
			}
			if (flag)
				break;
		}
	}
	fclose(UP101File);
	fclose(decodedFile);
}