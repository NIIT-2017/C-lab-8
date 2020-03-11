#include "task1.h"

struct SYM* buildTree(struct SYM* psym[], int N)
{
	struct SYM* temp = (struct SYM*)malloc(sizeof(struct SYM));
	temp->freq = psym[N - 2]->freq + psym[N - 1]->freq;
	temp->left = psym[N - 1];
	temp->right = psym[N - 2];
	temp->code[0] = 0;
	temp->ch = 0;
	if (N == 2)
		return temp;
	struct SYM* slip1 = (struct SYM*)malloc(sizeof(struct SYM));
	struct SYM* slip2 = (struct SYM*)malloc(sizeof(struct SYM));
	for (int i = 0; i < N-2; i++)
	{
		if (psym[i]->freq <= temp->freq)
		{
			slip1 = psym[i];
			psym[i] = temp;
			int j = i + 1;
			while (psym[j])
			{
				slip2 = psym[j];
				psym[j] = slip1;
				slip1 = slip2;
				j++;
			}
			break;
		}
	}
	return buildTree(psym, N - 1);
}

void makeCodes(struct SYM* root)
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

unsigned char* unpack(unsigned char ch, unsigned char * buf)
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
	buf[8] = '\0';
	return buf;
}

char* chomp(char* str)
{
	if (str[strlen(str) - 2] == '\r')
		str[strlen(str) - 2] = '\0';
	return str;
}

int cmp(const void* a, const void* b)
{
	return int(((struct SYM*)b)->freq * 1000000) - int(((struct SYM*)a)->freq * 1000000);
}