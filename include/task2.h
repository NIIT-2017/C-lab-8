#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 512
#define LEN 256

struct SYM
{
	unsigned char ch;
	float freq;
	char code[256];
	struct SYM* left;
	struct SYM* right;

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

unsigned char pack(unsigned char buf[]);
struct SYM* buildTree(struct SYM* psym[], int N);
void makeCodes(struct SYM* root);
char* chomp(char* str);
int cmp(const void* a, const void* b);
unsigned char* unpack(unsigned char ch, unsigned char* buf);
unsigned char searchTree(struct SYM* node, char* code);
void printTree(struct SYM* node);