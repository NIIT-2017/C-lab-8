#define _CRT_SECURE_NO_WARNINGS
#include "compressor.h"

struct SYM* buildTree(struct SYM *psym[], int N)
{
	struct SYM *temp = (struct SYM*)malloc(sizeof(struct SYM));
	temp->freq = psym[N - 2]->freq + psym[N - 1]->freq;
	temp->left = psym[N - 1];
	temp->right = psym[N - 2];
	temp->code[0] = 0;
	if (N == 2)
		return temp;
	for (int i = N - 3; i >= 0; i--)
	{
		if (psym[i]->freq <= temp->freq)
		{
			psym[i + 1] = psym[i];
			psym[i] = temp;
		}
	}
	return buildTree(psym, N - 1);
}

void makeCodes(struct SYM *root)
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

unsigned char* repack(unsigned char temp, unsigned char *buf)
{
	union CODE code;
	code.ch = temp;
	 buf[0] = code.byte.b1 + '0';
	 buf[1] = code.byte.b2 + '0';
	 buf[2] = code.byte.b3 + '0';
	 buf[3] = code.byte.b4 + '0';
	 buf[4] = code.byte.b5 + '0';
	 buf[5] = code.byte.b6 + '0';
	 buf[6] = code.byte.b7 + '0';
	 buf[7] = code.byte.b8 + '0';
	return buf;
}

static int index = 0;     // index of array members

void createBuf(PSYM buf, unsigned char ch, int count, int *word)
{
	int rep = 0;
	if (index == 0)
	{
		buf[index].ch = ch;
		buf[index].freq = buf[index].freq + ((float)1) / ((float)count);
		(*word)++;
		index++;
		return;
	}
	for (int i = 0; i < index; i++)
	{
		if (buf[i].ch == ch)
			rep++;
	}
	for (int i = 0; i < index; i++)
	{
		if (buf[i].ch != ch && rep != 1)
		{
			buf[index].ch = ch;
			buf[index].freq = buf[index].freq + ((float)1) / ((float)count);
			index++;
			(*word)++;
			break;
		}
		else if (buf[i].ch == ch && rep == 1)
			buf[i].freq = buf[i].freq + ((float)1) / ((float)count);
	}
}

void printBuf(PSYM buf, int word)
{
	for (int i = 0; i < word; i++)
		printf("%c %f %s\n", buf[i].ch, buf[i].freq, buf[i].code);
}

static int compar(const void* p1, const void* p2)
{
	if (((PSYM)p2)->freq > ((PSYM)p1)->freq)
		return 1;
	if (((PSYM)p2)->freq < ((PSYM)p1)->freq)
		return -1;
	if (((PSYM)p2)->freq == ((PSYM)p1)->freq)
		return 0;
}

void bufSort(PSYM str, int size)
{
	qsort(str, size, sizeof(TSYM), compar);
}

static unsigned int getChar(FILE *fp_101, PSYM root)
{
	if (root->left == NULL && root->right == NULL)
		return root->ch;
	else
	{
		char ch1;
		fread(&ch1, 1, 1, fp_101);
		if (ch1 != '0' && ch1 != '1')
			return EOF;
		if (ch1 == '0')
			return getChar(fp_101, root->left);
		if (ch1 == '1')
			return getChar(fp_101, root->right);
	}
}

void decoding(const char* file1, const char *file2, PSYM root)
{
	FILE *fp_101;
	FILE *fp_out;
	unsigned int ch;
	unsigned int char101 = 0;

	if ((fp_101 = fopen(file1, "rb")) == NULL)
	{
		puts("File not found!");
		exit(EXIT_FAILURE);
	}

	if ((fp_out = fopen(file2, "wb")) == NULL)
	{
		puts("File not found!");
		exit(EXIT_FAILURE);
	}
	
	while ((ch = getChar(fp_101, root)) != EOF)
	{
		fwrite(&ch, 1, 1, fp_out);
	}
	fclose(fp_out);
	fclose(fp_101);
}


