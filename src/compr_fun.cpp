#include "compr.h"


static int index = 0;

void createStruct(PSYM syms, unsigned char ch, int count, int * uniqueCount)
{
	int in = 0;
	if (index == 0)
	{
		syms[index].ch = ch;
		syms[index].freq = syms[index].freq + ((float)1) / ((float)count);
		(*uniqueCount)++;
		index++;
		return;
	}

	for (int i = 0;i < index;i++)
	{
		if (syms[i].ch == ch)
			in++;
	}

	for (int i = 0;i < index;i++)
	{
		if (syms[i].ch != ch && in != 1)
		{
			syms[index].ch = ch;
			syms[index].freq = syms[index].freq + ((float)1) / ((float)count);
			index++;
			(*uniqueCount)++;
			break;
		}
		else if (syms[i].ch == ch && in == 1)
		{
			syms[i].freq = syms[i].freq + ((float)1) / ((float)count);
		}
	}
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

void sortStruct(PSYM syms, int size)
{
	qsort(syms, size, sizeof(SYM), compar);
}


void printStruct(PSYM syms, int uniqueCount)
{
	for (int i = 0;i < uniqueCount;i++)
	{
		printf("%c\t%f\t%s\n", syms[i].ch, syms[i].freq, syms[i].code);
	}

}

PSYM buildTree(PSYM psym[], int size)
{
	// создаем временный узел
	PSYM temp = (PSYM)malloc(sizeof(SYM));
	// в поле частоты записывается сумма частот последнего и предпоследнего элементов psym
	temp->freq = psym[size - 2]->freq + psym[size - 1]->freq;
	// связываем созданный узел с двумя последними узлами
	temp->left = psym[size - 1];
	temp->right = psym[size - 2];
	temp->code[0] = '\0';
	if (size == 2) // мы сформировали корневой элемент с частотой 1.0
		return temp;

	for (int i = size - 3;i >= 0;i--)
	{
		if (psym[i]->freq <= temp->freq)
		{
			psym[i + 1] = psym[i];
			psym[i] = temp;
		}
	}
	// добавляем temp в нужную позицию psym, сохраняя порядок убывания частоты
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

void printStructFriq(PSYM syms, int uniqueCount)
{
	float maxFreq = 0.0;

	printf("Symbol\tfrequency\n");

	for (int i = 0;i < uniqueCount;i++)
	{
		printf("%c\t%f\n", syms[i].ch, syms[i].freq);
		maxFreq = maxFreq + syms[i].freq;
	}

	printf("Total frequency %f\n", maxFreq);
}
