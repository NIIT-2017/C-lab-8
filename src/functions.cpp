#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"

int findPoint(char** argv)
{
    int point = 0;
    for (int i = 0; argv[2][i] != '\0'; i++)
    {
        if (argv[2][i] == '.')
            point = i;
    }
    return point;
}
void getName(char** argv, char* name,int point)
{
    for (int i = 0; i < point; i++)
    {
        name[i] = argv[2][i];
        name[point] = '\0';
    }
}
void getExtension(char** argv, char* extension, int point)
{
    for (int i = point + 1, j = 0; i < argv[2][i] != '\0'; i++, j++)
        extension[j] = argv[2][i];
}
int read(FILE* fp, SYM *syms)
{
    int ch = 0;
    int sum = 0;
    int i = 0;
    while (((ch = fgetc(fp)) != EOF))
    {
        syms[ch].ch = ch;
        syms[ch].freq += 1;
        sum += 1;
    }
    return sum;
}
int checkIfEmpty(int sum)
{
    if (sum == 0)
    {
        printf("This file is empty. It's impossible to compress");
        return 1;
    }
    else
        return 0;
}
void countFreq(int sum, SYM* syms)
{
    for (int i = 0; i < 256; i++)
    {
        syms[i].freq = syms[i].freq /sum;
    }

}
int cmp(const void* a, const void* b)
{
    if (((struct SYM*)a)->freq > ((struct SYM*)b)->freq)
        return -1;
    else
        return 1;
}
int fillp(SYM* syms, SYM** psyms)
{
    int count = 0;
    for (int i = 0; (syms[i].freq - 0) > 0.00000001f; i++)
    {
        psyms[i] = &syms[i];
        count++;
    }
    return count;
}
void packIfOne(char ch, int number, char* name, char* extension)
{
    FILE* fp_compressed = fopen(strcat(name, ".kom"), "wb"); // compressed file
    if (!fp_compressed)
    {
        perror("File:");
        exit(1);
    }
    int count = 1;
    fwrite("kom", 1, 3, fp_compressed); // signature
    fwrite(&count, 4, 1, fp_compressed); // quantity of unique symbols
    fwrite(&ch, 1, 1, fp_compressed); // symbol
    fwrite(&number, 4, 1, fp_compressed); // quantity of repetitions of the symbol
    for (int i = 0; i < 8; i++)
        fwrite(&extension[i], 1, 1, fp_compressed); // extension of the source file
    printf("The file is compressed successfully!");
}
struct SYM* buildTree(struct SYM* psyms[], int count)
{
    // создаЄм временный узел
    struct SYM* temp = (struct SYM*)malloc(sizeof(struct SYM));
    // в поле частоты записываетс€ сумма частот последнего и предпоследнего элементов массива psym
    temp->freq = psyms[count - 2]->freq + psyms[count - 1]->freq;
    // св€зываем созданный узел с двум€ последними узлами
    temp->left = psyms[count - 1];
    temp->right = psyms[count - 2];
    temp->code[0] = 0;
    if (count == 2) // мы сформировали корневой элемент с частотой 1.0
        return temp;
        // добавл€ем temp в нужную позицию psyms, сохран€€ пор€док убывани€ частоты
    int position = count - 3;
    //отбрасываем две позиции с использованными частотами
    //пока позици€ больше нул€ и ее частота меньше рассчитанной,сдвигаем ее вправо. объ€вл€ем новую позицию на шаг левее
    while (position >= 0 && (psyms[position]->freq <= temp->freq))
    {
        psyms[position + 1] = psyms[position];
        position--;
    }
    psyms[position + 1] = temp;
    return buildTree(psyms, count - 1);
}
void makeCodes(struct SYM* root)
{
    if (root->left)
    {
        strcpy(root->left->code, root->code);
        strcat(root->left->code, "0");
        makeCodes(root->left);
    }
    if (root->right)
    {
        strcpy(root->right->code, root->code);
        strcat(root->right->code, "1");
        makeCodes(root->right);
    }
}
int coder(FILE* fp_in, SYM* syms, int count)
{
    int size = 0;
    int ch; 
    FILE* fp_101 = fopen("codes.txt", "wb");
    if (!fp_101)
    {
        perror("File:");
        exit(1);
    }
    while ((ch = fgetc(fp_in)) != EOF)
    {
        for (int i = 0; i < count; i++)
            if (syms[i].ch == (unsigned char)ch) 
            {
                fputs(syms[i].code, fp_101); 
                size += strlen(syms[i].code);
                break; 
            }
    }
    fclose(fp_101);
    return size;
}
void writeHeader(FILE* fp_compressed, int* count, SYM* syms, int* tail, int* sum, char* extension)
{
    fwrite("kom", 1, 3, fp_compressed); // сигнатура
    fwrite(count, 4, 1, fp_compressed); // количество уникальных символов
    for (int i = 0; i < *count; i++)
    {
        fwrite(&syms[i].ch, 1, 1, fp_compressed); // символ
        fwrite(&syms[i].freq, 4, 1, fp_compressed); // частота
    }
    fwrite(tail, 1, 1, fp_compressed); // длина хвоста
    fwrite(sum, 4, 1, fp_compressed); // размер исходного файла
    for (int i = 0; i < 8; i++)
    {
        fwrite(&extension[i], 1, 1, fp_compressed); // расширение
    }

}
unsigned char pack(unsigned char buf[])
{
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
void bytepacker(FILE* fp_compressed) 
{
    int temp = 0;
    FILE* fp_codes = fopen("codes.txt", "rb"); // 101 file
    if (!fp_codes)
    {
        perror("File:");
        exit(1);
    }
    unsigned char arr[8] = { 0 };
    int ch = 0;
    int i = 0;
    while ((ch = fgetc(fp_codes)) != EOF)
    {
        arr[i] = ch;
        if (i == 7)
        {
            temp = pack(arr);
            fwrite(&temp, 1, 1, fp_compressed);
            i = 0;
        }
        else
            i++;
    }
    fclose(fp_codes);
    while (i < 8)
    {
        arr[i] = '0';
        i++;
    }
    pack(arr);
    temp = pack(arr);
    fwrite(&temp, 1, 1, fp_compressed);
}
void getDecName(char** argv, char* name, int point)
{
    for (int i = 0; i < point; i++)
        name[i] = argv[2][i];
    name[point] = '1';
    name[point + 1] = '.';
    name[point + 2] = '\0';
}
void unpackIfOne(FILE* fp_compressed, char *name)
{
    int ch = 0;
    int number = 0;
    fread(&ch, 1, 1, fp_compressed); // symbol
    fread(&number, 4, 1, fp_compressed); // quantity
    char extension[8] = { 0 };// 
    for (int i = 0; i < 8; i++)
    {
        fread(&extension[i], 1, 1, fp_compressed);
    }
    FILE* fp_decompressed = fopen(strcat(name, extension), "wb"); 
    if (!fp_decompressed)
    {
        perror("File:");
        exit(1);
    }
    for (int i = 0; i < number; i++)
        fputc(ch, fp_decompressed);
    printf("the file is decompressed successfully!");
}
void unpack(unsigned char ch, unsigned char buf[]) 
{
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
void makeDecodes(FILE* fp_compressed)
{
    FILE* fp_101 = fopen("decodes.txt", "wb"); // 101 file
    if (!fp_101)
    {
        perror("File:");
        exit(1);
    }
    int ch = 0;
    unsigned char arr[8] = { 0 };
    while ((ch = fgetc(fp_compressed)) != EOF)
    {
        unpack(ch, arr);
        for (int i = 0; i < 8; i++)
            fputc(arr[i], fp_101);
    }
    fclose(fp_101);
}
char decoder(FILE* fp_decodes, PSYM root)
{
    PSYM temp = root;
    char ch = 0;
    if (temp->left == NULL && temp->right == NULL)
        return temp->ch;
    ch = fgetc(fp_decodes);
    if (ch == '0')
        return decoder(fp_decodes, temp->left);
    else if (ch == '1')
        return decoder(fp_decodes, temp->right);
}

