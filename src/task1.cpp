#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "task1.h"
extern int tail;

int sizeFileIn(const char * fileName)
{
	FILE *fp;
	fp = fopen(fileName, "rb");
	while (fgetc(fp) != -1)
		;
	return ftell(fp);
	fclose(fp);
}

int makebuf(unsigned char *symbol, float *number, const char * fileName)
{
	int n = 0;//временная переменная для хранения считанного символа
	int arr[256] = { 0 };//временный массив, куда записывается встречаемость символа в абсолютном выражении
	int x = 0, N = 0;//N - счетчик количества встреченных символов, он же "список свободных узлов"

	FILE *fp;//вывод текста из файла
	fp = fopen(fileName, "rb");
	if (fp == NULL)
		puts("File not found");
	while ((n = fgetc(fp)) != -1)//можно написать != EOF
	{
		for (x = 0; x <= N; )
		{
			if (x == N)
			{
				symbol[N] = (unsigned char) n;
				arr[N]++;
				N++;
				break;
			}
			else if ((unsigned char)n == symbol[x])
			{
				arr[x]++;
				break;
			}
			x++;
		}
	}

	float y = 0;//перевод абсолютного значения встречаемости символов в относительный
	for (x = 0; x < N; x++)
		y = y + arr[x];
	for (x = 0; x < N; x++)
		number[x] = arr[x] / y;

	float *pnum = &number[x];//сортировка обоих массивов
	unsigned char *psym = &symbol[x];
	float maxnum = 0;
	unsigned char maxsym = '0';
	for (x = 0; x < N; x++)
	{
		for (pnum = &number[x], psym = &symbol[x], maxnum = *pnum, maxsym = *psym; *pnum != number[N]; pnum++, psym++)
		{
			if (*pnum >= maxnum)
			{
				number[x] = *pnum, *pnum = maxnum, maxnum = number[x];
				symbol[x] = *psym, *psym = maxsym, maxsym = symbol[x];
			}
		}
	}
	fclose(fp);
	return N;
}

void makestruct(int N, struct SYM syms[], struct SYM *psym[], unsigned char *symbol, float *number)//заполнение массива структур
{
	int x = 0;
	while (x < N)
	{
		syms[x].ch = symbol[x];
		syms[x].freq = number[x];
		syms[x].code[0] = '\0';
		syms[x].left = 0;
		syms[x].right = 0;
		x++;
	}

	x = 0;
	while (x < N)
	{
		psym[x] = &syms[x];
		x++;
	}

}

