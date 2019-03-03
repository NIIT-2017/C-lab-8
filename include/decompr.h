#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 256


struct SYM
{
	unsigned char ch;
	float freq;
	char code[N];
	struct SYM *left;
	struct SYM *right;
};

typedef struct SYM SYM;
typedef struct SYM * PSYM;

union CODE
{
	unsigned char ch;
	struct
	{
		unsigned short b1 : 1;
		unsigned short b2 : 1;
		unsigned short b3 : 1;
		unsigned short b4 : 1;
		unsigned short b5 : 1;
		unsigned short b6 : 1;
		unsigned short b7 : 1;
		unsigned short b8 : 1;
	} byte;
};

void unPack(unsigned char buf[], unsigned char ch);
void decoding(const char* file1, const char *file2, PSYM root, int size);
void printStruct(PSYM syms, int uniqueCount);
PSYM buildTree(PSYM psym[], int size);
void makeCodes(PSYM root);