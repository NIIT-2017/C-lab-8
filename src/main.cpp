#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "task.h"
#define SIZE 256
#define rasmer1byte 8

/*программа принимает 2 аргумента. 1-й - это команда. 2-й - это имя файла. 
"com" используется для сжатия файла. "decom" используется для распаковки файла. Сжатие невозможно, если файл пуст. 
Цикл используется для сжатия / распаковки односимвольного файла. Дерево Хаффмана строится в другом случае.*/

int main(int argc, char* argv[])
{
	argv[1] = "decom";
	argv[2] = "in.key";
	if (strcmp(argv[1], "com") == 0)
	{
		//Анализатор
		FILE* fp = fopen(argv[2], "rb"); //исходный файл
		if (!fp)
		{
			perror("File:");
			exit(1);
		}
		/*printf("compression is started...\r");*/
		int point = findPoint(argv);
		//добавить расширение ".key\0", смотреть строку 64
		char* name = (char*)malloc((point) * sizeof(char));
		getName(argv, name, point);
		char extension[8] = { 0 };
		getExtension(argv, extension, point);
		struct SYM syms[SIZE] = { 0 };
		struct SYM* psyms[SIZE];
		int sum = read(fp, syms); // общее количество символов в файле
		fclose(fp);
		if (IfEmpty(sum))
			return 0;
		countFreq(sum, syms);
		qsort(syms, SIZE, sizeof(struct SYM), cmp);
		int count = fillinginpointers(syms, psyms); // количество уникальных символов
		struct SYM* root = buildTree(psyms, count);
		//ГЕНЕРАТОР КОДА
		makeCodes(root);
		//кодировщик
		FILE* fp_in = fopen(argv[2], "rb"); // исходный файл
		if (!fp_in)
		{
			perror("File:");
			exit(1);
		}
		/*делим общую длину 101 кода на размер байта.
         Затем вычитаем это число из размера байта.
         tail - количество добавленных нулей для заполнения последнего байта.
		*/
		int tail = rasmer1byte - ((coder(fp_in, syms, count)) % rasmer1byte);
		fclose(fp_in);
		//PACKER
		FILE* fp_compressed = fopen(strcat(name, ".key"), "wb"); // сжатый файл
		if (!fp_compressed)
		{
			perror("File:");
			exit(1);
		}
		writeHeader(fp_compressed, &count, syms, &tail, &sum, extension);
		packer(fp_compressed);
		_fcloseall();
		free(name);
		printf("The file is compressed!Everything is OK!");
	}
	else if (strcmp(argv[1], "decom") == 0)
	{
		FILE* fp_compressed = fopen(argv[2], "rb");
		if (!fp_compressed)
		{
			perror("File:");
			exit(1);
		}
		int point = findPoint(argv);
		char* name = (char*)malloc((point) * sizeof(char));
		newName(argv, name, point);
		char sign[3] = { 0 };
		int count = 0; //количество уникальных символов
		struct SYM syms[SIZE] = { 0 };
		int tail = 0;
		int sum = 0; // размер исходного файла
		char extension[8] = { 0 };
		//Читаем заголовок
		fread(sign, 1, 3, fp_compressed);
		//проверка сигнатуры
		if (sign[0] != 'k' || sign[1] != 'e' || sign[2] != 'y')
		{
			printf("invalid file!");
			return 0;
		}
		/*printf("decompression is started...\r");*/
		fread(&count, 4, 1, fp_compressed);
		if (count == 1)
		{
			void unkompressor(FILE * fp_compressed, char* name);
			return 0;
		}
		//чтение таблицы частот
		for (int i = 0; i < count; i++)
		{
			fread(&syms[i].ch, 1, 1, fp_compressed); // символ
			fread(&syms[i].freq, 4, 1, fp_compressed); // частота
		}
		fread(&tail, 1, 1, fp_compressed);
		fread(&sum, 4, 1, fp_compressed);
		for (int i = 0; i < 8; i++)
		{
			fread(&extension[i], 1, 1, fp_compressed);
		}
		struct SYM* psyms[SIZE] = { NULL };
		for (int i = 0; i < count; i++)
			psyms[i] = &syms[i];
		struct SYM* root = buildTree(psyms, count);
		makeDecodes(fp_compressed);
		fclose(fp_compressed);
		FILE* fp_decodes = fopen("101bydecom.txt", "rb"); // 101-файл
		if (!fp_decodes)
		{
			perror("File:");
			exit(1);
		}
		FILE* fp_decompressed = fopen(strcat(name, extension), "wb");
		if (!fp_decompressed)
		{
			perror("File:");
			exit(1);
		}
		//восстанавлиывем исходный файл
		char symbol = 0;
		for (int i = 0; i < sum; i++)
		{
			symbol = decoder(fp_decodes, root);
			fputc(symbol, fp_decompressed);
		}
		_fcloseall();
		free(name);
		printf("The file is decompressed! Well you were afraid))))");
	}
	else
		printf("You entered something wrong!Enter the command com or decom");
	return 0;
}