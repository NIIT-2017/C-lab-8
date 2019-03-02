#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compressor.h"


int main(int argc, char* argv[])
{
	FILE *fp_in;
	FILE *fp_101;
	FILE *fp_out;
	TSYM syms[256] = { 0 };
	struct SYM *psym[256] = {NULL};
	unsigned char ch;
	unsigned int  count = 0;     //total characters
	int word = 0;       //Number of unique characters
	int tail = 0;		//tail size
	unsigned char buf[8];
	unsigned char temp = 0;

	if (argc != 3)
	{
		printf("Using: %s  and file name\n for compress enter \"cmp\"\n for decompress enter \"decmp\"\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (strcmp(argv[2], "cmp") == 0)			// COMPRESSOR
	{
		if ((fp_in = fopen(argv[1], "rb")) == NULL)
		{
			puts("File not found!\n");
			exit(EXIT_FAILURE);
		}

		while (fgetc(fp_in) != EOF)
			count++;
		rewind(fp_in);

		unsigned int i = 0;
		while (i < count)
		{
			ch = fgetc(fp_in);
			createBuf(syms, ch, count, &word);
			i++;
		}
		printf("Array was created!\n");
		bufSort(syms, word);
		printf("Array was sorted!\n");

		for (int i = 0; i < word; i++)
		{
			psym[i] = &syms[i];
		}

		struct SYM* root = buildTree(psym, word);
		makeCodes(root);
		printf("Tree was built!\n");
		printBuf(syms, word);

		int ch1;

		if ((fp_101 = fopen("in101.101", "wb")) == NULL)
		{
			puts("File not found!\n");
			exit(EXIT_FAILURE);
		}

		rewind(fp_in);

		unsigned int count2 = 0;  //number of characters in the file fp_101
		while ((ch1 = fgetc(fp_in)) != -1)
		{
			for (int i = 0; i < word; i++)
				if (syms[i].ch == (unsigned char)ch1)
				{
					fputs(syms[i].code, fp_101);
					count2 += strlen(syms[i].code);
					break;
				}
		}

		tail = 8 - count2 % 8;  //tail size
		for (int i = 0; i < tail; i++)
			fputc('0', fp_101);

		fclose(fp_101);
		printf("File 101 was created!\n");

		if ((fp_101 = fopen("in101.101", "rb")) == NULL)
		{
			puts("File not found!");
			exit(EXIT_FAILURE);
		}

		if ((fp_out = fopen("out.abc", "wb")) == NULL)
		{
			puts("File not found!");
			exit(EXIT_FAILURE);
		}
		// write headline
		fwrite("ABC", 3, 1, fp_out);
		temp = (unsigned char)(word - 1);
		fwrite(&temp, 1, 1, fp_out);
		for (int i = 0; i < word; i++)
		{
			fwrite(&syms[i].ch, 1, 1, fp_out);
			fwrite(&syms[i].freq, 4, 1, fp_out);
		}
		fwrite(&tail, 1, 1, fp_out);

		char str[5] = {0};
		int k, q;
		for (k = (strlen(argv[1])) - 3, q = 0; argv[1][k] != '\0'; k++, q++)
		{
			str[q] = argv[1][k];
		}
		str[q] = '\0';
		fwrite(str, 3, 1, fp_out);
		// headline end
		while (fread(buf, 8, 1, fp_101))
		{
			temp = pack(buf);
			fwrite(&temp, 1, 1, fp_out);
		}

		fclose(fp_out);
		fclose(fp_in);
		fclose(fp_101);

		printf("File was successfully compressed!\n");
		
		
	}
	else if (strcmp(argv[2], "decmp") == 0)					// DECOMPRESSOR
	{
		char arr[5] = { 0 };
		
		if ((fp_in = fopen(argv[1], "rb")) == NULL)
		{
			puts("File not found!");
			exit(EXIT_FAILURE);
		}
		//read headline
		fread(arr, 3, 1, fp_in);
		if (strcmp("ABC", arr) != 0)
		{
			printf("File cannot be unpacked!");
			exit(EXIT_FAILURE);
		}
		fread(&word, 1, 1, fp_in);
		word++;
		for (int i = 0; i < word; i++)
		{
			fread(&syms[i].ch, 1, 1, fp_in);
			fread(&syms[i].freq, 4, 1, fp_in);
		}
		fread(&tail, 1, 1, fp_in);
		fread(arr, 3, 1, fp_in);
		//end read headline
		printf("Headline was read!\n");

		for (int i = 0; i < word; i++)
		{
			psym[i] = &syms[i];
		}
		struct SYM* root = buildTree(psym, word);  // make tree
		makeCodes(root);
		printf("Tree was built!\n");
		printBuf(syms, word);

		if ((fp_101 = fopen("out101.101", "wb")) == NULL)
		{
			puts("File not found!");
			exit(EXIT_FAILURE);
		}
		
		while (fread(&temp, 1, 1, fp_in))
		{
			repack(temp, buf);
			fwrite(buf, 8, 1, fp_101);
		}
		printf("File 101 was written!\n");
		fseek(fp_101, -tail, SEEK_END);
		int q = 0;
		while (q < tail)
		{
			fwrite("7", 1, 1, fp_101);
			q++;
		}
		fclose(fp_101);

		for (int i = 0; argv[1][i] !='\0'; i++)
		{
			if (argv[1][i] == '.')
			{
				argv[1][i + 1] = '\0';
				break;
			}
		}
		strcat(argv[1], arr);

		decoding("out101.101", argv[1], root); //decoding
		printf("File 101 was decoded!\n");
		fclose(fp_in);
		printf("File was successfully decompressed!\n");
	}
	else
	{
		printf("Using: %s  and file name\n for compress enter \"cmp\"\n for decompress enter \"decmp\"\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	return 0;
}