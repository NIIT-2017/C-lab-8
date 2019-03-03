#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "source.h"



int main (int argc, char * argv[]){
    
    //1-й Этап: запускаем анлизатор файла
    struct SYM syms[256]; // массив из 256 элементов (по элементу на каждый символ ASCII)
    struct SYM * root = NULL; // указатель на корень
    unsigned char biArr[8]; // массив под бинарную запись сжатого символа
    char * fileName; // массив под имя файла (значение передается из командной строки при открытии)
    char * fileExtension = { 0 }; // расширение файла
    if (argc < 1){
        printf("Error! Restart programm and enter name of file!\n");
        return 1;
    } else {
            fileName = argv[1];
    }
    /*Тумблер для выбора режима работы компрессор/декомпресор*/
    int regim = 0;
    while(regim == 0){
        printf("What do you want? If you want to pack you file enter '1' or '2' if you want to unpack your file!\n");
        scanf("%d", &regim);
    }
    
    if((regim != 1) && (regim != 2))
        printf("WRONG CHOICE! TRY AGAIN! ENTER '1' OR '2'!\n");
    
    if (regim == 1){ // запускаем компрессор
        fillBuf(syms, fileName);
        qsort(syms, 256, sizeof(struct SYM), compareByFreq);
        root = createTree(syms);
    //находим расширение сжимаего файла
    fileExtension = Extension(fileName, fileExtension);
    
    //2-й Этап: считаем количество уникальных символов и общее количество символов в тексте
    int totalCount = 0; // общее количество символов
    int uniqueCharCount = 0; // количество уникальных символов в тексте
    uniqueCharCount = uniqueCharCounter(syms, totalCount, uniqueCharCount)[0];
    totalCount = uniqueCharCounter(syms, totalCount, uniqueCharCount)[1];
    
    //3-й Этап: генератор кода для каждого обнаруженного символа в файле
    struct SYM * psyms[512]; // массив указателей на структуры типа SYM (для дерева генератора кода)
    for(int i = 0; i<uniqueCharCount; i++){
        psyms[i]=&syms[i];
    };
    
    root = buildTree(psyms, uniqueCharCount);
    makeCodes(root);
    int tail = fillCodeFile(fileName, "101", syms, uniqueCharCount);
    //4-й этап: получение нового сжатого файла
    compressFile(fileName, "compressed", "101", uniqueCharCount, fileExtension, syms, tail, biArr);
    }
    if(regim == 2){ // запускаем декомпресор
        struct SYM decodeSyms [256] = { 0 }; // массив из 256 символов для декодированного файла
        struct SYM *pdecodedSyms [512] = { 0 }; // массив указателей на структуры для нового дерева (аналогично struct SYM * psyms)
        int decUniqCharcount = 0; // количество уникальных символов
        int decodeTail = 0; // длина хвоста
        long sizeOfOriginFile = 0; // размер оригинального файла
        char originFileExtention [4] = {'\0'}; // расширение оригинального файла
        char decodedFileName[20] = { '\0' }; // имя декодированного файла (аналогичного 101 файлу)
        decompressFile(fileName, &decUniqCharcount, decodeSyms, decodeTail, sizeOfOriginFile, originFileExtention, decodedFileName);
        char nameOfUnPackFile [20] = {'\0'};
        
        for(int i = 0; i<decUniqCharcount; i++){
            pdecodedSyms[i] = &decodeSyms[i];
        }
        
        struct SYM * decodedRoot = NULL;
        decodedRoot = buildTree(pdecodedSyms, decUniqCharcount);
        makeCodes(decodedRoot);
        getUnPackFile("DECODED", originFileExtention, nameOfUnPackFile, decUniqCharcount, decodeSyms);
        
    }
    
    return 0;
}

