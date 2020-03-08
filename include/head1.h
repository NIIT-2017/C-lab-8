struct SYM {
    unsigned char ch;
    float freq;
    char code[256];
    struct SYM* left;
    struct SYM* right;
};

typedef struct SYM TSYM;     //struct variable
typedef TSYM* PSYM;         // struct pointer

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

void initializeTable(SYM* table, char* str);
void makeTable(SYM* table, char* str, float total);
void printTable(SYM* table);
void smartPrintTable(SYM* table);
int cmp(const void* a, const void* b);
int searchUniqueSymbols(SYM* table);
struct SYM* buildTree(struct SYM* psym[], int N);
void makeCodes(struct SYM* root);
int makeCodeFile(SYM* table, FILE* fp_in, FILE* fp_101);
unsigned char pack(unsigned char buf[]);
void makeHeader(FILE* fp, FILE* fpResult, char signsture, int uniqueSymbols, SYM* table, int tailLength, long sizeOfFile, char* fileExtension);
char* getFileExtension(char* str);
void readHeader(char* signsture, SYM* table, int* tailLength, long* sizeOfFile, char* fileExtension, FILE* fpResult);
void makeCodeFileForDec(FILE* fpResult, FILE* fp101Dec);
void makeSourseFile(FILE* fp101Dec, FILE* fpDecompressedFile, struct SYM* rootTree, int tailLength);