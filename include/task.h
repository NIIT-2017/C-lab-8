#define SIZE 256

struct SYM
{
    unsigned char ch;
    float freq;
    char code[256];
    SYM * left;
    SYM * right;
};

typedef struct SYM TSYM;

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
    } byte;
};

void makeFileName(char * filenameIn, char * name, char * filename101, char * extension);
void initSyms(TSYM * psym[], TSYM syms[SIZE], int * uCount, char * filenameIn);
void sortSyms(TSYM syms[SIZE]);
TSYM * buildTree(TSYM * psym[], int N);
void makeCodes(TSYM * root);
void coding(TSYM syms[SIZE], int uCount, long *sizeIn, long * size101, int * tail, char * filenameIn, char * filename101);
unsigned char pack(unsigned char buf[]);
void fillOutputFile(TSYM syms[SIZE], int uCount, long sizeIn, long size101, int tail, char * filename101, char * filenameOut, char * extension, char * name);

// DECOMPRESSING

void initSymsD(TSYM * psym[], TSYM syms[SIZE], int * uCount, int * tail, long * sizeIn, long * size101, long * position, char * extension, char * filenameIn);
void unPack(unsigned char buf[], unsigned char ch);
void decoding(long position, long size101, int tail, char * filenameIn, char * filename101);
void fillDecompFile(TSYM * rootD, long size101, char * filename101, char * filenameOut, char * name, char * extension);