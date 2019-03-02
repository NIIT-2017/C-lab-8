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

struct SYM
{
	unsigned char ch;
	float freq;
	char code[256];
	struct SYM *left;
	struct SYM *right;
};

int sizeFileIn(const char * fileName);
int makebuf(unsigned char *, float *, const char * fileName);
void makestruct(int, struct SYM[], struct SYM*[], unsigned char *, float *);
struct SYM *buildTree(struct SYM *psym[], int N);
void makeCodes(struct SYM *root);
int CodeBlock(int N, int tail, struct SYM[], const char * fileName);
void pack(int N, int tail, struct SYM syms[256], const char * fileName, unsigned int size_in);
int repack(int N, unsigned char *symbol, float *number, const char * fileName, unsigned int *psize_in);
void finish(struct SYM *root, const char * fileName, unsigned int size_in);