#include "task1.h"

static int index = 0;

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

void ListCreator(PSYM buf, unsigned char ch, ULL ch_count, UI *ch_uniq)
{
	int in = 0;
	if (index == 0)
	{
		buf[index].ch = ch;
		buf[index].freq = buf[index].freq + ((float)1) / ((float)ch_count);
		(*ch_uniq)++;
		index++;
		return;
	}

	for (int i = 0; i < index; i++)
	{
		if (buf[i].ch == ch)
			in++;
	}

	for (int i = 0; i < index; i++)
	{
		if (buf[i].ch != ch && in != 1)
		{
			buf[index].ch = ch;
			buf[index].freq = buf[index].freq + ((float)1) / ((float)ch_count);
			index++;
			(*ch_uniq)++;
			break;
		}
		else if (buf[i].ch == ch && in == 1)
		{
			buf[i].freq = buf[i].freq + ((float)1) / ((float)ch_count);
		}
	}
}

static int compare(const void* p1, const void* p2)
{
	if (((PSYM)p2)->freq > ((PSYM)p1)->freq)
		return 1;
	if (((PSYM)p2)->freq < ((PSYM)p1)->freq)
		return -1;
	if (((PSYM)p2)->freq == ((PSYM)p1)->freq)
		return 0;
}
void sortList(PSYM buf, int size)
{
	qsort(buf, size, sizeof(SYM), compare);
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