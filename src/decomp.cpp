#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comp.h"

void decomp(char FileName_fp_archive[])
{
	struct SYM syms[256] = { 0 }; // массив структур
	struct SYM *psyms[256]; // массив указателей на структуры
	int i,
		diff_count = 0, // счетчик различных символов
		tail;
	FILE *fp_archive, // исходный файл (заархивированный)
		*fp_101, //закодированный файл
		*fp_in; //результирующий файл
	char FileName_fp_in[256] = { 0 },
		FileName_fp_101[256] = { 0 };

	fp_archive = OpenFile(FileName_fp_archive, "r");
	fp_101 = OpenFile(AddFileExtension(DelFileExtension(FileName_fp_archive, FileName_fp_101), FileName_fp_101, ".101"), "w");
	
	tail = ReadTitleFile(fp_archive, &diff_count, syms);
	printf("Wait...\n");
	
	ReadFile(fp_101, fp_archive, tail);

	fclose(fp_101);
	fclose(fp_archive);
	
	i = 0;
	while (syms[i].ch != '\0') // установка указателей на структуру
	{
		psyms[i] = &syms[i];
		i++;
	}

	struct SYM *tree = buildTree(psyms, diff_count); //построение дерева-реализация алгоритма Хаффмана 
	
	makeCodes(tree); // получение префиксного кода
	
	fp_101 = OpenFile(FileName_fp_101, "r");
	fp_in = OpenFile(DelFileExtension(FileName_fp_101, FileName_fp_in), "w");

	while (!feof(fp_101))
		fputc(SearchCode(fp_101, tree), fp_in);
	printf("Decompression completed!\n");
}

char *DelFileExtension(const char source[], char dest[])
{
	int k = strlen(source), i = 0;

	while (k >= i)
	{
		while (source[k] != '.')
			k--;
		break;
	}

	for (i = 0; i < k; i++)
		dest[i] = source[i];

	return dest;
}

int ReadTitleFile(FILE *fp_archive, int *diff_count, struct SYM *psym)
{
	char Sign[3]; // переменная для считанной сигнатуры
	char chtail, chdiff_count;
	int i, tail;

	fread(&Sign, sizeof(char), 3, fp_archive); //считали сигнатуру
	fread(&chdiff_count, sizeof(char), 1, fp_archive); // считали кол-во уникальных символов

	*diff_count = (int)chdiff_count;

	for (i = 0; i < *diff_count; i++) //считали таблицу: код символа-частота
	{
		fread(&psym[i].ch, sizeof(char), 1, fp_archive);
		fread(&psym[i].freq, sizeof(float), 1, fp_archive);
	}

	fread(&chtail, sizeof(char), 1, fp_archive);
	tail = (int)chtail;

	return tail;
}

void ReadFile(FILE *fp_101, FILE *fp_archive, int tail)
{
	char buf[8], //массив для хранения битов 
		prev_RealSymbol;
	int ReadSymbol, i;
	union CODE code;
	prev_RealSymbol = fgetc(fp_archive);

	while ((ReadSymbol = fgetc(fp_archive)) != EOF)
	{
		code.ch = prev_RealSymbol;
		fputc(code.byte.b1 + '0', fp_101);
		fputc(code.byte.b2 + '0', fp_101);
		fputc(code.byte.b3 + '0', fp_101);
		fputc(code.byte.b4 + '0', fp_101);
		fputc(code.byte.b5 + '0', fp_101);
		fputc(code.byte.b6 + '0', fp_101);
		fputc(code.byte.b7 + '0', fp_101);
		fputc(code.byte.b8 + '0', fp_101);
		prev_RealSymbol = ReadSymbol;
	}

	code.ch = prev_RealSymbol;
	buf[0] = code.byte.b1 + '0';
	buf[1] = code.byte.b2 + '0';
	buf[2] = code.byte.b3 + '0';
	buf[3] = code.byte.b4 + '0';
	buf[4] = code.byte.b5 + '0';
	buf[5] = code.byte.b6 + '0';
	buf[6] = code.byte.b7 + '0';
	buf[7] = code.byte.b8 + '0';

	for (i = 0; i < tail; i++)
		fputc(buf[i], fp_101);
}

unsigned char SearchCode(FILE *fp_101, struct SYM *root)
{
	int ch;

	ch = fgetc(fp_101);

	if (ch == '0' && root->left != NULL && root->right != NULL)
		return SearchCode(fp_101, root->left);
	if (ch == '1' && root->left != NULL && root->right != NULL)
		return SearchCode(fp_101, root->right);
	if (root->left == NULL && root->right == NULL)
		ungetc(ch, fp_101);

	return root->ch;
}