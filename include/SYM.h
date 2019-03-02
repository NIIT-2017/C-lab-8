#ifndef SYM_H
#define SYM_H

#define MAX_SYM_COUNT 256

typedef struct SYM
{
	unsigned char ch;
	float freq;
	char code[256];
	struct SYM *left;
	struct SYM *right;
}SYM;

void FillingSYMarray(SYM *SYMarray, float *frequenciesArray);
void PrintSYMarray(SYM *SYMarray);

#endif