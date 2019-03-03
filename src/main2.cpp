#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "task2.h"



int main()
{
	FILE *fp_in = fopen("in.txt", "rb"); // ��������� ��� ���� ��� ������
	FILE *fp_temp = fopen("temp.txt", "wb"); // ��������� ���� ��� ������ ��������� ����
	FILE *fp_010 = fopen("010.txt", "wb"); // ��������� ���� ��� ������ ������� �����

	int info;  // � ��� ���������� �������� ���������� �� �����
	int different_letters = 0; //������� ���������� ��������� ����, ���������� ��������
	int all_sign = 0; // ������� ���������� ���� ������ � �����
	int fsize = 0;//������� ���������� �������� �� 0 � 1 � ������������� ����� temp
	int tail_size;//������ ������ ����� (��, ��� �� ������ 8 � ������������� �����)
	int numb_unique_symb[256] = { 0 };//�������������� ������ ���������� ���������� ��������
	struct SYM symb[256] = { 0 }; //�������������� ������ ������� 
	struct SYM *psym[256]; //�������������� ������ ���������� �� ������
	float sum = 0;//����� ������ �������������
	int arr[8];//������ 0 � 1
	char j = 0;//��������������� ����������

	//��������� ������ ������ �����
	if (fp_in == NULL)
	{
		puts("Error! File not open");
		return 0;
	}

	struct SYM *symbols = (struct SYM*)malloc(different_letters * sizeof(struct SYM));//�������� ������������� ������� �������� symb
	struct SYM **psum = (struct SYM**)malloc(different_letters * sizeof(struct SYM*));//�������� ������������� ������� ���������� �� symb

	//�������� �������� ������ ���� � ���������� ������� �������������
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

	// ������� ������� �������������
	for (int i = 0; i < different_letters; i++)
		symb[i].freq = (float)numb_unique_symb[i] / all_sign;

	for (int i = 0; i < different_letters; i++) //� ������ ���������� ������� ������ �������
		psym[i] = &symb[i];

	//���������� �� �������� 
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

	struct SYM *root = buildTree(psym, different_letters);//����� ������� �������� ������ �������

	makeCodes(root);//����� ������� ��������� ����

	rewind(fp_in);//���������� ��������� � ����� � ������ �����

	//� ����� ������ �������� ����, � ���������� ���������� � �������� ���� � ������������� ����
	while ((info = fgetc(fp_in)) != EOF)
	{
		for (int i = 0; i < different_letters; i++)
			if (info == symb[i].ASCII)
				fputs(symb[i].code, fp_temp);
	}
	fclose(fp_temp);

	//������ ��������� ���� � �������� �����, �� ������ ��� ������
	int i = 0;
	fp_temp = fopen("temp.txt", "rb");
	//������� ������ ��������� �����(���������� �������� � ��)
	while ((info = fgetc(fp_temp)) != EOF)
		fsize++;

	tail_size = fsize % 8;//������� �������, ���������� �������� �� ������� 8 (�����)

	//��������� ��������� ������� ����� ����� ���� ������
	fwrite("compressing", sizeof(char), 24, fp_010);//�������� �������
	fwrite(&different_letters, sizeof(int), 1, fp_010);//���������� ���������� ��������
	fwrite(&tail_size, sizeof(int), 1, fp_010);//�������� ������
	//���������� � ������ ���� ������� �������������
	for (i = 0; i < different_letters; i++)
	{
		fwrite(&symb[i].ASCII, sizeof(struct SYM), 1, fp_010);
		fwrite(&symb[i].freq, sizeof(struct SYM), 1, fp_010);
	}

	rewind(fp_temp);//���������� ��������� � ������������� ����� � ������ �����

	union CODE code;//�������������� ���������� code1
	//������ �������� ����, ������ ��������������� ������ 8 ��������� � ������ ��� ����������� ��������� ��������� � ����������� union
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
	//���������� �����
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

//	fcloseall(); // ��������� ��� �������� �����

	return 0;
}