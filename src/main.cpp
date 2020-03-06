#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#define SIZE 256

int main(int argc, char* argv[])
{
	FILE* fp_in;                       // указатель на исходный файл
	FILE* fp_101;                      // указатель на файл с бинарным кодом (временный с нулями и единицами)
	FILE* fp_vvk;                      // указатель на сжатый файл

	char resultName[SIZE] = { 0 };       // массив для записи имени сжатого файла с новым расширением
	strcpy(resultName, argv[1]);

	fp_101 = fopen("temp.txt", "wb");                   // открываем файл для записи бинарного кода
	fp_vvk = fopen(strcat(resultName, ".vvk"), "wb");   // открываем файл для записи сжатой информации и присваиваем ему новое расширение

	int tempCh;                        // в эту переменную читается информация из файла
	int count = 0;                     // счётчик количества уникальных символов в исходнике
	int globalCount = 0;               // счётчик количества всех символов в исходнике
	int count101 = 0;                  // счётчик количества символов из 0 и 1 в промежуточном .101 файле
	int quantity[SIZE] = { 0 };        // инициализируем массив количества уникальных символов
	struct SYM symbols[SIZE];          // инициализируем массив структур типа SYM
	struct SYM* psym[SIZE];            // инициализируем массив указателей на структуры типа SYM
	float sumFreq = 0;                 // сумма частот встречаемости

	if ((fp_in = fopen(argv[1], "rb")) == NULL)               // исключение ошибки чтения/нахождения файла
	{
		puts("File not found!"); 
		return 1;
	}

	for (int i = 0; i < SIZE; i++)
	{
		symbols[i].ch = '\0';
	}

	while ((tempCh = fgetc(fp_in)) != EOF)                       // побайтно читаем исходный файл и составляем таблицу встречаемости
	{
		for (int i = 0; i < SIZE; i++)
		{
			if (tempCh == symbols[i].ch)
			{
				quantity[i]++;
				globalCount++;
				break;
			}
			if (symbols[i].ch == '\0')
			{
				symbols[i].ch = (unsigned char)tempCh;
				quantity[i] = 1;
				count++; globalCount++;
				break;
			}
		}
	}

	for (int i = 0; i < count; i++)                             // рассчёт частоты встречаемости
		symbols[i].freq = (float)quantity[i] / globalCount;

	for (int i = 0; i < count; i++)                             // в массив указателей заносим адреса структур
	{
		psym[i] = &symbols[i];
		psym[i]->left = NULL;
		psym[i]->right = NULL;
	}

	struct SYM temp;                                           // сортировка по убыванию, Метод «пузырька»
	for (int i = 0; i < count; i++)
		for (int j = 0; j < count - i - 1; j++)
			if (symbols[j].freq < symbols[j + 1].freq)
			{
				temp = symbols[j];
				symbols[j] = symbols[j + 1];
				symbols[j + 1] = temp;
			}

	for (int i = 0; i < count; i++)
	{
		sumFreq += symbols[i].freq;
		printf("Ch= %d\tFreq= %f\tPPP= %c\t\n", symbols[i].ch, symbols[i].freq, psym[i]->ch);
	}
	printf("\nSlova = %d\tsumFreq=%f\n", globalCount, sumFreq);

	struct SYM* root = buildTree(psym, count);      // вызов функции создания дерева Хаффмана
	makeCodes(root);                                // вызов функции получения кода
	rewind(fp_in);                                  // возвращаем указатель в файле в начало файла

	while ((tempCh = fgetc(fp_in)) != EOF)          // читаем исходник, и записываем полученные через makeCodes коды в .101 файл
	{                                               // каждый прочитанный символ просматривается в массиве структур SYM в поле ch
		for (int i = 0; i < count; i++)
			if (symbols[i].ch == tempCh)             // когда совпадение произошло, необходимо вывести в .101 файл содержимое поля code
			{
				fputs(symbols[i].code, fp_101);     // выводим строку с кодом
				break;                              // прерываем поиск
			}
	} 
	fclose(fp_101); //сначала записал, потом открыть,чтобы обработать

	int i = 0;
	fp_101 = fopen("temp.txt", "rb");               // вновь открываем промежуточный .101 файл с бинарным кодом (для чтения)

	while ((tempCh = fgetc(fp_101)) != EOF)          // считаем количество единиц инулей в .101 файле (определяем его размер)
		count101++;

	int tail = count101 % 8;                        // находим остаток, размер хвоста файла (количество символов не кратных 8 в .101 файле)
	
	//формируем заголовок сжатого файла через поля байтов
	fwrite("vvk", 3, 1, fp_vvk);					// сигнатура (подпись формата)
	fwrite(&count, 3, 1, fp_vvk);                   // количество уникальных символов (число строк в таблице встречаемости)
	for (i = 0; i < count; i++)                      // сама таблица встречаемости (пары: код символа - частота)
	{
		fwrite(&symbols[i].ch, 3, 1, fp_vvk);
		fwrite(&symbols[i].freq, 4, 1, fp_vvk);
	}
	fwrite(&tail, 1, 1, fp_vvk);                    // длина хвоста (остаток деления размера .101 файла на 8)
		/* конец заголовка */

	rewind(fp_101);                                 // возвращаем указатель в промежуточном файле в начало файла
	unsigned char buf[8] = { 0 };                     // инициализируем массив единиц и нулей для union
	union CODE code;                                // инициализируем переменную code для ф-и pack
	char j = 0;

	for (int i = 0; i < count101 - tail; i++)        // читаем .101 файл, занося последовательно каждые 8 элементов в массив для побитовой обработки в union
	{
		buf[j] = fgetc(fp_101);
		if (j == 7)
		{
			fputc(pack(buf), fp_vvk);  
			j = 0;
		}
		j++;
	}

	j = 0;
	for (int i = 0; i <= tail; i++)                  // записываем хвост
	{
		buf[j] = fgetc(fp_101);
		if (j == tail)
		{
			//pack(buf);
			fputc(pack(buf), fp_vvk);
		}
		j++;
	}

	_fcloseall();  //закрываем все открытые файлы
	return 0;
}