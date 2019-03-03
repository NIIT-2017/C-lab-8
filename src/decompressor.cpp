#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "source.h"

unsigned char * unPack(unsigned char codedSymbols, unsigned char buf[8])
{
    union CODE decode;
    
    decode.ch = codedSymbols;
    buf[0] = decode.byte.b1 + '0';
    buf[1] = decode.byte.b2 + '0';
    buf[2] = decode.byte.b3 + '0';
    buf[3] = decode.byte.b4 + '0';
    buf[4] = decode.byte.b5 + '0';
    buf[5] = decode.byte.b6 + '0';
    buf[6] = decode.byte.b7 + '0';
    buf[7] = decode.byte.b8 + '0';
    
    return buf;
}

void decompressFile(char * filename, int * decUniqCharcount, SYM * decodeSyms, int decodeTail, long sizeOfOriginFile, char * originFileExtention, char * decodedFileName){
    //открываем сжатый файл
    FILE *fp_compressedFile = fopen(filename, "rb");
    if(!fp_compressedFile){
        puts("Compressed file are not finded!");
        exit(0);
    }
    //находим размер сжатого файла
    fseek(fp_compressedFile, 0, SEEK_END);
    long sizeOfCompessedFile = ftell(fp_compressedFile);
    fseek(fp_compressedFile, 0, SEEK_SET);
    
    //обрабатываем заголовок файла
    char tempSignature[4] = { 0 }; // массив для разных элементов подписи
    //проверяем сигнатуру
    fread(tempSignature, 3*sizeof(char), 1, fp_compressedFile);
    if(strncmp(tempSignature, "|&|", 3) != 0){
        printf("Your file is not HAFFMAN file!\n");
        exit(0);
    }
    //считываем количество уникальных символов
    fread(decUniqCharcount, 1*sizeof(int), 1, fp_compressedFile);
    //записываем символы и их частоту в массив структур
    for(int i = 0; i<*decUniqCharcount; i++){
        fread(&decodeSyms[i].ch, 1*sizeof(unsigned char), 1, fp_compressedFile);
        fread(&decodeSyms[i].freq, 1*sizeof(float), 1, fp_compressedFile);
    }
    //считываем хвост
    fread(&decodeTail, 1*sizeof(int), 1, fp_compressedFile);
    //считываем размер исходного файла
    fread(&sizeOfOriginFile, 1*sizeof(long), 1, fp_compressedFile);
    //считываем расширения исходного файла
    fread(originFileExtention, 4*sizeof(char), 1, fp_compressedFile);
    //закончился заголовок - ищим его длину
    long signatureSize = ftell(fp_compressedFile);
    
    decodedFileName = "DECODED";
    
    //ДЕКОДИРУЕМ ЗАШИФРОВАННУЮ ЧАСТЬ
    FILE * fp_decodedFile = fopen(decodedFileName, "wb");
    
    unsigned char buf[8] = { '\0' }; // массив под 8 символов
    unsigned char codedSymbol = 0; //
    
    //главное остановиться до начала хвоста, поэтому последний символ не считываем (см. цикл for)
    for(long i = 0; i<(sizeOfCompessedFile-signatureSize-1); i++){
        codedSymbol = fgetc(fp_compressedFile);
        unPack(codedSymbol, buf);
        for(int i = 0; i < 8; i++)
            fputc(buf[i], fp_decodedFile);
    }
    //обрабатываем хвост
    codedSymbol = fgetc(fp_compressedFile);
    unPack(codedSymbol, buf);
    for(int i = 0; i < (8 - decodeTail); i++)
        fputc(buf[i], fp_decodedFile);
   
    fclose(fp_compressedFile);
    fclose(fp_decodedFile);
}

void getUnPackFile(char * decodedFileName, char * originFileExtention, char * nameOfUnPackFile, int decUniqCharcount, SYM * decodeSyms){
    //открываем декодированный двоичный файл
    FILE * fp_decodedFile = fopen(decodedFileName, "rb");
    if(!fp_decodedFile){
        puts("ERROR of decodedFile!");
        exit(0);
    }
    //сохраняем название для разорхивированного файла
    strcpy(nameOfUnPackFile, decodedFileName);
    strcat(nameOfUnPackFile, originFileExtention);
    //создаем файл для записи дешифрованных данных (наш готовый файл)
    FILE *fp_UNPACKEDFILE = fopen(nameOfUnPackFile, "wb");
    if(!fp_UNPACKEDFILE){
        puts("ERROR of UNPACKEDFILE!");
        exit(0);
    }
    
    char arrSymb[256] = { '\0' };
    int check = 0;
    while(!feof(fp_decodedFile)){
        for(int i = 0; i < 256; i++){
            arrSymb[i] = fgetc(fp_decodedFile);
            int flag = 0;
            for(int k = 0; k < decUniqCharcount; k++){
                if(strcmp(arrSymb, decodeSyms[k].code) == 0){
                    fputc((int)decodeSyms[k].ch, fp_UNPACKEDFILE);
                    check = (int)decodeSyms[k].ch;
                    for(int j = 0; j <= i; j++){
                        arrSymb[j] = '\0';
                    }
                    flag = 1;
                    break;
                } else
                    continue;
            }
            if(flag)
                break;
        }
    }
    fclose(fp_decodedFile);
    fclose(fp_UNPACKEDFILE);
}
