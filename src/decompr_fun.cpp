#include "decompr.h"

void printStruct(PSYM syms, int uniqueCount)
{
	for (int i = 0;i < uniqueCount;i++)
	{
		printf("%c\t%f\t%s\n", syms[i].ch, syms[i].freq, syms[i].code);
	}

}

PSYM buildTree(PSYM psym[], int size)
{
	PSYM temp = (PSYM)malloc(sizeof(SYM)); // create temporary node
	temp->freq = psym[size - 2]->freq + psym[size - 1]->freq; // write the sum of the frequencies of the last and next-to-last psym elements
	temp->left = psym[size - 1]; //associate the created node with the last two nodes
	temp->right = psym[size - 2];
	temp->code[0] = '\0';
	if (size == 2) // we formed a root element with frequency 1.0
		return temp;

	for (int i = size - 3;i >= 0;i--) // add temp to the psym, keeping the frequency decreasing order
	{
		if (psym[i]->freq <= temp->freq)
		{
			psym[i + 1] = psym[i];
			psym[i] = temp;
		}
	}
	return buildTree(psym, size - 1);
}

void makeCodes(PSYM root)
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

void unPack(unsigned char buf[], unsigned char ch)
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

static unsigned int getChar(FILE *fp_101, PSYM root, int *size)
{
	if (root->left == NULL && root->right == NULL)
	{
		(*size) -= strlen(root->code);
		return root->ch;
	}
	else
	{
		char ch1 = 0;
		fread(&ch1, sizeof(char), 1, fp_101);
		if (ch1 == '0')
			return getChar(fp_101, root->left, size);
		if (ch1 == '1')
			return getChar(fp_101, root->right, size);
	}
}

void decoding(const char* file1, const char *file2, PSYM root, int size)
{
	FILE *fp_101;
	FILE *fp_out;
	unsigned int ch;

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

	while (size > 0)
	{
		ch = getChar(fp_101, root, &size);
		fwrite(&ch, sizeof(char), 1, fp_out);
	}
	fclose(fp_out);
	fclose(fp_101);
}