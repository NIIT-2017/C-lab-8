#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#define SIZE 256
#define sizeOfByte 8
/* The programm takes 2 arguments. The 1st is command. The 2d is name of file  
"com" is used to compress the file. "decom" is used to decompressed the file. 
It's impossible to compress if the file is empty.
Loop is used for compression/decompression of one-symbol-file. Huffman's tree is building in other case. 
*/
int main(int argc, char* argv[])
{
    if (strcmp(argv[1], "com") == 0)
    {
        //ANALIZER
        FILE* fp = fopen(argv[2], "rb"); //source file
        if (!fp)
        {
            perror("File:");
            exit(1);
        }
        printf("compression is started...\r");
        int point = findPoint(argv);
        //point+5 - need 5 to add extension later. ".kom\0" which added in string 62
        char* name = (char*)malloc((point+5) * sizeof(char));
        getName(argv, name, point);
        char extension[8] = { 0 };
        getExtension(argv, extension, point);
        struct SYM syms[SIZE] = { 0 };
        struct SYM* psyms[SIZE] = { NULL };
        int sum = read(fp, syms); // total quantity of symbols in the file
        fclose(fp);
        if (checkIfEmpty(sum))
            return 0;
        countFreq(sum, syms);
        qsort(syms, SIZE, sizeof(struct SYM), cmp);
        int count = fillp(syms, psyms); // quantity of unique symbols
        if (count == 1)
        {
            packIfOne(psyms[0]->ch, sum, name, extension);
            return 0;
        }
        struct SYM* root = buildTree(psyms, count);
           //CODE GENERATOR
        makeCodes(root);
           //CODER
        FILE* fp_in = fopen(argv[2], "rb"); // source file
        if (!fp_in)
        {
            perror("File:");
            exit(1);
        }
        /*devides total length of 101 code into size of byte. 
        Then substract this number from size of byte.
        tail - number of added zeros to fill last byte. 
        */
        int tail = sizeOfByte - ((coder(fp_in, syms, count)) % sizeOfByte); 
        fclose(fp_in);
        //PACKER
        FILE* fp_compressed = fopen(strcat(name, ".kom"), "wb"); // compressed file
        if (!fp_compressed)
        {
            perror("File:");
            exit(1);
        }
        writeHeader(fp_compressed, &count, syms, &tail, &sum, extension);
        bytepacker(fp_compressed);
        _fcloseall();
        free(name);
        printf("The file is compressed successfully!");
    }
    else if (strcmp(argv[1], "decom") == 0)
    {
        FILE* fp_compressed = fopen(argv[2], "rb");
        if (!fp_compressed)
        {
            perror("File:");
            exit(1);
        }
        int point = findPoint(argv);
        //after point add 10 because after name write ".\0" and then extension[8]
        char* name = (char*)malloc((point+10) * sizeof(char));
        getDecName(argv, name, point);
        char sign[3] = { 0 };
        int count = 0; //quantity of unique symbols
        struct SYM syms[SIZE] = { 0 };
        int tail = 0; 
        int sum = 0; // the size of the source file
        char extension[8] = { 0 };
        //READS HEADER
        fread(sign, 1, 3, fp_compressed);
        //check sign
        if (sign[0] != 'k' || sign[1] != 'o' || sign[2] != 'm') 
        {
            printf("incorrect file!");
            return 0;
        }
        printf("decompression is started...\r");
        //if the source file consists only of one symbol, decompress without tree
        fread(&count, 4, 1, fp_compressed);
        if (count == 1)
        {
            void unpackIfOne(FILE * fp_compressed, char* name);
            return 0;
        }
        //read frequency table
        for (int i = 0; i < count; i++) 
        {
            fread(&syms[i].ch, 1, 1, fp_compressed); // symbol
            fread(&syms[i].freq, 4, 1, fp_compressed); // frequency
        }
        fread(&tail, 1, 1, fp_compressed);
        fread(&sum, 4, 1, fp_compressed);
        for (int i = 0; i < 8; i++)
        {
            fread(&extension[i], 1, 1, fp_compressed);
        }
        struct SYM* psyms[SIZE] = { NULL };
        for (int i = 0; i < count; i++)
            psyms[i] = &syms[i];
        struct SYM* root = buildTree(psyms, count);
        makeDecodes(fp_compressed);
        fclose(fp_compressed);
        FILE* fp_decodes = fopen("decodes.txt", "rb"); // 101 file
        if (!fp_decodes)
        {
            perror("File:");
            exit(1);
        }
        FILE* fp_decompressed = fopen(strcat(name, extension), "wb"); 
        if (!fp_decompressed)
        {
            perror("File:");
            exit(1);
        }
        //restore source the file
        char symbol = 0;
        for (int i = 0; i < sum; i++)
        {
            symbol = decoder(fp_decodes, root);
            fputc(symbol, fp_decompressed);
        }
        _fcloseall();
        free(name);
        printf("the file is decompressed successfully!");
    }
    else
        printf("error: no command! com/decom are available");
    return 0;
}