#include "Header1.h"
#include <string.h>

//makePsym:
//создает массив указателей на объекты SYM хранящиеся в chart
//возвращает указатель на первый элемент массива psym
SYM** makePsym(SYM** psym, SYM* chart, int size)
{
    psym = (SYM**)malloc(size * sizeof(SYM*));
    if (psym == NULL)
    {
        printf("malloc error\n");
        return NULL;
    }

    for (int i = 0; i < size; i++)
        psym[i] = &chart[i];

    return psym;
}

//используется в qsort, сортирует элементы в psym по убыванию частоты встречаемости.
int compar2(const void* p1, const void* p2)
{
    SYM* f1 = *(SYM**)p1;
    SYM* f2 = *(SYM**)p2;

    if ((f1->frequency) > (f2->frequency)) return -1;
    if ((f1->frequency) == (f2->frequency)) return 0;
    if ((f1->frequency) < (f2->frequency)) return 1;
}

//buildTree:
//строит бинарное дерево по алгоритму Хаффмана используя psym
//возвращает указатель на корень дерева
SYM* buildTree(SYM* psym[], int N)
{
    SYM* temp = (SYM*)malloc(sizeof(SYM));
    if (temp == NULL)
    {
        printf("malloc error\n");
        return NULL;
    }
    temp->frequency = psym[N - 2]->frequency + psym[N - 1]->frequency;
    temp->left = psym[N - 1];
    temp->right = psym[N - 2];
    temp->code[0] = 0;
    if (N == 2)
        return temp;
    psym[N - 2] = temp;
    qsort(psym, N - 1, sizeof(SYM*), compar2);//сортировка psym по убыванию частоты встречаемости символов

    /*for (int i = 0; i < N - 1; i++)
    {
        printf("symbol %i = '%c', frequency = %f\n", psym[i]->symbol, psym[i]->symbol, psym[i]->frequency);
    }
    printf("***********\n");*/

    return buildTree(psym, N - 1);
}

//makeCodes:
//определяет префиксный код для каждого символа хранящегося в chart 
void makeCodes(SYM* root)
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

//write101:
//записывает в поток out перекодированные данные из потока fp в виде строкового представления бинарного кода
//возвращает размер "хвоста" 
int write101(SYM* chart, FILE* fp, FILE* out, int size)
{
    int ch;
    unsigned char c;
    int counter = 0;//количество бит которое занимает закодированный код без "хвоста"
    //int coun = 0;

    while ((ch = fgetc(fp)) != EOF)
    {
        c = (unsigned char)ch;
        for (int i = 0; i < size; i++)
        {
            if (chart[i].symbol == c)
            {
                counter += fprintf(out, "%s", chart[i].code);//запись в файл .101 строкового представления перекодированного символа и подсчет количества бит
                break;
            }
        }
        //coun++;
    }

    //printf("\nnumber of symbols = %i\n", coun);
    //printf("\nnumber of characters = %i\n", counter);
    return 8 - (counter % 8); //вычисление размера "хвоста" в битах
}

//pack:
//преобразовывает строковое представление байта в бинарное
//возвращает байт информации с помощью переменной типа unsigned char
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

//writeCode:
//записывает в поток out сигнатуру файла и перекодированную информацию из потока fp
void writeCode(FILE* fp, FILE* out, int numberOfSymbols, SYM* chart, int tail, int size, char* id, char* fileType)
{
    unsigned char buf[8] = { 0 }; //массив для временного хранения строкового представления байта
    //int flag = 1;
    int ch;
    //int counter = 0;
    //int coun = 0;
    FCODE fcode; //используется для записи переменных типа float в бинарном виде

    //fprintf(out, "%s%i", id, numberOfSymbols);
    fwrite(id, sizeof(char), strlen(id), out); //запись итендификатора файла
    fwrite(&numberOfSymbols, sizeof(int), 1, out); //запись количества уникальных символов
    /*for (int i = 0; i < numberOfSymbols; i++)
        fprintf(out, "%c%f", chart[i].symbol,chart[i].frequency);*/

    //запись каждого уникального символа и его частоты встречаемости
    for (int i = 0; i < numberOfSymbols; i++)
    {
        fprintf(out, "%c", chart[i].symbol);
        fcode.ft = chart[i].frequency;
        fprintf(out, "%c%c%c%c", fcode.quad.c1, fcode.quad.c2, fcode.quad.c3, fcode.quad.c4);
    }

    //fprintf(out, "%i%i", tail, size);
    fwrite(&tail, sizeof(int), 1, out); //запись размера "хвоста" в битах
    fwrite(&size, sizeof(int), 1, out); //запись размера исходного файла в байтах
    fprintf(out, "%c%s", strlen(fileType), fileType); //запись длины расширения файла и расширения файла

    //запись содержимого файла .101 в бинарном виде
    while (!feof(fp))
    {
        int i;
        //считывание одного байта в строковом представлении
        for (i = 0; i < 8; i++)
        {
            if ((ch = fgetc(fp)) == EOF)
            {
                //flag = 0;
                break;
            }
            buf[i] = (unsigned char)ch;
        }
        //counter += i;
        //если есть хотя бы один бит для записи формируем байт и записываем его
        if (i)
        {
            fputc(pack(buf), out);
            //coun++;
        }
    }
    //printf("\nnumber of zero//one were written = %i\n", counter);
    //printf("\nnumber of characters were written = %i\n", coun);
}