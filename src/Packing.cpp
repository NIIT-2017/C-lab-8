#include "HaffTree.h"
#include "Packing.h"

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * FindFileExtention(const char * fileInitial, unsigned char * fileExtention)
{
	for (unsigned int i = 0; i < strlen(fileInitial); i++)
	{
		if ('.' == fileInitial[i])
		{
			for (int j = 0; j < 4; j++)
			{
				fileExtention[j] = fileInitial[i + 1];
				i++;
			}
		}
	}
	return fileExtention;
}


void MakeCodedFile(const char * fileInitial, const char * fileCoded, int countUniqSymb, SYM * syms)
{
	FILE *fp_in = fopen(fileInitial, "rb");		//checking file for reading
	if (!fp_in)
	{
		puts("Cannot read file");
		exit(-2);
	}
	FILE *fp_101 = fopen(fileCoded, "wb");		//checking file for writing
	if (!fp_101)
	{
		puts("Cannot write file");
		exit(-3);
	}
	int ch = 0;									//code of symbol from file
	while ((ch = fgetc(fp_in)) != -1)
	{
		for (int i = 0; i < countUniqSymb; i++)
			if (syms[i].ch == (unsigned char)ch)
			{
				fputs(syms[i].code, fp_101);	//printing string with code to the coded file
				break;							//stop searching
			}
	}
	fclose(fp_in);
	fclose(fp_101);
}

unsigned char CountingLengthOfTail(const char * fileCoded, int * pquantityOfLoopsForCompressing)
{
	FILE *fp_101 = fopen(fileCoded, "rb");	//checking file for reading
	if (!fp_101)
	{
		puts("Cannot read file");
		exit(-4);
	}
	int totalAmountOfCodedSymbols = 0, tailLength = 0;
	while (1)								//till the end of the file
	{
		(unsigned char)fgetc(fp_101);
		if (feof(fp_101)) break;
		totalAmountOfCodedSymbols++;		//increasing total amount of coded symbols by one
	}
	tailLength = totalAmountOfCodedSymbols % 8;
	*pquantityOfLoopsForCompressing = totalAmountOfCodedSymbols / 8;
	fclose(fp_101);
	return ((unsigned char)tailLength);
}


void MakeCompressedFile(const char * fileCoded, const char * fileResult, int quantOfLoopsForCompressing, 
						int tail, int countUniqSymb, SYM * syms, const char * fileInitial, long sizeOfOriginalFile)
{
	FILE *fp_101 = fopen(fileCoded, "rb");						//checking file for reading
	if (!fp_101)
	{
		puts("Cannot read file");
		exit(-5);
	}
	FILE *fp_out = fopen(fileResult, "wb");						//checking file for writing
	if (!fp_out)
	{
		puts("Cannot write file");
		exit(-6);
	}
	unsigned char fileExtention[5] = { 0 };
	//writing header
	fwrite("HfC", 3 * sizeof(unsigned char), 1, fp_out);		//signature, means Haffman Comperssor

	fwrite(&countUniqSymb, sizeof(int), 1, fp_out);				//amount of unique symbols
	for (int i = 0; i < countUniqSymb; i++)						//the table of frequencies
	{
		fwrite(&syms[i].ch, sizeof(unsigned char), 1, fp_out);
		fwrite(&syms[i].freq, sizeof(float), 1, fp_out);
	}
	fwrite(&tail, sizeof(unsigned char), 1, fp_out);			//length of the tail

	//finding file extention
	FindFileExtention(fileInitial, fileExtention);
	fwrite(fileExtention, 4 * sizeof(unsigned char), 1, fp_out);
	fwrite(&sizeOfOriginalFile, 1 * sizeof(long), 1, fp_out);

	//writing codes
	unsigned char arrOfEightElements[8] = { 0 };				//forming array of 8 symbols
	//for all elements before tail
	for (int i = 0; i <= quantOfLoopsForCompressing; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			arrOfEightElements[j] = (unsigned char)fgetc(fp_101);
		}
		fputc((int)Pack(arrOfEightElements), fp_out);
	}

	//writing tail
	for (int i = 0; i < tail; i++)
	{
		arrOfEightElements[i] = (unsigned char)fgetc(fp_101);	//forming array for tail
	}
	for (int i = tail; i < 8-tail; i++)
	{
		arrOfEightElements[i] = '0';							//adding nulls for the rest of array's elements
	}
	fputc((int)Pack(arrOfEightElements), fp_out);				//writing tail to the compressed file
	fclose(fp_101);
	fclose(fp_out);
}

unsigned char Pack(unsigned char buf[]) 
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