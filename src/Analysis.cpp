#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "head1.h"

void initializeTable(SYM* table, char* str) {
    for (int i = 0; i < 256; i++) {
        table[i].ch = i;
        table[i].freq = 0;
        table[i].code[0] = 0;
        table[i].left = NULL;
        table[i].right = NULL;
    }
}

void makeTable(SYM* table, char* str, float total) {
    float oneSymbolFreq = 1 / total;
    while (*str) {
        total++;
        table[*str].freq += oneSymbolFreq;
        str++;
    }
}

void printTable(SYM* table) {
    for (int i = 0; i < 256; i++) {
        printf("Symbol - %c, freq - %3f \n", table[i].ch, table[i].freq);
    }
}

//This function is just for check if symbol code looks properly
void smartPrintTable(SYM* table) {
    for (int i = 0; i < 256; i++) {
        if (table[i].freq == 0.0) {
            break;
        }
        printf("Symbol - '%c', freq - %3f; code: '%s'\n", table[i].ch, table[i].freq, table[i].code);
    }
}

int cmp(const void* a, const void* b) {                    
    if (((struct SYM*)a)->freq > ((struct SYM*)b)->freq) {
        return -1;
    }
    if (((struct SYM*)a)->freq == ((struct SYM*)b)->freq) {
        return 0;
    }
    if (((struct SYM*)a)->freq > ((struct SYM*)b)->freq) {
        return 1;
    }
}

int searchUniqueSymbols(SYM* table) {
    int count = 0;
    int i = 0;
    for (int i = 0; i < 256; i++) {
        if (table[i].freq == 0.0) {
           
        }
        else {
         count++;
        }
    }
    return count;
}