#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head1.h"

int makeCodeFile(SYM* table , FILE* fp_in, FILE* fp_101) {
    int ch=0;
    long length = 0;
    int tailLenght = 0;
    while ((ch = fgetc(fp_in)) != -1) {
        for (int i = 0; i < 256; i++) {
            /*if (table[i].ch == (unsigned char)ch) {
                fputs(table[i].code, fp_101);
                break;
            }*/

            if (table[i].ch == (unsigned char)ch) {
                fputs(table[i].code, fp_101);
                break;
            }
        }
    }
    fseek(fp_101, 0L, SEEK_END);
    length = ftell(fp_101);
    fseek(fp_101, 0L, SEEK_SET);
    tailLenght = length % 8;
    return tailLenght;
}

unsigned char pack(unsigned char buf[]) {
    union CODE code;
    code.byte.b1 = buf[0] - '0';
    code.byte.b2 = buf[1] - '0';
    code.byte.b3 = buf[2] - '0';
    code.byte.b4 = buf[3] - '0';
    code.byte.b5 = buf[4] - '0';
    code.byte.b6 = buf[5] - '0';
    code.byte.b7 = buf[6] - '0';
    code.byte.b8 = buf[7] - '0';
    return code.ch;
}

void makeHeader(FILE *fp, FILE *fpResult, char signsture, int uniqueSymbols, SYM* table, int tailLength, long sizeOfFile, char* fileExtension) {
    //Write down the signature
    fwrite(&signsture, sizeof(char), 1, fpResult);
    //Write down number of unique symbols
    fwrite(&uniqueSymbols, sizeof(int), 1, fpResult);
    //Write down the table
    for (int i = 0; i < uniqueSymbols; i++) {
        fwrite(&table[i].ch, sizeof(char), 1, fpResult);
        fwrite(&table[i].freq, sizeof(float), 1, fpResult);
    }
    //Write down tail lenght
    fwrite(&tailLength,  sizeof(int), 1, fpResult);
    //Write down size of file 
    fwrite(&sizeOfFile,  sizeof(int), 1, fpResult);
    //Write down extension of file
    fwrite(fileExtension,  sizeof(char), strlen(fileExtension), fpResult);
}