struct SYM *buildTree(struct SYM *psym[], int N)
{
	int i, j;
	struct SYM *temp = (struct SYM*)malloc(sizeof(struct SYM));
	temp->freq = psym[N - 1]->freq + psym[N - 2]->freq;
	temp->left = psym[N - 1];
	temp->right = psym[N - 2];
	temp->code[0] = 0;
	if (N == 2)//конец рекурсии
		return temp;
	else//вставка temp в список свободных узлов (например, теперь станет psys(N-1)=syms(N-2), а psys (N-2)=temp)
	{
		for (i = 0; i < N; i++)
			if (temp->freq > psym[i]->freq)
			{
				for (j = N - 1; j > i; j--)
					psym[j] = psym[j - 1];
				psym[i] = temp;
				break;
			}
	}
	return buildTree(psym, N - 1);//вызов рекурсии с обрезанием списка свободных узлов на 1
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

int CodeBlock(int N, int tail, struct SYM syms[], const char * fileName)//заполнение файла 101, дописывание в него хвостика
{
	unsigned int l_101 = 0;//длина файла 101 до деления на 8
	int i = 0;
	int ch;//int, чтобы корректно проверялось, что возвращает fgetc

	FILE *fp;
	fp = fopen(fileName, "rb");
	FILE *fp_101;
	fp_101 = fopen("101.txt", "wb");

	while ((ch = fgetc(fp)) != -1)
	{
		for (i = 0; i < N; i++)
			if (syms[i].ch == (unsigned char)ch)
			{
				fputs(syms[i].code, fp_101); // выводим строку с кодом
				break; // прерываем поиск
			}
	}
	fclose(fp);
	fclose(fp_101);

	fp_101 = fopen("101.txt", "rb"); //дописываем хвостик в файл 101
	while (fgetc(fp_101) != -1)
		l_101++;
	tail = l_101 % 8;
	fclose(fp_101);
	if (tail != 0)
	{
		fp_101 = fopen("101.txt", "ab");
		for (i = 0; i < 8 - tail; i++)
			fputs("0", fp_101);
		fclose(fp_101);
	}
	return tail;
}

void pack(int N, int tail, struct SYM syms[256], const char * fileName, unsigned int size_in)
{
	char arr[256];//создание имени сжатого файла
	strcpy(arr, fileName);
	strcat(arr, ".krv");
	FILE *fcode;
	fcode = fopen(arr, "wb");//

	int buf[8];
	int i = 0;
	FILE *fp_101;
	fp_101 = fopen("101.txt", "rb");

	fwrite("krv", 3, 1, fcode);//запись сигнатуры в 101
	fwrite(&N, 4, 1, fcode);//запись количества уникальных символов в 101
	fwrite(&tail, 4, 1, fcode);//запись хвоста в 101
	fwrite(&size_in, 4, 1, fcode);//запись размера исходного файла в 101
	while (i < N)//запись таблицы символов в 101
	{
		fwrite(&syms[i].ch, 1, 1, fcode);
		fwrite(&syms[i].freq, 4, 1, fcode);
		i++;
	}
	
	i = 0;
	union CODE code;
	while ((buf[i] = fgetc(fp_101)) != -1)
	{
		if (i == 7)
		{
			code.byte.b1 = buf[0] - '0';
			code.byte.b2 = buf[1] - '0';
			code.byte.b3 = buf[2] - '0';
			code.byte.b4 = buf[3] - '0';
			code.byte.b5 = buf[4] - '0';
			code.byte.b6 = buf[5] - '0';
			code.byte.b7 = buf[6] - '0';
			code.byte.b8 = buf[7] - '0';
			fputc(code.ch, fcode);
			i = 0;
		}
		else i++;
	}
	fclose(fcode);
	fclose(fp_101);
	remove("101.txt");

}

int repack(int N, unsigned char *symbol, float *number, const char * fileName, unsigned int *psize_in)//чтение сжатого файла
{
	int i = 0;
	union CODE code1;

	FILE *fp_101;
	fp_101 = fopen("101.txt", "wb");
	FILE *fcode;
	fcode = fopen(fileName, "rb");

	char signatur[4];//проверка сигнатуры
	signatur[3] = '\0';
	fread(signatur, 3, 1, fcode);
	if (strcmp(signatur, "krv") != 0)
		puts("File not found");

	fread(&N, 4, 1, fcode);
	fread(&tail, 4, 1, fcode);
	fread(psize_in, 4, 1, fcode);
	while (i < N)
	{
		fread(&symbol[i], 1, 1, fcode);
		fread(&number[i], 4, 1, fcode);
		i++;
	}

	i = 0;
	while ((i = fgetc(fcode)) != -1)
	{
		code1.ch = (unsigned char)i;
		fputc(code1.byte.b1 + '0', fp_101);
		fputc(code1.byte.b2 + '0', fp_101);
		fputc(code1.byte.b3 + '0', fp_101);
		fputc(code1.byte.b4 + '0', fp_101);
		fputc(code1.byte.b5 + '0', fp_101);
		fputc(code1.byte.b6 + '0', fp_101);
		fputc(code1.byte.b7 + '0', fp_101);
		fputc(code1.byte.b8 + '0', fp_101);
	}
		
	fclose(fcode);
	fclose(fp_101);
	return N;
}

void finish(struct SYM *root, const char * fileName, unsigned int size_in)//поиск по обратному дереву и запись его в итоговый файл
{
	int i = strlen (fileName);//создание имени итогового файла
	char arr[256];
	strcpy(arr, fileName);
	arr[i - 4] = '\0';
	FILE *fp_out;
	fp_out = fopen(arr, "wb");
	if (fp_out == NULL)
		puts("File not found");
	
	FILE *fp_101;
	fp_101 = fopen("101.txt", "rb");
	
	int x = 0;
	while (fgetc(fp_101) != -1)//счетчик длины файла 101, нужен для вычитания из него хвостика
		x++;
	rewind(fp_101);

	int y = 0;
	i = 0;
	struct SYM *tree = root;
	while (y <= (x - (8 - tail)))
	{
		if ((tree->left == 0) && (tree->right == 0))
		{
			fputc(tree->ch, fp_out);
			tree = root;
		}
		else
		{
			i = fgetc(fp_101);
			y++;
			if (i == '0')
				tree = tree->left;
			else
				tree = tree->right;
		}
	}
	fclose(fp_101);
	remove("101.txt");
	if (ftell(fp_out) != size_in)//проверка исходного и раскодированного файлов по размеру
		puts("Fail uncorrect");
	fclose(fp_out);
}



