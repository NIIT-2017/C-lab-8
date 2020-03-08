#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head1.h"

void readHeader(char* signsture, SYM* table, int* tailLength, long* sizeOfFile, char* fileExtension, FILE* fpResult) {
    int uniqueSymbols = 0;
    char symbol = 0;
    int lengthOfFileExtension = 0;
    //Read the signature
    fread(signsture, sizeof(char), 1, fpResult);
    //Read the number of unique symbols
    fread(&uniqueSymbols, sizeof(int), 1, fpResult);
    //Read the table
    for (int i = 0; i < uniqueSymbols; i++) {
        fread(&symbol, sizeof(char), 1, fpResult);
        fread(&table[symbol].freq, sizeof(float), 1, fpResult);
    }
    //Read tail lenght
    fread(tailLength, sizeof(int), 1, fpResult);
    //Read size of file 
    fread(sizeOfFile, sizeof(int), 1, fpResult);
    //Read extension of file
    fread(&lengthOfFileExtension, sizeof(int), 1, fpResult);
    fread(fileExtension, sizeof(char), lengthOfFileExtension, fpResult);
    printf("Current position after header reading: '%d'\n", ftell(fpResult));
}

void unPack(unsigned char buf[], unsigned char ch) {
    union CODE code;
    code.ch = ch;
    buf[0] = code.byte.b1 + '0'; 
    buf[1] = code.byte.b2 + '0';
    buf[2] = code.byte.b3 + '0';
    buf[3] = code.byte.b4 + '0';
    buf[4] = code.byte.b5 + '0';
    buf[5] = code.byte.b6 + '0';
    buf[6] = code.byte.b7 + '0';
    buf[7] = code.byte.b8 + '0';
}

void makeCodeFileForDec(FILE* fpResult, FILE* fp101Dec) {
    unsigned char ch = 0;
    unsigned char buf[8];
    while (fread(&ch, sizeof(unsigned char), 1, fpResult)) {
        unPack(buf, ch);
            fwrite(buf, sizeof(unsigned char), 8, fp101Dec);

    } 
}

void makeSourseFile(FILE* fp101Dec, FILE* fpDecompressedFile, struct SYM* rootTree, int tailLength) {
    struct SYM* PNODE = rootTree;
    char ch = 0;
    while ((ch = fgetc(fp101Dec)) != -1) {
        if (ch == '0') {
            PNODE = PNODE->left;
        }
        else {
            PNODE = PNODE->right;
        }
        if (PNODE == NULL) {
            printf("Something goes wrong!");
            PNODE = rootTree;
        }
        if (PNODE->left == NULL && PNODE->right == NULL) {
            fputc(PNODE->ch, fpDecompressedFile);
            PNODE = rootTree;
        }
    }
}