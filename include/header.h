#define N 256

struct SYM			// main structure
{
	unsigned char ch; 
	float freq; 
	char code[256]; 
	struct SYM *left; 
	struct SYM *right; 
};

// COMPRESSOR:

// analyzator functions:
void makeSymArray(struct SYM *buf, char letter);
void initializeArray(struct SYM* buf);
void calculateFreq(struct SYM*buf, long long count);
int comp(const void * x1, const void * x2);
void printArray(struct SYM* buf);

// cofe generator functions:
struct SYM* buildTree(struct SYM *psym[], int M);
void makeCodes(struct SYM *root);

// pack file
int packFile(struct SYM *buf, int tail, char* filename, long symCounter);
unsigned char pack(char buf[]);

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

// DECOMPRESSOR:
int decompress(char* filename);
unsigned char* unpack(unsigned char *string, unsigned char letter);
