#define UNIQ_CODE "aSiN"
#define COMPRSD_EXTANSION ".asin"
#define TEMP_FILE_NAME "101.txt"
#define COMMAND_TO_COMPRSD "c"
#define COMMAND_TO_DECOMPRSD "d"
#define SIZE_OF_UNIQ_CODE 4
#define CODE_SIZE 256
#define SYMS_SIZE 256
#define PSYMS_SIZE 512
#define SIZE_FILE_NAME 256
#define SIZE_FILE_EXTANSION 10


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct SYM
{
    unsigned char ch;
    float freq;
    char code[CODE_SIZE];
    struct SYM* left;
    struct SYM* right;
};

typedef unsigned long int uli;
typedef unsigned char uc;
typedef struct SYM tSYM;
typedef tSYM* pSYM;

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


void clean_buf(char* buf, int size);
int create_syms(FILE* file, tSYM* syms);
void sort_syms(tSYM* buf, int size);
void create_psyms(tSYM* syms, pSYM* psyms, int number_of_syms);
void print(pSYM* buf, int size);
pSYM build_tree(pSYM p_sym[], int N, int size);
void makeCodes(pSYM root);
void coder(FILE* in, FILE* out);
void decoder(FILE* in, FILE* out);
int file_101_to_decopressed_file(FILE* fp_101, FILE* fp, tSYM* syms, uli size_of_101, int number_of_syms);