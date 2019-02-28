#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

struct SYM {
	unsigned char ch;
	float freq;
	char code[256];
	struct SYM *left;
	struct SYM *right;
};
union CODE {
	unsigned char ch;
	struct {
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

unsigned long getArr(const char * fileName, int * arr);
int compare(const void * pArr1, const void * pArr2);
int arr2SYM(int * arr, unsigned long count, struct SYM ** pSYM);
struct SYM* buildTree(struct SYM *psym[], int N);
void makeCodes(struct SYM *root);
int make101File(const char * fileName, struct SYM ** pSYM, int size);
unsigned char pack(unsigned char buf[]);
void makeOutFile(const char * fileOutName, const char * expansion, int size, struct SYM ** pSYM, int bits2Bite);
void path(char * pathIn, char * pathOut, char * expansion);
