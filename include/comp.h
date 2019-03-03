struct SYM
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

FILE* OpenFile(char name_file[], const char mode[]);
char* AddFileExtension(const char source[], char dest[], const char extension[]);
int MakeTable(FILE *fp_in, struct SYM *psyms);
struct SYM * buildTree(struct SYM *psym[], int N);
void makeCodes(struct SYM *root);
void Coder(FILE *fp_in, FILE *fp_101, struct SYM *psym, int diff_count);
int SearchTail(FILE *fp_101, int *sizeControlFile);
void CreateTitleFile(FILE *fp_archive, int diff_count, struct SYM *psym, int tail);
unsigned char pack(unsigned char buf[]);
void CreateFile(FILE *fp_101, FILE * fp_archive, int sizeControlFile, int tail);
char *DelFileExtension(const char source[], char dest[]);
int ReadTitleFile(FILE *fp_archive, int *diff_count, struct SYM *psym);
void ReadFile(FILE *fp_101, FILE *fp_archive, int tail);
unsigned char SearchCode(FILE *fp_101, struct SYM *root);

void comp(char FileName_fp_in[]);
void decomp(char FileName_fp_archive[]);