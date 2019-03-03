#include "HeaderD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

struct SYM * buildTree(struct SYM *psym[], int N, int countSymbols)
{
	struct SYM *temp = (struct SYM *)malloc(sizeof(struct SYM));
	temp->freq = psym[N - 2]->freq + psym[N - 1]->freq;
	temp->left = psym[N - 1];
	temp->right = psym[N - 2];
	temp->code[0] = 0;
	temp->symbol = -2;
	if (N == 2)
	{
		return temp;
	}
	else
	{
		for (int i = 0; i < N; i++)
			if (temp->freq > psym[i]->freq)
			{
				for (int j = countSymbols * 2 - N - 1; j >= i; j--)
					psym[j + 1] = psym[j];
				psym[i] = temp;
				break;
			}
	}
	return buildTree(psym, N - 1, countSymbols);
}
void makeCodes(struct SYM * root)
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

int compareByFreq(const void *_a, const void* _b)
{
	struct SYM *a = *(struct SYM**)_a, *b = *(struct SYM**)_b;
	if ((a)->freq < (b)->freq)
		return 1;
	else
		return -1;
}

int  readConten(char *file_in, char *file_out, struct SYM *psym[])
{
	FILE *fp_in, *fp_out;
	fp_in = fopen(file_in, "rb");
	fp_out = fopen(file_out, "w");
	char buf[3];
	long count = 0;
	int number = 0;
	char *arr[10];
	char unbuf[8] = { 0 };
	char ch;
	unsigned char unsymbol;
	float frq;
	int i = 0;
	int tail = 0;
	long fileSize = 0;
	char fileExt[3];
	fread(buf, sizeof(char), 3, fp_in);
	fread(&number, sizeof(int), 1, fp_in);
	while (i < number)
	{
		psym[i] = (struct SYM*)malloc(sizeof(struct SYM));
		fread(&ch, sizeof(char), 1, fp_in);
		psym[i]->symbol = ch;
		fread(&frq, sizeof(float), 1, fp_in);
		psym[i]->freq = frq;
		psym[i]->count = 0;
		psym[i]->left = NULL;
		psym[i]->right = NULL;
		i++;
	}
	psym[i] = (struct SYM*)malloc(sizeof(struct SYM));
	strcpy(&psym[i]->code[0],"2");
	fread(&tail, sizeof(int), 1, fp_in);
	fread(&fileSize, sizeof(long), 1, fp_in);
	fread(fileExt, sizeof(char), 3, fp_in);
	fread(&count, sizeof(long), 1, fp_in);
	for (int k = 0; k < count / 8 -1 ; k++)
	{
		fread(&unsymbol, sizeof(unsigned char), 1, fp_in);
		unpack(unsymbol, unbuf);
		fwrite(unbuf, sizeof(unsigned char), 8, fp_out);
	}
	if (tail > 0)
	{
		fread(&unsymbol, sizeof(unsigned char), 1, fp_in);
		unpack(unsymbol, unbuf);
		unbuf[8 - tail] = 0;
		fwrite(unbuf, sizeof(unsigned char), 8 - tail, fp_out);
	}

	fcloseall();
	return number;
}

char * unpack(unsigned char symbol, char unbuf[])
{
	union CODE uncode;
	uncode.chr = symbol;
	unbuf[0] = uncode.byte.b1 + '0';
	unbuf[1] = uncode.byte.b2 + '0';
	unbuf[2] = uncode.byte.b3 + '0';
	unbuf[3] = uncode.byte.b4 + '0';
	unbuf[4] = uncode.byte.b5 + '0';
	unbuf[5] = uncode.byte.b6 + '0';
	unbuf[6] = uncode.byte.b7 + '0';
	unbuf[7] = uncode.byte.b8 + '0';

	return unbuf;
}

void fromNumberToSymbol(char *file101, struct SYM *psym[], char *fileOut)
{
	struct stat st;
	char sequence[8] = { 0 };
	stat(file101, &st);
	long fileSize = st.st_size;
	int i = 0;
	int j = 0;
	char ch[8] = { 0 };
	FILE *fp_101, *fp2txt;
	fp_101 = fopen(file101, "r");
	fp2txt = fopen(fileOut, "wb");
	while (fileSize)
	{
		fread(&sequence[j], sizeof(char), 1, fp_101);
		sequence[j + 1] = '\0';
		i = 0;
		while (strcmp(psym[i]->code,"2")!= 0)
		{
			
			if (strcmp(sequence, psym[i]->code) == 0 && psym[i]->symbol != -2)
			{
				fwrite(&psym[i]->symbol, sizeof(char), 1, fp2txt);
				sequence[0] = 0;
				j = -1;
			}
			i++;
		}
		
		j++;
		fileSize--;
	}
	fcloseall();
}