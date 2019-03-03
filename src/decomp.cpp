#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "comp.h"
#include <string.h>

void unPack(unsigned char buf[], unsigned char ch)
{
	union CODE code;
	code.ch = ch;
	buf[0] = code.byte.b1 + '0';
	buf[1] = code.byte.b2 + '0';
	buf[2] = code.byte.b3 + '0';
	buf[3] = code.byte.b4 + '0';
	buf[4] = code.byte.b5 + '0';
	buf[5] = code.byte.b6 + '0';
	buf[6] = code.byte.b7 + '0';
	buf[7] = code.byte.b8 + '0';
	buf[8] = 0;

	//return buf;
}

void getASCII(unsigned int ch, char *code)
{
	unsigned int n = ch;
	_itoa(n, code, 2);
	char buf;
	for (int i = 0; i < strlen(code) / 2; i++)
	{
		buf = code[i];
		code[i] = code[strlen(code) - 1 - i];
		code[strlen(code) - 1 - i] = buf;
	}
	while (strlen(code) < 8)
		strcat(code, "0");
	return;
}
int makeStructureFromTable(char *filename, struct SYM syms[], struct SYM *psyms[])
{
	FILE *fp;
	fp = fopen(filename, "rb");
	char buf[30];
	unsigned char a;
	int b;
	int countSymbols;
	//fgets(buf, 6, fp);
	if (fp == NULL)
		return 0;
	fread(buf, sizeof(char), 5, fp);
	buf[5] = 0;
	if (strcmp(buf, "MDYLL") != 0)
		return -1;
	//fgets(buf, 5, fp);
	//fgets(buf, 4, fp);
	fread(&countSymbols, sizeof(int), 1, fp);
	//countSymbols = atoi(buf);
	if (countSymbols < 1 || countSymbols>256)
		return NULL;
	for (int i = 0; i < countSymbols; i++)
	{
		//fscanf(fp, "%c %f", &a, &b);
		fread(&a, sizeof(unsigned char), 1, fp);
		fread(&b, sizeof(int), 1, fp);
		//fgetc(fp);
		syms[i].ch = a;
		syms[i].freq = b;
		strcpy(syms[i].code, "");
		syms[i].left = NULL;
		syms[i].right = NULL;
		psyms[i] = (struct SYM*)malloc(sizeof(struct SYM));
		psyms[i] = &syms[i];
	}
	return countSymbols;
}

int convertCompToBin(char *filename, int countSymbols, char *ext)
{
	char a, c;
	int b;
	char buf[255], fileExt[5];
	unsigned char code[9];
	char *file101;
	unsigned char ch;
	int j = 0;
	int fileSize;
	int sizeOfOriginalFile;
	int tail;
	FILE *fp_in = fopen(filename, "rb");
	file101 = strtok(filename, ".");
	strcat(file101, ".101");
	FILE *fp_out = fopen(file101, "w");
	fread(buf, sizeof(char),5, fp_in);
	fread(&b, sizeof(int), 1, fp_in);
	int aa;
	for (int i = 0; i < countSymbols; i++)
	{
		fread(&a, sizeof(char), 1, fp_in);
		fread(&b, sizeof(int), 1, fp_in);
	}
	fread(&tail, sizeof(int), 1, fp_in);
	fread(&sizeOfOriginalFile, sizeof(int), 1, fp_in);
	fread(ext, sizeof(char), 3, fp_in);
	ext[3] = 0;
	fread(&fileSize, sizeof(long), 1, fp_in);
	for (int i = 0; i < fileSize / 8 - 1; i++)
	{
		fread(&ch,sizeof(unsigned char),1,fp_in);
		unPack(code, ch);
		fputs((char*)code, fp_out);
	}
	ch = fgetc(fp_in);
	unPack(code, ch);
	
	for (int i = 0; i < tail; i++)
		code[7 - i] = 0;

	fputs((char*)code, fp_out);
	fclose(fp_in);
	fclose(fp_out);

	return sizeOfOriginalFile;
}

void makeOriginalFile(char *filename, struct SYM syms[],char *ext)
{
	char file101[50];
	char file_out[50];
	char fileExt[10];
	FILE *fp101;
	FILE *fp_out;
	char sequence[256];
	unsigned int ch;
	int i = 0, j;
	while (filename[i] && filename[i] != '.')
	{
		file101[i] = filename[i];
		file_out[i] = filename[i];
		i++;
	}
	file101[i] = '\0';
	file_out[i] = '\0';
	strcat(file_out, ".");
	strcat(file_out, ext);
	strcat(file101, ".101");
	//strcat(file_out, ".102");
	fp101 = fopen(file101, "r");
	fp_out = fopen(file_out, "wb");
	i = 0;
	bool flag;
	while ((ch = fgetc(fp101)) != EOF)
	{
		sequence[i++] = ch;
		sequence[i] = 0;
		j = 0;
		flag = false;
		while (syms[j].freq > 0)
		{
			if (strcmp(sequence, syms[j].code) == 0)
			{
				flag = true;
				break;
			}
			else
				j++;
		}
		if (flag==false)
			continue;
		else
		{
			//fputc(syms[j].ch, fp_out);
			fwrite(&syms[j].ch, sizeof(unsigned char), 1, fp_out);
			i = 0;
		}
	}
	fclose(fp101);
	fclose(fp_out);
}
