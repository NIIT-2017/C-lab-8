#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "task1.h"
int tail = 0;//����� ������ � ����� 101
/*
��������� ��������� ������:
1. ����\���_����� C - ���������� (� ��������� ���������� ��������� ������ ���� � ������ "���_���������_�����.krv")
2. ����\���_�����.krv D - ������������ (� ��������� ���������� ��������� �������� ���� � ������ "���_���������_�����"). �������� - ���� � ���������� ��� ���� ���� � ����� ������, �� ����� �����������.
��� ���������� ����� �������� �������� �� ������������ ���������, � ����� �� ������������ �������� (� ������) ��������� � ��������� ������.
*/
int main(int argc, const char * argv[])
{
	if (argc < 3)
		printf("Arguments not found");
	struct SYM syms[256];
	struct SYM *psym[256];
	
	unsigned char symbol[256];//������ ��� ��������, ��������� � ��������� �����
	float number[256];//������� ��� �������� � ������������� ���������
	int N = 0;
	unsigned int size_in = 0;//���������� ��� �������� ������� ��������� �����
		
	if (*argv[2] == 'C')
	{
		size_in = sizeFileIn(argv[1]);
		
		N = makebuf(symbol, number, argv[1]);
		makestruct(N, syms, psym, symbol, number);
		struct SYM *root = buildTree(psym, N);
		makeCodes(root);
		tail = CodeBlock(N, tail, syms, argv[1]);
		pack(N, tail, syms, argv[1], size_in);
	}
	if (*argv[2] == 'D')
	{
		unsigned int *psize_in = &size_in;
		N = repack(N, symbol, number, argv[1], psize_in);
		makestruct(N, syms, psym, symbol, number);
		struct SYM *root = buildTree(psym, N);
		if (tail != 0)
			finish(root, argv[1], size_in);
		else
		{
			tail = tail + 8;
			finish(root, argv[1], size_in);
		}
	}
	getchar();
	return 0;
}
