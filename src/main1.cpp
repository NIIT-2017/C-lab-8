#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head1.h"

int main(int argc, char* argv[]) {
    //Variables and arrays
    int i = 0;
    int N = 256;
    struct SYM* root = NULL;
    char text[256];
    struct SYM table[256];
    struct SYM* psym[256];
    char buf[8];
    int tail = 0;
    int result = 0;
    char sig = 3;
    unsigned char packedCh;
    int countOfUniqueSymbols = 0;
    char* fileName;
    char* fileExtension;
    char* outFileName;

    //Start

    if (argc < 2) {
       printf("You should call the programme with parameters, for example: analysis.exe compress file_name.txt\n");
       return 1;
    }

    //Work with app params

    if (argc == 2) {
        fileName = argv[1];
    } else if (strcmp(argv[1], "compress")) {
        fileName = argv[2];
    } else {
        printf("Unfortunatly decompressor isn`t ready yet =( Please, try tommorow.\n");
        return 1;
    }

    //Work with files
    //1 Work with sourse file
    FILE* fp = fopen(fileName, "rb");
    if (!fp) {
        printf("Error! File %s couldn't be opened!\n", fileName);
        return 1;
    }
    long len = 0;
    long lenResult = 0;
    fseek(fp, 0L, SEEK_END);
    len = ftell(fp);
    lenResult = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    initializeTable(table, text, len);

    while (fgets(text, 256, fp)) {
        makeTable(table, text, len);
    }
    qsort(table, 256, sizeof(SYM), cmp);
    printTable(table);

    ///Initializing array of pointer to structs
    for (i = 0; i < 256; i++) {
        psym[i] = &table[i];
    }
    root = buildTree(psym, N);
    makeCodes(root);

    //2 Work with code file
    FILE* fp101 = fopen("101.txt", "w+");
    if (!fp101) {
        printf("Error! File 101.txt isn'i exist!\n");
        return 1;
    }
    //This function is just for check if symbol code looks properly
    smartPrintTable(table);
    //Before useing of fp go back to begining of file
    fseek(fp, 0L, SEEK_SET);
    countOfUniqueSymbols = searchUniqueSymbols(table);
    printf("UNIQUE IS %d\n", countOfUniqueSymbols);
    makeCodeFile(table, fp, fp101);
    tail = makeCodeFile(table, fp, fp101);

    //3 Work with final file
    outFileName = (char*)malloc(sizeof(char) * (strlen(fileName) + 4));
    strcpy(outFileName, fileName);
    strcat(outFileName, ".arc");
    FILE* fpResult = fopen(outFileName, "wb");
    if (!fpResult) {
        printf("Error! File %s couldn't be opened!", outFileName);
        return 1;
    }
    fileExtension = getFileExtension(fileName);
    makeHeader(fp, fpResult, sig, countOfUniqueSymbols, table, tail, lenResult, fileExtension);
    //Before reading of fp101 go back to begining of file
    fseek(fp101, 0L, SEEK_SET);
    while (fgets(buf, 8, fp101)) {
        packedCh = pack((unsigned char*)buf);
        fwrite(&packedCh, sizeof(unsigned char), 1, fpResult);
    }

    //Closeing all used files
    fclose(fp);
    fclose(fp101);
    fclose(fpResult);

    return 0;
}