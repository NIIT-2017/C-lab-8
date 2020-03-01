#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head1.h"

void readHeader(FILE* fp, FILE* fpResult, char signsture, int uniqueSymbols, SYM* table, int tailLength, long sizeOfFile, char* fileExtension) {
    //Read the signature
    fread(&signsture, sizeof(char), 1, fpResult);
    //Read the number of unique symbols
    fread(&uniqueSymbols, sizeof(int), 1, fpResult);
    //Read the table
    for (int i = 0; i < uniqueSymbols; i++) {
        fread(&table[i].ch, sizeof(char), 1, fpResult);
        fread(&table[i].freq, sizeof(float), 1, fpResult);
    }
    //Read tail lenght
    fread(&tailLength, sizeof(int), 1, fpResult);
    //Read size of file 
    fread(&sizeOfFile, sizeof(int), 1, fpResult);
    //Read extension of file
    fread(fileExtension, sizeof(char), strlen(fileExtension), fpResult);
}


void unPack(unsigned char buf[], unsinged char ch) {
    union CODE code;
    code.ch = ch;
    code.byte.b1 = buf[0] + '0';
    code.byte.b2 = buf[1] + '0';
    code.byte.b3 = buf[2] + '0';
    code.byte.b4 = buf[3] + '0';
    code.byte.b5 = buf[4] + '0';
    code.byte.b6 = buf[5] + '0';
    code.byte.b7 = buf[6] + '0';
    code.byte.b8 = '\0';
}