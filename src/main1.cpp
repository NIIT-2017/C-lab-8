#include <stdio.h>
#include <stdlib.h>
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
    char* fileExtension = "txt";

    //compress.exe toRead.txt;
    //Work with files
    //1 Work with sourse file
    FILE* fp = fopen("toRead.txt", "rb");
    if (!fp) {
        printf("Error!\n");
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
        chomp(text);
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
        printf("Error! fp1.");
        return 1;
    }
    printf("Smart code table\n");
    smartPrintTable(table);
    //Before useing of fp go back to begining of file
    fseek(fp, 0L, SEEK_SET);
    countOfUniqueSymbols = searchUniqueSymbols(table);
    printf("UNIQUE IS %d\n", countOfUniqueSymbols);
    makeCodeFile(table, fp, fp101);
    tail = makeCodeFile(table, fp, fp101);
        //fprintf(fp1, "%s\n", table->code);
   
    //Work with final file
    FILE* fpResult = fopen("Result.txt", "wb");
    if (!fpResult) {
        printf("Error! Result");
        return 1;
    }
    makeHeader(fp, fpResult, sig, countOfUniqueSymbols, table, tail, lenResult, fileExtension);
    //Before reading of fp101 go back to begining of file
    fseek(fp101, 0L, SEEK_SET);
    while (fgets(buf, 8, fp101)) {
        packedCh = pack((unsigned char*)buf);
        fwrite(&packedCh, sizeof(unsigned char), 1, fpResult);
    }
    fclose(fp);
    fclose(fp101);
    fclose(fpResult);
   
    return 0;
}