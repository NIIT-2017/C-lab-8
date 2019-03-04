#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "task1.h"


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("wrong name of the file\n");
		return 1;
	}
	else
	{
		{
			int count = 0;
			int number = 0;
			unsigned char current;
			SYM *PSYM[256];
			char buf[256];
			int mes[8];

			FILE *fp1;
			fp1 = fopen("original.txt", "rb");
			if (fp1 == NULL)
			{
				puts("File not open");
				return 0;
			}

			SYM *symbols = (SYM*)malloc(count * sizeof(SYM));

			while ((current = fgetc(fp1)) != EOF)
			{
				for (int i = 0; i < 256; i++)
				{
					if (symbols[i].ch == 0)
					{
						symbols[i].ch = current;
						count++;
						buf[i] = 1;
						number++;
						break;
					}
					if (current == symbols[i].ch)
					{
						buf[i]++;
						count++;
						break;
					}
				}

				for (int i = 0; i < count; i++)
				{
					symbols[i].freq = (float)buf[i] / count;
					PSYM[i] = &symbols[i];
				}
			}
			int compare(const void*a, const void *b);
			SYM *root = makeTree(PSYM, number); 
			makeCodes(root);

			FILE *fp2;
			fp2 = fopen("101.txt", "wb");
			rewind(fp1);
			while ((current = fgetc(fp1)) != EOF)
			{
				for (int i = 0; i < number; i++)
					if (current == symbols[i].ch)
						fputs(symbols[i].code, fp2);
			}
			fclose(fp2);
			fp2 = fopen("101.txt", "rb");
			fseek(fp2, 0, SEEK_END);
			int numByts = ftell(fp2);
			int tail = numByts % 8;
			fclose(fp2);
			fp2 = fopen("101.txt", "wb");
			fseek(fp2, 0, SEEK_END);
			for (int i = 0; i < tail; i++)
				puts("0");
			fclose(fp2);
			fp2 = fopen("101.txt", "rb");

			FILE *fp3;
			fp3 = fopen("Compressed", "wb");
			fwrite("Compressed", 3, 1, fp3);
			fwrite(&number, 1, 1, fp3);
			fwrite(&tail, 1, 1, fp3);

			for (int i = 0; i < number; i++)
			{
				fwrite(&symbols[i].ch, sizeof(SYM), 1, fp3);
				fwrite(&symbols[i].freq, sizeof(SYM), 1, fp3);
			}

			union CODE code;
			int j = 0;
			for (int i = 0; i < numByts - tail; i++)
			{
				mes[j] = fgetc(fp2);
				if (j == 7)
				{
					code.byte.b1 = mes[0] - '0';
					code.byte.b2 = mes[1] - '0';
					code.byte.b3 = mes[2] - '0';
					code.byte.b4 = mes[3] - '0';
					code.byte.b5 = mes[4] - '0';
					code.byte.b6 = mes[5] - '0';
					code.byte.b7 = mes[6] - '0';
					code.byte.b8 = mes[7] - '0';
					fputc(code.ch, fp3);
					j = 0;
				}
				j++;
			}
			fclose(fp2);
			fclose(fp3);
			return 0;
		}
	}