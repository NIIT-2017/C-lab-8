#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 256


struct SYM* buildTree(struct SYM* psym[], int N) // построения дерева
{
	// создаём временный узел
	struct SYM* temp = (struct SYM*)malloc(sizeof(struct SYM));
	// в поле частоты записывается сумма частот
	// последнего и предпоследнего элементов массива psym
	temp->freq = psym[N - 2]->freq + psym[N - 1]->freq;
	// связываем созданный узел с двумя последними узлами
	temp->left = psym[N - 1];
	temp->right = psym[N - 2];
	temp->code[0] = 0;
	if (N == 2) // мы сформировали корневой элемент с частотой 1.0
		return temp;
	for (int i = N-2; i > 0; i--)
	{
		if (temp->freq >= psym[0]->freq)
		{
			for (int j = N - 2; j >= 0; j--)
				psym[j + 1] = psym[j];
			psym[0] = temp;
			break;
		}
		else if (temp->freq >= psym[i]->freq && temp->freq <= psym[i - 1]->freq)
		{
			for (int j = N - 2; j >= i; j--)
				psym[j + 1] = psym[j];
			psym[i]= temp;
			break;
		}
	}
	return buildTree(psym, N - 1);
}


unsigned char pack(unsigned char buf[]) // функция упаковки
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

void unpack(unsigned char* buf,unsigned char ch) // функция распаковки
{
	union CODE code;
	code.ch = ch;
	buf[0] = code.byte.b1+'0';
	buf[1] = code.byte.b2+'0';
	buf[2] = code.byte.b3+'0';
	buf[3] = code.byte.b4+'0';
	buf[4] = code.byte.b5+'0';
	buf[5] = code.byte.b6+'0';
	buf[6] = code.byte.b7+'0';
	buf[7] = code.byte.b8+'0';
}

void searchTree(struct SYM* root, FILE* in,FILE*out) // поиск нужного слова в дереве
{
	static SYM* rootD = root;
	if (root->right == NULL && root->left == NULL)
	{
		fprintf(out, "%hhc", root->ch);
		return;
	}
	int ch = 0;
	while ((ch = fgetc(in)) != EOF)
	{
		if (ch == '0')
		{
			searchTree(root->right, in, out);
			if (root != rootD)
				return;
			else
				continue;
		}
		if (ch == '1')
		{
			searchTree(root->left, in, out);
			if (root != rootD)
				return;
			else
				continue;
		}
	}
}

void makeCodes(struct SYM* root) // создает код из 0 и 1
{
	if (root->left)
	{
		strcpy(root->left->code, root->code);
		strcat(root->left->code, "1");
		makeCodes(root->left);
	}
	if (root->right)
	{
		strcpy(root->right->code, root->code);
		strcat(root->right->code, "0");
		makeCodes(root->right);
	}
}


int cmp(const void* a, const void* b) // функция компаратора
{
	return int(((struct SYM*)b)->freq * 100) - int(((struct SYM*)a)->freq * 100);
}

void print(struct SYM* syms)
{
	for (int i = 0; i < SIZE; i++)
	{
		if (syms[i].ch == '\n')
			printf("\\n - %f\n", syms[i].freq);
		else if (syms[i].ch == '\r')
			printf("\\r - %f\n", syms[i].freq);
		else if (syms[i].ch != 0 && syms[i].ch != '\n')
			printf("%c - %f\n", syms[i].ch, syms[i].freq);
	}
}

long int filesize(FILE* fp) // функция определяет размер файла
{
	long int save_pos, size_of_file;
	save_pos = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	size_of_file = ftell(fp);
	fseek(fp, save_pos, SEEK_SET);
	return(size_of_file);
}