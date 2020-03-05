#define SIZE 256
#define BIT 8
#define SIGNATURA "poppy"

struct SYM
{
	unsigned char ch;
	float freq;
	char code[SIZE];
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

                            //      COMPRESSOR  

int Analyzer(char* argv, struct SYM* syms, struct SYM** psysms, int* sumCh);
struct SYM* buildTree(struct SYM* psym[], int N);
void makeCodes(struct SYM* root);
void printTree(struct SYM* root, int level);
FILE* CoderLine(struct SYM* syms, int N, FILE*fp_in, FILE* fp_101);
int makeFirstLine(FILE* fp_101, FILE* fp_finish, int N, struct SYM* syms, int sum_ch, char* argv);
void Packing(FILE* fp_101, FILE* fp_finish, int N, struct SYM* syms, int sum_ch, char* argv);
void deleteAll(struct SYM* root);


                            //    DE - COMPRESSOR   
void UnPacking(FILE* fp_finish, FILE* fp_FENIX, int simbol_format);
