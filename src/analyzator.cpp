#include <stdio.h>
#include "header.h"



void makeSymArray(struct SYM *buf, char letter)
{
	buf[letter].freq++;
}

void initializeArray(struct SYM* buf)
{
	for (int i = 0; i < N; i++)
	{
		buf[i].ch = (unsigned char)(i-'\0');
		buf[i].freq = 0;
		buf[i].code[0] = '\0';
		buf[i].left = nullptr;
		buf[i].right = nullptr;
	}
}

void calculateFreq(struct SYM*buf, long long count)
{
	for (int i = 0; i < N; i++)
	{
		buf[i].freq = buf[i].freq / (float)count;
	}
}

int comp(const void * x1, const void * x2)
{
	return (((struct SYM*)(x2))->freq - ((struct SYM*)(x1))->freq);
}

void printArray(struct SYM* buf)
{
	float totalFreq = 0.0;
	for (int i = 0; i < N; i++)
	{
		if (buf[i].freq == 0.0)
			break;
		else
		{
			printf("Symbol - %c\tFrequency - %f\n", buf[i].ch, buf[i].freq);
			totalFreq = totalFreq + buf[i].freq;
		}

	}
	printf("Total frequency = %f\n", totalFreq);
}