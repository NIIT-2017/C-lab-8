struct SYM
{
	char symbol;
	char code[256] = { 0 };
	float freq = 0;
	unsigned long count = 0;
	struct SYM *left = nullptr;
	struct SYM *right = nullptr;

};

union CODE {
	unsigned char chr;
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

void makeCodes(struct SYM * root);
int compareByFreq(const void *_a, const void* _b);
char * unpack(unsigned char symbol, char unbuf[]);
void fromNumberToSymbol(char *file101, struct SYM *psym[], char *fileOut);
int readConten(char *file_in, char *file_out, struct SYM *psym[]);
struct SYM * buildTree(struct SYM *psym[], int N, int countSymbols);