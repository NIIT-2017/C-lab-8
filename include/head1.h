

struct SYM {
    unsigned char ch;
    float freq;
    char code[256];
    struct SYM* left;
    struct SYM* right;
};

typedef struct SYM TSYM;     //struct variable
typedef TSYM* PSYM;         // struct pointer

void initializeTable(SYM* table, char* str, float total);
void makeTable(SYM* table, char* str, float total);
void printTable(SYM* table);
void smartPrintTable(SYM* table);
int cmp(const void* a, const void* b);
int cmpT(const void* a, const void* b);
void chomp(char* string);
void printTree(struct SYM* root);
int searchUniqueSymbols(SYM* table);
struct SYM* buildTree(struct SYM* psym[], int N);
void makeCodes(struct SYM* root);
int makeCodeFile(SYM* table, FILE* fp_in, FILE* fp_101);
unsigned char pack(unsigned char buf[]);
void makeHeader(FILE* fp, FILE* fpResult, char signsture, int uniqueSymbols, SYM* table, int tailLength, long sizeOfFile, char* fileExtension);


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

