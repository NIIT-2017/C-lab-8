#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include "task2.h"


struct SYM *buildTree(struct SYM *psym[], int N) // �e��������� ������� ���������� ��������� �������
{
	// ������ ��������� ����
	struct SYM *temp = (struct SYM*)malloc(sizeof(struct SYM));
	// � ���� ������� ������������ ����� ������ ���������� � �������������� ��������� ������� psym
	temp->freq = psym[N - 1]->freq + psym[N - 2]->freq;
	// ��������� ��������� ���� � ����� ���������� ������
	temp->left = psym[N - 1];
	temp->right = psym[N - 2];
	temp->code[0] = 0;

	if (N == 2)  // �� ������������ �������� ������� � �������� 1.0
		return temp;
	else // ��������� temp � ������ ������� psym, �������� ������� �������� �������
	{
		for (int i = 0; i < N; i++)
			if (temp->freq > psym[i]->freq)
			{
				for (int j = N - 1; j > i; j--)
					psym[j] = psym[j - 1];

				psym[i] = temp;
				break;
			}
	}
	return buildTree(psym, N - 1);
}

//======================================================================================

/*
������ ������� ������� ������, ������� ������� ���������� �� ������� ����� ������
� �������� "0" ��� "1" � ����������� �� ����� ��� ������ �����
*/
void makeCodes(struct SYM *root) // ����������� ������� ��������� ����������� ����
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

//======================================================================================
