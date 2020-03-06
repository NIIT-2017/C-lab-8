#include<stdio.h>

#define SIZE 256


struct SYM // представление символа
{
	unsigned char ch; // ASCII-код
	float freq; // частота встречаемости
	char code[256]; // массив для нового кода
	struct SYM* left; // левый потомок в дереве
	struct SYM* right; // правый потомок в дереве
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
struct SYM* buildTree(struct SYM* psym[], int N);
void makeCodes(struct SYM* root);
void print(struct SYM* syms);
int cmp(const void* a, const void* b);
unsigned char pack(unsigned char buf[]);
void unpack(unsigned char buf[],unsigned char ch);
void searchTree(struct SYM* root, FILE* in, FILE* out);
long int filesize(FILE* fp);


