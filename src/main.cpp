#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#define SIZE 256

int main(int argc, char* argv[])
{
	FILE* fp_in;                       // ��������� �� �������� ����
	FILE* fp_101;                      // ��������� �� ���� � �������� ����� (��������� � ������ � ���������)
	FILE* fp_vvk;                      // ��������� �� ������ ����

	char resultName[SIZE] = { 0 };       // ������ ��� ������ ����� ������� ����� � ����� �����������
	strcpy(resultName, argv[1]);

	fp_101 = fopen("temp.txt", "wb");                   // ��������� ���� ��� ������ ��������� ����
	fp_vvk = fopen(strcat(resultName, ".vvk"), "wb");   // ��������� ���� ��� ������ ������ ���������� � ����������� ��� ����� ����������

	int tempCh;                        // � ��� ���������� �������� ���������� �� �����
	int count = 0;                     // ������� ���������� ���������� �������� � ���������
	int globalCount = 0;               // ������� ���������� ���� �������� � ���������
	int count101 = 0;                  // ������� ���������� �������� �� 0 � 1 � ������������� .101 �����
	int quantity[SIZE] = { 0 };        // �������������� ������ ���������� ���������� ��������
	struct SYM symbols[SIZE];          // �������������� ������ �������� ���� SYM
	struct SYM* psym[SIZE];            // �������������� ������ ���������� �� ��������� ���� SYM
	float sumFreq = 0;                 // ����� ������ �������������

	if ((fp_in = fopen(argv[1], "rb")) == NULL)               // ���������� ������ ������/���������� �����
	{
		puts("File not found!"); 
		return 1;
	}

	for (int i = 0; i < SIZE; i++)
	{
		symbols[i].ch = '\0';
	}

	while ((tempCh = fgetc(fp_in)) != EOF)                       // �������� ������ �������� ���� � ���������� ������� �������������
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

	for (int i = 0; i < count; i++)                             // ������� ������� �������������
		symbols[i].freq = (float)quantity[i] / globalCount;

	for (int i = 0; i < count; i++)                             // � ������ ���������� ������� ������ ��������
	{
		psym[i] = &symbols[i];
		psym[i]->left = NULL;
		psym[i]->right = NULL;
	}

	struct SYM temp;                                           // ���������� �� ��������, ����� ���������
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

	struct SYM* root = buildTree(psym, count);      // ����� ������� �������� ������ ��������
	makeCodes(root);                                // ����� ������� ��������� ����
	rewind(fp_in);                                  // ���������� ��������� � ����� � ������ �����

	while ((tempCh = fgetc(fp_in)) != EOF)          // ������ ��������, � ���������� ���������� ����� makeCodes ���� � .101 ����
	{                                               // ������ ����������� ������ ��������������� � ������� �������� SYM � ���� ch
		for (int i = 0; i < count; i++)
			if (symbols[i].ch == tempCh)             // ����� ���������� ���������, ���������� ������� � .101 ���� ���������� ���� code
			{
				fputs(symbols[i].code, fp_101);     // ������� ������ � �����
				break;                              // ��������� �����
			}
	} 
	fclose(fp_101); //������� �������, ����� �������,����� ����������

	int i = 0;
	fp_101 = fopen("temp.txt", "rb");               // ����� ��������� ������������� .101 ���� � �������� ����� (��� ������)

	while ((tempCh = fgetc(fp_101)) != EOF)          // ������� ���������� ������ ������ � .101 ����� (���������� ��� ������)
		count101++;

	int tail = count101 % 8;                        // ������� �������, ������ ������ ����� (���������� �������� �� ������� 8 � .101 �����)
	
	//��������� ��������� ������� ����� ����� ���� ������
	fwrite("vvk", 3, 1, fp_vvk);					// ��������� (������� �������)
	fwrite(&count, 3, 1, fp_vvk);                   // ���������� ���������� �������� (����� ����� � ������� �������������)
	for (i = 0; i < count; i++)                      // ���� ������� ������������� (����: ��� ������� - �������)
	{
		fwrite(&symbols[i].ch, 3, 1, fp_vvk);
		fwrite(&symbols[i].freq, 4, 1, fp_vvk);
	}
	fwrite(&tail, 1, 1, fp_vvk);                    // ����� ������ (������� ������� ������� .101 ����� �� 8)
		/* ����� ��������� */

	rewind(fp_101);                                 // ���������� ��������� � ������������� ����� � ������ �����
	unsigned char buf[8] = { 0 };                     // �������������� ������ ������ � ����� ��� union
	union CODE code;                                // �������������� ���������� code ��� �-� pack
	char j = 0;

	for (int i = 0; i < count101 - tail; i++)        // ������ .101 ����, ������ ��������������� ������ 8 ��������� � ������ ��� ��������� ��������� � union
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
	for (int i = 0; i <= tail; i++)                  // ���������� �����
	{
		buf[j] = fgetc(fp_101);
		if (j == tail)
		{
			//pack(buf);
			fputc(pack(buf), fp_vvk);
		}
		j++;
	}

	_fcloseall();  //��������� ��� �������� �����
	return 0;
}