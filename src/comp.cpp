#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "comp.h"
#include <string.h>
#include <sys/stat.h>

int compareByFreq(const void *_a, const void *_b)
{
	struct SYM *a = *(struct SYM**)_a, *b = *(struct SYM**)_b;
	if ((a)->freq < (b)->freq)
		return 1;
	else if ((a)->freq > (b)->freq)
		return -1;
	else if ((a)->ch < (b)->ch)
		return 1;
	else
		return -1;
}

struct SYM *makeStructure(char *filename,struct SYM syms[],struct SYM *psyms[])
{
	FILE *fp;
	int ch;
	int countChar[256] = { 0 };
	int count=0;

	fp = fopen(filename, "rb");
	if (fp == NULL)
		return NULL;
	while ((ch = fgetc(fp)) != -1)
	{
		countChar[ch]++;
		count++;
	}
	for (ch = 0; ch < 256; ch++)
	{
		psyms[ch] = (struct SYM*)malloc(sizeof(struct SYM));
		syms[ch].freq = countChar[ch]; // (int)(((float)countChar[ch] / count) * 1000);
		syms[ch].ch = ch;
		strcpy(syms[ch].code, "");
		syms[ch].left = NULL;
		syms[ch].right = NULL;
		psyms[ch] = &syms[ch];

	}
	
	return psyms[0];
}

struct SYM *buildTree(struct SYM *psyms[], int N, int totalSymbols)
{
	struct SYM *temp = (struct SYM*)malloc(sizeof(struct SYM));
	if (N == 1)
	{
		return psyms[0];
	}
	temp->freq = psyms[N - 1]->freq + psyms[N - 2]->freq;
	temp->left = psyms[N - 1];
	temp->right = psyms[N - 2];
	if (N == 2)
	{
		temp->ch = 0;
		strcpy(temp->code, "");
		return temp;
	}
	psyms[totalSymbols*2 - N] = temp;
	struct SYM *buf1;
	for (int i = totalSymbols * 2 - N - 1; i >= 0; i--)
		if (psyms[i+1]->freq > psyms[i]->freq)
		{
			buf1 = psyms[i+1];
			psyms[i+1] = psyms[i];
			psyms[i] = buf1;
		}
		else
			break;
	return buildTree(psyms, N - 1,totalSymbols);
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

int fillCodeFile(char *filename, char *file101, struct SYM syms[])
{
	FILE *fp_in, *fp_out;
	unsigned char ch;
	int j;
	int count=0;
	fp_in = fopen(filename, "rb");
	fp_out = fopen(file101, "w+");
	while ((j=fread(&ch,sizeof(unsigned char),1,fp_in))==1)
		for	(int i=0;i<256;i++)
			if (syms[i].ch == ch)
			{
				fputs(syms[i].code, fp_out);
				count += strlen(syms[i].code);
				break;
			}
	int tail = count % 8;
	for (int i = 8 - tail; i > 0; i--)
		fputs("0", fp_out);
	//fputs("\n", fp_out);
	fclose(fp_in);
	fclose(fp_out);
	if (tail == 0)
		tail = 8;
	return 8 - tail;
}

void makeFiles(char *filename, char *file101, char *file_out, char *fileExt)
{
	int i = 0;
	while (filename[i] && filename[i] != '.')
	{
		file101[i] = filename[i];
		file_out[i] = filename[i];
		i++;
	}
	file101[i] = '\0';
	file_out[i] = '\0';
	strcat(file101, ".101");
	strcat(file_out, ".mdy");
	i++;
	int j = 0;
	while (filename[i])
		fileExt[j++] = filename[i++];
	fileExt[j] = '\0';
	return;
}

void writeSignature(char *file_out, int countSymbols, struct SYM syms[], int tail, long int fileSize, char *fileExt)
{
	char buf[30];
	FILE *fp_out;

	fp_out = fopen(file_out, "wb");

	//fputs("MDYLL\n", fp_out);
	fwrite("MDYLL", sizeof(char),5 , fp_out);
	//sprintf(buf, "%d", countSymbols);
	//fputs(buf, fp_out);
	fwrite(&countSymbols, sizeof(int), 1, fp_out);
	for (int i = 0; i < 256; i++)
		if (syms[i].freq > 0)
		{
			//sprintf(buf, "%c%d", syms[i].ch, syms[i].freq);
			//fputs(buf, fp_out);
			fwrite(&syms[i].ch, sizeof(char), 1, fp_out);
			fwrite(&syms[i].freq, sizeof(int), 1, fp_out);
		}
	//sprintf(buf, "%d\n", tail);
	//fputs(buf, fp_out);
	fwrite(&tail, sizeof(int), 1, fp_out);
	//sprintf(buf, "%ld\n", fileSize);
	//fputs(buf, fp_out);
	fwrite(&fileSize, sizeof(int), 1, fp_out);
	//sprintf(buf, "%s\n", fileExt);
	//fputs(buf, fp_out);
	fwrite(fileExt, sizeof(char), 3, fp_out);
	fclose(fp_out);
	return;
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

void writeContent(char *file101, char *file_out)
{
	FILE *fp_in;
	FILE *fp_out;
	unsigned char buf[8];
	unsigned char ch;
	int j = 0;
	fp_in = fopen(file101, "r");
	fp_out = fopen(file_out, "ab");
	struct stat st;
	stat(file101, &st);
	long fileSize = st.st_size;
	//fprintf(fp_out, "%ld\n", fileSize);
	fwrite(&fileSize, sizeof(long), 1, fp_out);
	while ((buf[0] = fgetc(fp_in))=='0'||buf[0]=='1')
	{
		for (int i = 1; i <= 7; i++)
			buf[i] = fgetc(fp_in);
		ch = pack(buf);
		//fputc(ch, fp_out);
		fwrite(&ch, sizeof(unsigned char), 1, fp_out);
		j++;
	}
	fclose(fp_in);
	fclose(fp_out);
}