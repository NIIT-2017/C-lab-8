#include "Header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

struct SYM* createTree(struct SYM*root, struct SYM item)
{
	if (root == NULL)
	{
		root = (struct SYM*)malloc(sizeof(struct SYM));
		root->symbol = item.symbol;
		root->count = item.count;
		root->freq = item.freq;
		root->left = root->right = NULL;
	}
	else if (root->freq > item.freq)
		root->left = createTree(root->left, item);
	else
		root->right = createTree(root->right, item);

	return root;
}

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
				for (int j = countSymbols *2 -N -1; j >= i; j--)
					psym[j+1] = psym[j];
				psym[i] = temp;
				break;
			}
	}
	return buildTree(psym, N - 1,countSymbols);
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

void printTree(struct SYM*root)
{
	if (root)
	{
		printTree(root->left);
		if ((root->freq) > 0)
			printf("Symbol: '%c' frequency : %5.3f%%\n", root->symbol, (root->freq) * 100);
		printTree(root->right);
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

int fillCodeFile(char *filename, char *file101, struct SYM *syms)
{
	FILE *fp_in, *fp_out;
	unsigned char symbol;
	int j;
	int tail = 0;
	int count = 0;
	fp_in = fopen(filename, "rb");
	fp_out = fopen(file101, "w+");
	while ((j = fread(&symbol, sizeof(unsigned char), 1, fp_in)) == 1)
	{
		for (int i = 0; i < 256; i++)
			if (syms[i].symbol == symbol)
			{
				fputs(syms[i].code, fp_out);
				count += strlen(syms[i].code);
				break;
			}
	}
	if (count % 8 != 0)
	{
		tail = count % 8;
		for (int i = 0; i < (8 - tail); i++)
		{
			fwrite("0", sizeof(char), 1, fp_out);
		}
		return 8 - tail;
	}
	fcloseall();
	return 0;
}

void writeSignature(char *file_out,int countSymbols,struct SYM *psym[], int tail,long fileSize,char *fileExt,int count)
{
	FILE *fp2;
	int i = 0;
	int j = 0;
	fp2 = fopen(file_out, "wb");
	fwrite("dsg", sizeof(char),3,fp2);
	fwrite(&countSymbols, sizeof(int), 1, fp2);
	while (psym[i]->freq > 0)
	{
		if (psym[i]->freq > 0 && psym[i]->symbol != -2)
		{
			fwrite(&psym[i]->symbol, sizeof(char), 1, fp2);
			fwrite(&psym[i]->freq, sizeof(float), 1, fp2);
			
		}
		i++;
	}
	fwrite(&tail, sizeof(int), 1, fp2);
	fwrite(&fileSize, sizeof(long), 1, fp2);
	fwrite(fileExt, sizeof(char), 3, fp2);
	fcloseall();
}

void writeContent(char *file101, char *fileOut)
{
	unsigned char buf[8], ch;
	struct stat st;
	int i = 0;
	int j = 0;
	FILE *fp_in,*fp_out;
	fp_in = fopen(file101, "r");
	fp_out = fopen(fileOut, "ab");
	stat(file101, &st);
	long fileSize101 = st.st_size;
	fwrite(&fileSize101, sizeof(long), 1, fp_out);
	while (1)
	{
		for (i = 0; i < 8; i++)
		{
			buf[i] = fgetc(fp_in);
			
		}
		if (buf[0] != '1' && buf[0] != '0')
		{
			break;
		}
		ch = pack(buf);
		fwrite(&ch, sizeof(unsigned char), 1, fp_out);
	}
	fcloseall();
}

unsigned char pack(unsigned char buf[])
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

