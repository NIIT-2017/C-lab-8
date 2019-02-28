#include"decompress.h"

void unPack(unsigned char ch, unsigned char *buf)
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

struct SYM* buildTree(struct SYM *psym[], int N)
{
	struct SYM * temp = (struct SYM*)malloc(sizeof(struct SYM));
	temp->freq = psym[N - 2]->freq + psym[N - 1]->freq;
	temp->left = psym[N - 1];
	temp->right = psym[N - 2];
	temp->code[0] = '\0';
	if (N == 2)
		return temp;

	int i = N - 3;
	while (i >= 0 && (temp->freq >= psym[i]->freq))
	{
		psym[i + 1] = psym[i];
		i--;
	}
	psym[i + 1] = temp;
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

int f1(struct SYM *node, FILE *pf_101, char magic)
{	if (node->left == 0 && node->right == 0)
		return node->ch;
	else
	{
		int temp = '0';
		fread(&temp, 1, 1, pf_101);
		if (temp == magic)
			return EOF;
		if (temp == '0')
			return f1(node->left, pf_101, magic);
		if (temp == '1')
			return f1(node->right, pf_101, magic);
	}
}

void restoreFrom101File(const char * fileName, struct SYM * root, char magic)
{
	FILE *pf_101;
	pf_101 = fopen("101.txt", "rb");
	if (pf_101 == NULL) { printf("file open error\n"); exit(EXIT_FAILURE); }
	
	FILE *pf_in;
	pf_in = fopen(fileName, "wb");
	if (pf_in == NULL) { printf("file open error\n"); exit(EXIT_FAILURE); }
	
	int ch = 0;
	while ((ch = f1(root, pf_101, magic))!= EOF)
			fwrite(&ch, 1, 1, pf_in);
	
	fclose(pf_in);
	fclose(pf_101);
}