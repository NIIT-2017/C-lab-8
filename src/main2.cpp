#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "task2.h"



int main()
{
	FILE *fp_in = fopen("in.txt", "rb"); // открываем наш файл для сжатия
	FILE *fp_temp = fopen("temp.txt", "wb"); // открываем файл для записи бинарного кода
	FILE *fp_010 = fopen("010.txt", "wb"); // открываем файл для записи сжатого файла

	int info;  // в эту переменную читается информация из файла
	int different_letters = 0; //счётчик количества различных букв, уникальных символов
	int all_sign = 0; // счётчик количества всех знаков в файле
	int fsize = 0;//счётчик количества символов из 0 и 1 в промежуточном файле temp
	int tail_size;//размер хвоста файла (то, что не кратно 8 в промежуточном файле)
	int numb_unique_symb[256] = { 0 };//инициализируем массив количества уникальных символов
	struct SYM symb[256] = { 0 }; //инициализируем массив записей 
	struct SYM *psym[256]; //инициализируем массив указателей на записи
	float sum = 0;//сумма частот встречаемости
	int arr[8];//массив 0 и 1
	char j = 0;//вспомогательная переменная

	//Обработка ошибок чтения файла
	if (fp_in == NULL)
	{
		puts("Error! File not open");
		return 0;
	}

	struct SYM *symbols = (struct SYM*)malloc(different_letters * sizeof(struct SYM));//создание динамического массива структур symb
	struct SYM **psum = (struct SYM**)malloc(different_letters * sizeof(struct SYM*));//создание динамического массива указателей на symb

	//Начинаем побайтно читать файл и составлять таблицу встречаемости
	while ((info = fgetc(fp_in)) != EOF)
	{
		for (int j = 0; j < 256; j++)
		{
			if (info == symb[j].ASCII)
			{
				numb_unique_symb[j]++;
				all_sign++;
				break;
			}
			if (symb[j].ASCII == 0)
			{
				symb[j].ASCII = (unsigned char)info;
				numb_unique_symb[j] = 1;
				different_letters++; all_sign++;
				break;
			}
		}
	}

	// Рассчёт частоты встречаемости
	for (int i = 0; i < different_letters; i++)
		symb[i].freq = (float)numb_unique_symb[i] / all_sign;

	for (int i = 0; i < different_letters; i++) //в массив указателей заносим адреса записей
		psym[i] = &symb[i];

	//Сортировка по убыванию 
	struct SYM temp;
	for (int i = 1; i < different_letters; i++)
		for (int j = 0; j < different_letters - 1; j++)
			if (symb[j].freq < symb[j + 1].freq)
			{
				temp = symb[j];
				symb[j] = symb[j + 1];
				symb[j + 1] = temp;
			}

	for (int i = 0; i < different_letters; i++)
	{
		sum += symb[i].freq;
		printf("ASCII = %.3d\tfrequence = %f\tsymbol = %c\t\n", symb[i].ASCII, symb[i].freq, psym[i]->ASCII);
	}
	printf("\nwords = %d\tsum = %f\n", all_sign, sum);

	struct SYM *root = buildTree(psym, different_letters);//вызов функции создания дерева Хофмана

	makeCodes(root);//вызов функции получения кода

	rewind(fp_in);//возвращаем указатель в файле в начало файла

	//в цикле читаем исходный файл, и записываем полученные в функциях коды в промежуточный файл
	while ((info = fgetc(fp_in)) != EOF)
	{
		for (int i = 0; i < different_letters; i++)
			if (info == symb[i].ASCII)
				fputs(symb[i].code, fp_temp);
	}
	fclose(fp_temp);

	//Заново открываем файл с бинарным кодом, но теперь для чтения
	int i = 0;
	fp_temp = fopen("temp.txt", "rb");
	//Считаем размер бинарного файла(количество символов в нём)
	while ((info = fgetc(fp_temp)) != EOF)
		fsize++;

	tail_size = fsize % 8;//находим остаток, количество символов не кратных 8 (хвост)

	//формируем заголовок сжатого файла через поля байтов
	fwrite("compressing", sizeof(char), 24, fp_010);//условная подпись
	fwrite(&different_letters, sizeof(int), 1, fp_010);//количество уникальных символов
	fwrite(&tail_size, sizeof(int), 1, fp_010);//величина хвоста
	//Записываем в сжатый файл таблицу встречаемости
	for (i = 0; i < different_letters; i++)
	{
		fwrite(&symb[i].ASCII, sizeof(struct SYM), 1, fp_010);
		fwrite(&symb[i].freq, sizeof(struct SYM), 1, fp_010);
	}

	rewind(fp_temp);//возвращаем указатель в промежуточном файле в начало файла

	union CODE code;//инициализируем переменную code1
	//Читаем бинарный файл, занося последовательно каждые 8 элементов в массив для последующей побитовой обработки в объединении union
	j = 0;
	for (int i = 0; i < fsize - tail_size; i++)
	{
		arr[j] = fgetc(fp_temp);
		if (j == 7)
		{
			code.byte.b1 = arr[0] - '0';
			code.byte.b2 = arr[1] - '0';
			code.byte.b3 = arr[2] - '0';
			code.byte.b4 = arr[3] - '0';
			code.byte.b5 = arr[4] - '0';
			code.byte.b6 = arr[5] - '0';
			code.byte.b7 = arr[6] - '0';
			code.byte.b8 = arr[7] - '0';
			fputc(code.code, fp_010);
			j = 0;
		}
		j++;
	}
	//Записываем хвост
	j = 0;
	for (int i = 0; i <= tail_size; i++)
	{
		arr[j] = fgetc(fp_temp);
		if (j == tail_size)
		{
			code.byte.b1 = arr[0] - '0';
			code.byte.b2 = arr[1] - '0';
			code.byte.b3 = arr[2] - '0';
			code.byte.b4 = arr[3] - '0';
			code.byte.b5 = arr[4] - '0';
			code.byte.b6 = arr[5] - '0';
			code.byte.b7 = arr[6] - '0';
			code.byte.b8 = arr[7] - '0';
			fputc(code.code, fp_010);
		}
		j++;
	}

//	fcloseall(); // закрываем все открытые файлы

	return 0;
}