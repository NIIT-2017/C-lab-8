#include <stdio.h>
#include <string.h>
#include "SYM.h"

int counter = 0;

void SortingByFreqs(SYM *SYMarray)
{
	unsigned char tempCh;
	float tempFreq;
	for (int i = 0; i < counter; i++)
	{
		for (int j = 0; j < counter; j++)
		{
			if (SYMarray[j].freq < SYMarray[i].freq)
			{
				tempCh = SYMarray[j].ch;
				tempFreq = SYMarray[j].freq;
				SYMarray[j].ch = SYMarray[i].ch;
				SYMarray[j].freq = SYMarray[i].freq;
				SYMarray[i].ch = tempCh;
				SYMarray[i].freq = tempFreq;
			}
		}
	}
}

void FillingSYMarray(SYM *SYMarray, float *frequenciesArray)
{
	char code[256] = { 0 };
	for (int i = 0; i < MAX_SYM_COUNT; i++)
	{
		if (frequenciesArray[i] > 0)
		{
			SYMarray[counter].ch = i;
			SYMarray[counter].freq = frequenciesArray[i];
			SYMarray[counter].left = 0;
			SYMarray[counter].right = 0;
			strcpy(SYMarray[counter].code, code);			//"временное" решение
			counter++;
		}
	}
	SortingByFreqs(SYMarray);
}

void PrintSYMarray(SYM *pSYMarray)
{
	for (int i = 0; i < counter; i++)
		printf("char: %c\tfreq: %.8f\n", pSYMarray[i].ch, pSYMarray[i].freq);
}