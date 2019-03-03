#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 256

typedef unsigned long long ULL;
typedef unsigned int UI;
typedef struct SYM SYM;
typedef struct SYM * PSYM;

struct SYM // представление символа
{
	unsigned char ch; // ASCII-код
	float freq; // частота встречаемости
	char code[256]; // массив для нового кода
	struct SYM *left; // левый потомок в дереве
	struct SYM *right; // правый потомок в дереве
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

void ListCreator(PSYM buf, unsigned char ch, ULL count, UI *ch_uniq);
void sortList(PSYM str, int size);
struct SYM* buildTree(struct SYM *psym[], int N);
void makeCodes(struct SYM *root);
unsigned char pack(unsigned char buf[]);