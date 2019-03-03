struct SYM
{
	char symbol;
	char code[256];
	float freq = 0;
	unsigned long count = 0;
	struct SYM *left=nullptr;
	struct SYM *right=nullptr;

};

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
	}byte;
};

void makeCodes(struct SYM * root);
void printTree(struct SYM * root);
int compareByFreq(const void *_a, const void* _b);
void writeContent(char *file101, char *fileOut);
unsigned char pack(unsigned char buf[]);
void  readConten(char *file_in, char *file_out, struct SYM *psym[]);
void writeSignature(char *file_out, int countSymbols, struct SYM *psym[], int tail, long fileSize, char *fileExt,int count);
int fillCodeFile(char *filename, char *file101, struct SYM *syms);
struct SYM * buildTree(struct SYM * psym[], int N , int countSymbols);
struct SYM * createTree(struct SYM * root, struct SYM item);
