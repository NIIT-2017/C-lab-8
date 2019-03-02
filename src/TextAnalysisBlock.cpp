#include <stdio.h>
#include "SYM.h"
#include "TextAnalysisBlock.h"

int getSymbolsCount(FILE *book)		//������� ���-�� �������� � �����
{
	fseek(book, 0, SEEK_END);
	int counter = ftell(book);
	fseek(book, 0, SEEK_SET);
	return ++counter;				//���� ����� ������				
}

int MakeFreqsArray(FILE *book, float *freqs)		//���������� ������� ������ �������������
{
	int SymbolsCount = getSymbolsCount(book), SYMcount = 0;
	while (!feof(book))
		freqs[(unsigned char)fgetc(book)]++;
	for (int i = 0; i < MAX_SYM_COUNT; i++)
	{
		freqs[i] /= SymbolsCount;
		if (freqs[i] != 0)
			SYMcount++;
	}
	return SYMcount;
}