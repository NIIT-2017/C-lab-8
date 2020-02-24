#define _CRT_SECURE_NO_WARNINGS
#include "functions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


void CompareSymbols(const DATA *x, const unsigned char ch, FILE *filewr); //����� �������� ���������, ���������������� ������� �� �����
void AddToFile(const LIST*phead, const unsigned char ch, FILE *filewr); //������ � ����� ���� �������� �������� �� 0 � 1

int main()
{
	/*for (unsigned int i = 0; i < 256; ++i)
	{
		printf("%u - %c\n", i, (unsigned char)i);
	}
	return 0;*/
	//��������� �����, ������� ����� �����
	char str[SIZE2] = { 0 };

	FILE * file = fopen("book.txt", "rt"); //rt - ������ � ��������� ������� 
	if (!file)
	{
		puts("Error open file for readind");
		return 0;
	}

	int freq[SIZE1] = { 0 }; //������ ������ ������������� ��������
	int number_symbols_in_file = 0;
	int len = 0;
	while (!feof(file))
	{		
		number_symbols_in_file = number_symbols_in_file + len;
		fgets(str, SIZE2, file); //������ �� ������� �����	
		len = strlen(str);	
		int i = 0;
		while (i < len)
		{
			freq[str[i]]++;
			i++;
		}
	}
	fclose(file);


	/*int freq[SIZE] = { 0 };
	srand(time(0));
	for (int i = 0; i < SIZE; i++)  //���������� ������� ������ ���������� ����������
	{
	int j = rand() % 2;
	if (j)
	{
	freq[i] = rand() % (SIZE/3);
	}
	}*/

	
	//printf("kolichestvo simvolov v file: %d\n", len);
	 
	

	int unique_symbols = 0; //���������� ���������� ��������
	unique_symbols = NumberOfUniqueSymbols(freq);
	printf("Number of unique symbols: %d\n", unique_symbols);

	//����� ��������
	LIST*phead = MakeListFromFreq(freq);
	DATA *root = MakeTreeFromList(phead);
	Print(phead);
	printf("++++++++\n");
	MakeCodeOfSymbol(root);
	PrintTree(root);

	printf("**************\n");
	//�������� �������������� �����
	FILE * file1 = fopen("book.txt", "rt"); //rt - ������ � ��������� ������� 
	if (!file1)
	{
		puts("Error open file for readind");
		return 0;
	}
	FILE * filewr = fopen("file.txt", "wt"); //wt - ������� ���� ��� ������ ��� ��������� �� ������ � �������� ����
	if (!filewr)
	{
		puts("Error open file for writing");
		return 0;
	}
	
	char ch;
	while ((ch = fgetc(file1)) != EOF) //������������ ���������� �����
	{
		//printf("%c\n", ch);
		AddToFile(phead, ch, filewr); //��������� � ������ � ����� ����		
	}
	fclose(file1);
	fclose(filewr);
	
	//������ �������������� ����� � ������ � ����� ����
	char binstr[SIZE2] = { 0 };
	FILE * bincode = fopen("file.txt", "rt"); //rt - ������ � ��������� ������� 
	if (!bincode)
	{
		puts("Error open file for readind");
		return 0;
	}
	while (!feof(bincode))
	{
		fgets(binstr, SIZE2, bincode); //������ �� ������� �����				
	}
	fclose(bincode);



	FILE * result = fopen("result.txt", "wt"); //wt - ������� ���� ��� ������ 
	if (!result)
	{
		puts("Error open file for writing");
		return 0;
	}
	
	fputs("GrigorevaNatalya\n", result);//���������
	fprintf(result, "%d", unique_symbols); //���������� ���������� ��������

	//������� �������������
	for (int i = 0; i < SIZE1; i++)
	{
		if (freq[i] != 0)
		{
			fprintf(result, "%c%d", i, freq[i]);
		}

	}
	fputs("\n", result);
	

	int tail = MakeTail(binstr);
	printf("%d\n", tail);
	fprintf(result, "%d\n", tail); //����� ������
	fprintf(result, "%d\n", number_symbols_in_file); //������ ��������� �����
	fputs(".txt\n", result);//�������� ���������� �����

	int length = strlen(binstr);
	unsigned char buf[9] = { 0 };
	for (int i = 0; i<length; i = i + 8)
	{
		int a = i;
		for (int j = 0; j<8; j++)
		{
			buf[j] = binstr[a];
			a++;
		}
		printf("%s\n", buf);
		printf("%c\n", pack(buf));
		fputc(pack(buf), result);
	}
	fclose(result);




	return 0;
}


void CompareSymbols(const DATA *x, const unsigned char ch, FILE*filewr) 
{
	if (x->symb == (unsigned char)ch)
	{
		//for (int i=0; i<SIZE;i++)
		//if((x->code[i])!='\0')//=="0"||(x->code[i])=="1")
		{
			//printf("**************\n");
			//printf("%s\n", x->code);
			//printf("%c\n", x->symb);
			//printf("%s\n", x->code);
		}


		fputs(x->code, filewr);
	}
}
void AddToFile(const LIST*phead, const unsigned char ch, FILE*filewr)
{
	while (phead)
	{
		{
			CompareSymbols(&(phead->value), ch, filewr);
			phead = phead->next;
		}
	}
}