#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct SYM
{
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

typedef struct SYM TSYM;
typedef TSYM* PSYM;

struct SYM* buildTree(struct SYM *psym[], int N);
void makeCodes(struct SYM *root);
unsigned char pack(unsigned char buf[]);
void createBuf(PSYM buf, unsigned char ch, int count, int *word);
void printBuf(PSYM buf, int word);
void bufSort(PSYM str, int size);
unsigned char* repack(unsigned char temp, unsigned char *buf);
void decoding(const char* file1, const char* file2, PSYM root);