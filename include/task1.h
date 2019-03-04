struct SYM
{
	unsigned char ch;
	float freq;
	char code[255];
	SYM *left;
	SYM *right;
};

typedef struct SYM TSYM;
typedef TSYM* PSYM;

union CODE
{
	unsigned char ch;
	struct 
	{
		unsigned b1 : 1;
		unsigned b2 : 1;
		unsigned b3 : 1;
		unsigned b4 : 1;
		unsigned b5 : 1;
		unsigned b6 : 1;
		unsigned b7 : 1;
		unsigned b8 : 1;
	}byte;
};

SYM *makeTree(SYM *PSYM[], int number);
int compare(const void*a, const void *b);
void makeCodes(SYM *root);
void Sort(PSYM str, int size);
void uncompress(unsigned char buf[], unsigned char ch);
void decodes(const char* file1, const char *file2, PSYM root, int size);