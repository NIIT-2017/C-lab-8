#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comp.h"

void comp(char FileName_fp_in[])
{
	struct SYM syms[256] = { 0 }; // массив структур
	struct SYM *psyms[256]; // массив указателей на структуры
	int i,
		diff_count = 0, // счетчик различных символов
		sizeControlFile = 0, //кол-во символов в файле 010
		tail; //длина хвоста
	FILE *fp_in, // исходный файл
		*fp_101, //закодированный файл
		*fp_archive; //результирующий файл
	char FileName_fp_101[256] = { 0 },
		FileName_fp_archive[256] = { 0 };

	fp_in = OpenFile(FileName_fp_in, "rb");
	fp_101 = OpenFile(AddFileExtension(FileName_fp_in, FileName_fp_101, ".101"), "w");
	
	diff_count = MakeTable(fp_in, syms); //чтение исходного файла и создание таблицы встречаемости
	printf("Wait...\n");

	i = 0;
	while (syms[i].ch != '\0') // установка указателей на структуру
	{
		psyms[i] = &syms[i];
		i++;
	}
	
	struct SYM *tree = buildTree(psyms, diff_count); //построение дерева-реализация алгоритма Хаффмана 
	makeCodes(tree); // получение префиксного кода
	
	rewind(fp_in); //поставили указатель на начало исходного файла
	Coder(fp_in, fp_101, syms, diff_count);
	fclose(fp_in);
	fclose(fp_101);
	
	fp_101 = OpenFile(FileName_fp_101, "rb");
	tail = SearchTail(fp_101, &sizeControlFile);
	
	fp_archive = OpenFile(AddFileExtension(FileName_fp_in, FileName_fp_archive, ".comp"), "w");
	CreateTitleFile(fp_archive, diff_count, syms, tail);
	
	rewind(fp_101); //поставили указатель на начало файла 101
	CreateFile(fp_101, fp_archive, sizeControlFile, tail);
	fclose(fp_101);
	fclose(fp_archive);
	printf("Compression completed!\n");
}

FILE* OpenFile(char name_file[], const char mode[])
{
	FILE* fp = fopen(name_file, mode);
	if (fp == NULL)
	{
		printf("Error: ");
		return 0;
	}
	return fp;
}

char* AddFileExtension(const char source[], char dest[], const char extension[])
{
	strcpy(dest, source);
	strcat(dest, extension);

	return dest;
}

int MakeTable(FILE *fp_in, struct SYM *psyms) //чтение исходного файла и создание таблицы встречаемости
{
	int i, j,
		ReadSymbol, // текущий символ (читаемый из файла)
		count = 0, // счетчик символов всего (в исходном файле)
		diff_count = 0;
	struct SYM temp;

	while ((ReadSymbol = fgetc(fp_in)) != EOF) //чтение исходного файла и создание таблицы встречаемости 
	{
		count++;
		for (i = 0; i <= count; i++)
		{
			if (ReadSymbol == psyms[i].ch)
			{
				psyms[i].freq++;
				break;
			}
			if (psyms[i].ch == '\0')
			{
				psyms[i].ch = ReadSymbol;
				psyms[i].freq++;
				diff_count++;
				break;
			}
		}
	}

	for (i = 0; i < diff_count; i++) // подсчет частоты встречаемости
		psyms[i].freq = (float)psyms[i].freq / count;

	for (i = 0; i < diff_count - 1; i++) // сортировка по частоте встречаемости
		for (j = diff_count - 1; j > i; j--)
			if (psyms[j - 1].freq < psyms[j].freq)
			{
				temp = psyms[j - 1];
				psyms[j - 1] = psyms[j];
				psyms[j] = temp;
			}

	return diff_count;
}

struct SYM * buildTree(struct SYM *psym[], int N)
{
	struct SYM *temp = (struct SYM*)malloc(sizeof(struct SYM)); // создаём временный узел
	temp->freq = psym[N - 2]->freq + psym[N - 1]->freq; /* в поле частоты записывается сумма частот
														последнего и предпоследнего элементов массива psym*/
	temp->left = psym[N - 1]; // связываем созданный узел с двумя последними узлами
	temp->right = psym[N - 2];
	temp->code[0] = 0;

	if (N == 2) // мы сформировали корневой элемент с частотой 1.0
		return temp;
	else //внесение в массив в нужное место элемента дерева Хофмана
	{
		for (int i = 0; i < N; i++)
			if (temp->freq > psym[i]->freq)
			{
				for (int j = N - 1; j > i; j--)
					psym[j] = psym[j - 1];
				psym[i] = temp;
				break;
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

void Coder(FILE *fp_in, FILE *fp_101, struct SYM *psym, int diff_count)
{
	int i, ReadSymbol;

	while ((ReadSymbol = fgetc(fp_in)) != -1) //чтение исходного файла и создание файла с кодами
	{
		for (i = 0; i < diff_count; i++)
			if (ReadSymbol == psym[i].ch)
			{
				fputs(psym[i].code, fp_101); // выводим строку с кодом в файл
				break; // прерываем поиск
			}
	}
}

int SearchTail(FILE *fp_101, int *sizeControlFile)
{
	int	ReadSymbol, tail;

	while ((ReadSymbol = fgetc(fp_101)) != EOF)
		(*sizeControlFile)++;
	tail = *sizeControlFile % 8; //длина хвоста

	return tail;
}

void CreateTitleFile(FILE *fp_archive, int diff_count, struct SYM *psym, int tail)
{
	int i;

	fwrite("liz", sizeof(char), 3, fp_archive); // записываем 1. Сигнатуру
	fwrite(&diff_count, sizeof(char), 1, fp_archive); //записываем 2. Кол-во различных символов

	for (i = 0; i < diff_count; i++) //записываем 3. код символа-частота
	{
		fwrite(&psym[i].ch, sizeof(char), 1, fp_archive);
		fwrite(&psym[i].freq, sizeof(float), 1, fp_archive);
	}

	fwrite(&tail, sizeof(char), 1, fp_archive); //записываем 4. Длину хвоста
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

void CreateFile(FILE *fp_101, FILE * fp_archive, int sizeControlFile, int tail)
{
	unsigned char buf[8]; //массив для хранения битов 
	int i, j;

	for (i = 0; i < sizeControlFile / 8; i++) //кодируем основную часть
	{
		j = 0;
		while (j < 8)
		{
			buf[j] = getc(fp_101);
			j++;
		}
		fputc(pack(buf), fp_archive);
	}

	if (tail != 0) //если есть хвост, то кодируем его
	{
		for (i = 0; i < 8; i++)
			buf[i] = 0;
		j = 0;

		while (j < tail)
		{
			buf[j] = getc(fp_101);
			j++;
		}
		fputc(pack(buf), fp_archive);
	}
}