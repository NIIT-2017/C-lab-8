#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

// to compress write in CMD "filename.txt compress"
// to decompress write in CMD "filename.txt decompress"

/*
struct SYM			
{
	unsigned char ch;
	float freq;
	char code[256];
	struct SYM *left;
	struct SYM *right;
};
*/

int main(int argc, char* argv[])
{
	// analyzator:

	if (argc < 2)									// check arguments
	{
		puts("Error! Not enough arguments! Please enter a filename in cmd for compress it!");
		puts("To compress file enter 'filename.txt compress' in CMD");
		puts("To decompress file enter 'filename.txt decompress' in CMD");
		puts("In theory file extension may be different (not only .txt) =)");
		puts("If you want to buy a full version please call 8-800-555-35-35");
		return 1;
	}
	
	if (strcmpi(argv[2], "compress") == NULL)		// COMPRESS
	{

		FILE *fp = fopen(argv[1], "rb");			// take filename from cmd
		if (fp == NULL)								// check file opening
		{
			puts("ERROR! Cannot read file!");
			return 1;
		}

		struct SYM buf[N * 2] = { 0 };				// structure array for letters
		initializeArray(buf);						// fill each structure in array

		char letter;								// one letter from file
		long long count = 0;						// how much letters in the file

		while (!feof(fp))
		{
			count++;
			letter = fgetc(fp);
			//fread(&letter, sizeof(char), 1, fp);
			makeSymArray(buf, letter);
		}
		fclose(fp);

		qsort(buf, N, sizeof(struct SYM), comp);

		calculateFreq(buf, count);		// redefine count of letters to part of 1 (calculate frequency)

		printArray(buf);


		// code generation:

		struct SYM *psym[N * 2];
		for (int i = 0; i < N * 2; i++)
		{
			psym[i] = &buf[i];
		}
		struct SYM *root;
		root = buildTree(psym, N);

		printf("Root frequency (in all tree) = %f\n", root->freq);
		makeCodes(root);

		// make .101 file:

		FILE *fp_in = fopen(argv[1], "rb");
		FILE *fp_101 = fopen("101.101", "wb");					// coded .101 file
		if (fp_101 == NULL)										// check file opening
		{
			puts("ERROR! Cannot create .101 file!");
			return 1;
		}

		unsigned long long count_101 = 0;
		long symCounter=0;
		while ((letter = fgetc(fp_in)) != -1)
		{
			for (int i = 0; i < N; i++)
				if (buf[i].ch == (unsigned char)letter)
				{
				//	fputs(buf[i].code, fp_101);				// write symbol code into .101 file
					fwrite(&buf[i].code, sizeof(char), strlen(buf[i].code), fp_101);
					count_101+=strlen(buf[i].code);
					symCounter++;
					break;
				}
		}
		int tail = 8-(count_101 % 8);							// tail lenght
		char tailLetter = '1';
		if (tail > 0)										// write tail to the .101 file
		{
			for (int k = 0; k < tail;k++)
				fwrite(&tailLetter, sizeof(char), 1, fp_101);
				//fputs("1", fp_101);
		}

		fclose(fp_in);
		fclose(fp_101);

		// pack file:

		int error = packFile(buf, tail, argv[1], symCounter);
		if (error == 1)
			return 1;
	}
	else if (strcmpi(argv[2], "decompress") == NULL)		// DE-COMPRESS
	{
	int decError=decompress(argv[1]);
	if (decError == 1)
		return 1;


	}
	else													// ERROR
	{
		puts("ERROR! UNKNOWN COMMAND!");
		return 1;
	}

	puts("Done successfully!");
	return 0;
}