#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[])
{
    if (argc != 3)
    {
        printf("Enter 3 arguments!\n");
        printf("Enter \"huffman.exe comp file.txt\" to compress.\n");
        printf("Enter \"huffman.exe decomp file.txt\" to decompress.\n");
        return 1;
    }
    TSYM syms[SIZE];
    TSYM * psym[SIZE];
    char filenameIn[32]; // name of input file with extension
    char name[32]; // name of input file without extension
    char extension[4]; // extension of input file
    char filename101[32]; // name of .101 file
    char filenameOut[32]; // name of output file
    int uCount = 0; // unique symbols
    long sizeIn = 0; // size of input uncompressed file
    long size101 = 0; // size of .101 file
    int tail = 0; // tail length

    if (strcmp(argv[1], "comp") == 0)
    {
        strcpy(filenameIn, argv[2]);
        filenameIn[strlen(filenameIn)] = '\0';
        makeFileName(filenameIn, name, filename101, extension);
        initSyms(psym, syms, &uCount, filenameIn);
        sortSyms(syms);
        TSYM * root = buildTree(psym, uCount);
        makeCodes(root);
        coding(syms, uCount, &sizeIn, &size101, &tail, filenameIn, filename101);
        fillOutputFile(syms, uCount, sizeIn, size101, tail, filename101, filenameOut, extension, name);
        printf("Compression finished!\n");
        return 0;
    }
    else if (strcmp(argv[1], "decomp") == 0)
    {
        long position; // position of the pointer in file
        strcpy(filenameIn, argv[2]);
        filenameIn[strlen(filenameIn)] = '\0';
        makeFileName(filenameIn, name, filename101, extension);
        initSymsD(psym, syms, &uCount, &tail, &sizeIn, &size101, &position, extension, filenameIn);
        TSYM * rootD = buildTree(psym, uCount);
        makeCodes(rootD);
        decoding(position, size101, tail, filenameIn, filename101);
        fillDecompFile(rootD, size101, filename101, filenameOut, name, extension);
        printf("Decompression finished!\n");
        return 0;
    }
}