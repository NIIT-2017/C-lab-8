#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct SYM
{
    unsigned char symbol;
    float frequency;
    char code[256];
    SYM* left;
    SYM* right;
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
    } byte;
};

int printChart(SYM* chart, int size);
SYM* findOut(SYM* chart, FILE* fp, int* numberOfSymbols, int* size);
int compar(const void* p1, const void* p2);
SYM** makePsym(SYM** psym, SYM* chart, int size);
SYM* buildTree(SYM* psym[], int N);
int compar(const void* p1, const void* p2);
void makeCodes(SYM* root);
int write101(SYM* chart, FILE* fp, FILE* out, int size);
void writeCode(FILE* fp, FILE* out, int numberOfSymbols, SYM* chart, int tail, int size, char* id, char* fileType);
int check(FILE* fp, char* id);
SYM* buildChart(SYM* chart, int numberOfSymbols, FILE* fp);
long int printText(FILE* fp, SYM* root, int tail);
long int writeDownText(FILE* fp, FILE* out, SYM* root, int tail);
