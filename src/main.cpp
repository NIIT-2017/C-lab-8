            /***************************
Данный код реализует алгоритм Хаффмана по сжатию файлов
   проверен на текстовых файлах
            ***************************/


#include "task.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
	if (strcmp(argv[2], "com") == 0) // сжатие
	{
		FILE* fp;
		FILE* out;
		FILE* filezip;
		fp = fopen(argv[1], "rb");
		out = fopen("fp_101.txt", "wt");// временный файл из 0 и 1
		{
			if (fp == NULL)
			{
				puts("File open erorr!\n");
				return 1;
			}
		}
		struct SYM syms[SIZE] = { 0 };
		struct SYM* psyms[SIZE] = { 0 };
		int ch;
		int count = 0, count1 = 0;
		while ((ch = fgetc(fp)) != EOF)//по символьно 
		{
			count++;
			for (int i = 0; i < SIZE; i++)
			{
				if (ch == syms[i].ch)
				{
					syms[i].freq++;
					break;
				}
				else if (syms[i].ch == 0)
				{
					syms[i].ch = ch;
					syms[i].freq++;
					count1++;
					break;
				}
			}
		}
		for (int i = 0; i < SIZE; i++)
			syms[i].freq = syms[i].freq / count;
		rewind(fp);
		qsort(syms, count1, sizeof(syms[0]), cmp);// сортировка
		for (int i = 0; i < SIZE; i++)
			psyms[i] = &syms[i];// заполняем массив указателей адресами структуры SYM
		struct SYM* root = buildTree(psyms, count1);
		makeCodes(root);
		ch = 0;
		while ((ch = fgetc(fp)) != EOF)
		{
			for (int i = 0; i < count1; i++)
				if (syms[i].ch == (unsigned char)ch)
				{
					fputs(syms[i].code, out); // выводим строку с кодом
					break; // прерываем поиск
				}
		}
		ch = 0;
		fclose(out);
		out = fopen("fp_101.txt", "rb"); 
		long int count2 = filesize(out);// получившйся count размер файла из 0 и 1
		count2 = count2%8; // остаток от деления на 8
		rewind(out);
		char bufname[256] = { 0 }; // массив для записи имя файла
		for (int i = 0; argv[1][i] != '.'; i++)
			bufname[i] = argv[1][i];
		char bufname2[5] = { ".tmv" }; // формат сжатого файла
		filezip = fopen(strcat(bufname, bufname2), "wb");
		fwrite("tmv", sizeof(char), 3, filezip); // записываю сигнатуру
		fwrite(&count1, sizeof(unsigned char), 1, filezip);// количество символов в таблице встречаемости
		for (int i = 0; i < count1; i++)
		{
			fwrite(&syms[i].ch, sizeof(unsigned char), 1, filezip);// записываю таблицу встречаемости - символ
			fwrite(&syms[i].freq, sizeof(float), 1, filezip);// записываю таблицу встречаемости - частота
		}
		fwrite(&count2, sizeof(unsigned char), 1, filezip);//длина хвоста - размер файла из 0 и 1 деленный на 8
		fwrite(&count, sizeof(int), 1, filezip);// размер иссходного сжимаего файла
		for (int i = 0; argv[1][i] != '\0'; i++) // записываем формат исходного файла
			if (argv[1][i] == '.')
			{
				for (int j = i; argv[1][j] != '\0'; j++)
					fwrite(&argv[1][j], sizeof(char), 1, filezip);
				break;
			}
		ch = 0;
		unsigned char buf[8] = { 0 };
		int count4 = 0;
		while ((ch = fgetc(out)) != EOF)
		{
			if (count4 == 8)
			{
				unsigned char a = pack(buf); // считываем по элементно и запаковывваем файл из 0 и 1
				fwrite(&a, sizeof(unsigned char), 1, filezip);
				for (int i = 0; i < 8; i++)
				{
					if (i == 0)
						buf[i] = ch;
					else
						buf[i] = '\0';
				}
				count4 = 1;
			}
			else 
			{
				buf[count4] = ch;
				count4++;
			}
		}
		if (count2 != 0) // если остаток от деления на 8 не равен 0, то дополняем хвостом из 0
		{
			for (int i = count2; i < 8; i++)
				buf[i] = '0';
			unsigned char a = pack(buf);
			fwrite(&a, sizeof(unsigned char), 1, filezip);
		}
		fcloseall();
		return 0;
	}
	else // декомпрессия
	{
		FILE* fp;
		FILE* out;
		FILE* decomp;
		fp = fopen(argv[1], "rb");
		char name[256] = { 0 }; // записываем имя файла
		for (int i = 0; argv[1][i] != '.'; i++)
			name[i] = argv[1][i];
		strcat(name, "1"); //добавляем еденицу вконце имени чтобы ибежать совпадения имен
		char buf[4] = { 0 };
		fread(buf, sizeof(char), 3, fp);// считываю формат
		if (strcmp(buf, "tmv") != 0)
		{
			puts("Unknown format!\nExit\n");
			exit(0);
		}
		int count = 0;
		fread(&count, sizeof(unsigned char), 1, fp); // считываю количесвто уникальных символов
		struct SYM syms[SIZE] = { 0 };// образую массив структур
		struct SYM* psyms[SIZE] = { 0 }; // образую массив указателей на структуры
		for (int i = 0; i < count; i++)
		{
			fread(&syms[i].ch, sizeof(unsigned char), 1, fp);// считываю таблицу встречаемости - символ
			fread(&syms[i].freq, sizeof(float), 1, fp);// считываю таблицу встречаемости - частота
			psyms[i] = &syms[i];
		}
		int count2 = 0;
		fread(&count2, sizeof(unsigned char), 1, fp); // хвост, остаток от деления на 8
		struct SYM* root = buildTree(psyms, count);
		makeCodes(root);
		int definesize = 0;
		fread(&definesize, sizeof(int), 1, fp); // считываем исходный размер файла;
		char buf2[5] = { 0 };
		fread(buf2, sizeof(char), 4, fp); // формат исходного файла
		unsigned char buf3[9] = { 0 };
		unsigned char ch = 0; // считываем по 2 байта чтобы определить последний и байт и обрезать его на остаток хвоста
		unsigned char ch1 = 0;
		out = fopen("upfile_101.txt", "wt"); // временный файл из 0 и 1
		while (fread(&ch, sizeof(unsigned char), 1, fp))
		{
			if (count2 != 0)
			{
				if (!fread(&ch1, sizeof(unsigned char), 1, fp)) // если байт последний то обрезаем его
				{
					unpack(buf3, ch);
					for(int i = 0;i<count2;i++)
						fprintf(out, "%c", buf3[i]);
					break;
				}
				fseek(fp, -sizeof(unsigned char), SEEK_CUR); // сдвигаем указатель в файле fp на 1 байт назад
			}
			unpack(buf3, ch); // распаковка файла
			fprintf(out, "%s", buf3); // содаем файл из 0 и 1
			for (int i = 0; i < 9; i++) // очищаем буфер
				buf3[i] = '\0';
		}
		fclose(out);
		out = fopen("upfile_101.txt", "rb");
		decomp = fopen(strcat(name, buf2), "wb");
		searchTree(root, out, decomp);// ищем в дереве нужное нам сочетание и записываем в файл соответсвующий символ
		fclose(decomp);
		decomp = fopen(name, "rb");
		long int resultsize = filesize(decomp);
		if (definesize!= resultsize)
		{
			puts("Error\n");
			exit(0);
		}
		fcloseall();
		return 0;
	}
}