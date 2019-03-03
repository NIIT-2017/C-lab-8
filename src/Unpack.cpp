#include "HaffTree.h"
#include "Packing.h"
#include "Unpack.h"
#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


unsigned char * Unpack(unsigned char codedSymb, char newBuf[8])
{
	union CODE decode;
	decode.ch = codedSymb;
	newBuf[0] = decode.byte.b1 + '0';
	newBuf[1] = decode.byte.b2 + '0';
	newBuf[2] = decode.byte.b3 + '0';
	newBuf[3] = decode.byte.b4 + '0';
	newBuf[4] = decode.byte.b5 + '0';
	newBuf[5] = decode.byte.b6 + '0';
	newBuf[6] = decode.byte.b7 + '0';
	newBuf[7] = decode.byte.b8 + '0';

	return newBuf;
}


void MakeDecompressedFile(SYM * newSyms, const char * fileName, int * pcountUniqSymb, unsigned char * pnewTail,
	unsigned char newFileExtention[5], long * pnewSizeOfOriginalFile)
{
	FILE * fp_pack = fopen(fileName, "rb");					//checking packed file for reading
	if (!fp_pack)
	{
		puts("Cannot read packed file");
		exit(-7);
	}
	FILE *fp_decompr = fopen("decompressed.txt", "wb");
	if (!fp_decompr)
	{
		puts("Cannot write decompressed file");
		exit(-8);
	}
	//finding the length of the file in bytes
	fseek(fp_pack, 0, SEEK_END);							//moving to the end of the compressed file
	long size = ftell(fp_pack);								//finding size

	fclose(fp_pack);
	//reading the header***********************************************
	fp_pack = fopen(fileName, "rb");						//checking packed file for reading
	unsigned char arrTmp[4] = { 0 };

	fread(arrTmp, 3 * sizeof(unsigned char), 1, fp_pack);
	if (strcmp(arrTmp, "HfC") != 0)							//checking if this file is a packed file
	{
		puts("This file is not a Haffman packed file!");
		exit(-9);
	}
	fread(pcountUniqSymb, 1 * sizeof(int), 1, fp_pack);		//reading the amount of unique symbols

	for (int i = 0; i < *pcountUniqSymb; i++)				//reading the table of frequencies to the array newSyms
	{
		fread(&newSyms[i].ch, sizeof(unsigned char), 1, fp_pack);
		fread(&newSyms[i].freq, sizeof(float), 1, fp_pack);
	}
	fread(pnewTail, 1 * sizeof(unsigned char), 1, fp_pack);	//reading the length of the tail
	fread(newFileExtention, 4 * sizeof(unsigned char), 1, fp_pack);
	fread(pnewSizeOfOriginalFile, 1 * sizeof(long), 1, fp_pack);
	long sizeOfHeader = ftell(fp_pack);						//finding size of header

	//reading the coded part*****************************************
	unsigned char newArrOfEightElements[8] = { 0 };			//forming array of 8 symbols
	unsigned char codedSymb = 0;

	for (long i = 0; i < (size - sizeOfHeader - 1); i++)	//writing to the coded decompressed file  
	{														//we don't touch last symb because it is tail
		codedSymb = fgetc(fp_pack);
		Unpack(codedSymb, newArrOfEightElements);			//calling unpacking func
		for (int j = 0; j < 8; j++)
		{
			fputc(newArrOfEightElements[j], fp_decompr);
		}
	}
	//writing the tail
	codedSymb = fgetc(fp_pack);								//reading rhe last symbol from the compressed file
	Unpack(codedSymb, newArrOfEightElements);				//calling unpacking func
	for (int j = 0; j < *pnewTail; j++)
	{
		fputc(newArrOfEightElements[j], fp_decompr);
	}
	fclose(fp_pack);
	fclose(fp_decompr);
}

void MakeDecodedFile(const char * fileDecompr, int newCountUniqSymb, SYM * newSyms,
	unsigned char * newFileExtention, unsigned char * nameOfDecodedFile)
{
	FILE *fp_decompr = fopen(fileDecompr, "rb");			//checking file for reading
	if (!fp_decompr)
	{
		puts("Cannot read decompressed file");
		exit(-10);
	}
	strcpy(nameOfDecodedFile, "decoded.");
	strcat(nameOfDecodedFile, newFileExtention);
	FILE *fp_decoded = fopen(nameOfDecodedFile, "wb");		//checking file for writing
	if (!fp_decoded)
	{
		puts("Cannot write decoded file");
		exit(-11);
	}
	int ch = 0;												//code of symbol from file
	char arrTmp[SIZE1] = { 0 };
	int check = 0;
	while (!feof(fp_decompr))
	{
		for (int i = 0; i < SIZE1; i++)						//checking for all array
		{
			arrTmp[i] = fgetc(fp_decompr);
			int flag = 0;
			for (int j = 0; j < newCountUniqSymb; j++)
			{

				if (strcmp(arrTmp, newSyms[j].code) == 0)
				{
					fputc((int)newSyms[j].ch, fp_decoded);
					check = (int)newSyms[j].ch;

					for (int k = 0; k <= i; k++)			//cleaning temporary array
					{
						arrTmp[k] = '\0';
					}
					flag = 1;
					break;
				}
				else
					continue;
			}
			if (flag)
				break;
		}
	}
	fclose(fp_decompr);
	fclose(fp_decoded);
}