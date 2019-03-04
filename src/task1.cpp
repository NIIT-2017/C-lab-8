#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "task1.h"

SYM *makeTree(SYM *PSYM[], int number)
{
	SYM *temp;
	temp = (SYM*)malloc(sizeof(SYM));
	temp->freq = PSYM[number - 1]->freq + PSYM[number - 2]->freq;
	temp->code[0] = 0;
	temp->left = PSYM[number - 1];
	temp->right = PSYM[number - 2];

	if (number == 2)
		return temp;
	else 
	{
		for (int i = 0; i < number; i++)
			if (temp->freq > PSYM[i]->freq)
			{
				for (int j = number - 1; j > i; j--)
					PSYM[j] = PSYM[j - 1];

				PSYM[i] = temp;
				break;
			}
	}
	return makeTree(PSYM, number - 1);
}

void makeCodes(SYM *root)
{
	if (root->left)
	{
		strcpy(root->left->code, root->code);
		strcat(root->left->code, "0");
		makeCodes(root->left);
	}
	if (root->right)
	{
		strcpy(root->right->code, root->code);
		strcat(root->right->code, "1");
		makeCodes(root->right);
	}
}

void Sort(PSYM str, int size)
{
	qsort(str, size, sizeof(TSYM), compare);
}

int compare(const void*p1, const void *p2)
{
	if (((PSYM)p2)->freq > ((PSYM)p1)->freq)
		return 1;
	if (((PSYM)p2)->freq < ((PSYM)p1)->freq)
		return -1;
	if (((PSYM)p2)->freq == ((PSYM)p1)->freq)
		return 0;
}

void uncompress(unsigned char buf[], unsigned char ch)
{
	union CODE code;
	code.ch = ch;
	buf[0] = code.byte.b1 + '0';
	buf[1] = code.byte.b2 + '0';
	buf[2] = code.byte.b3 + '0';
	buf[3] = code.byte.b4 + '0';
	buf[4] = code.byte.b5 + '0';
	buf[5] = code.byte.b6 + '0';
	buf[6] = code.byte.b7 + '0';
	buf[7] = code.byte.b8 + '0';
}

static unsigned int getChar(FILE *fp101, PSYM root, int *size)
{
	if (root->left == NULL && root->right == NULL)
	{
		(*size) -= strlen(root->code);
		return root->ch;
	}
	else
	{
		char ch1 = 0;
		fread(&ch1, sizeof(char), 1, fp101);
		if (ch1 == '0')
			return getChar(fp101, root->left, size);
		if (ch1 == '1')
			return getChar(fp101, root->right, size);
	}
}

void decodes(const char* file1, const char *file2, PSYM root, int size)
{
	FILE *fp2;
	unsigned int ch;

	if ((fp2 = fopen(file1, "rb")) == NULL)
	{
		puts("File not found!");
		exit(EXIT_FAILURE);
	}
	FILE *fp3;
	if ((fp3 = fopen(file2, "wb")) == NULL)
	{
		puts("File not found!");
		exit(EXIT_FAILURE);
	}

	while (size > 0)
	{
		ch = getChar(fp2, root, &size);
		fwrite(&ch, sizeof(char), 1, fp3);
	}
	fclose(fp2);
	fclose(fp3);
}

