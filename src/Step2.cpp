#include "Header1.h"
#include <string.h>

//создает массив указателей на объекты SYM хранящиеся в chart
//возвращает указатель на первый элемент массива psym
SYM** makePsym(SYM** psym, SYM* chart, int size)
{
    psym = (SYM**)malloc(size * sizeof(SYM*));
    if (psym == NULL)
    {
        printf("function \"makePsym\" : ");
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

//строит бинарное дерево по алгоритму Хаффмана используя psym
//возвращает указатель на корень дерева
SYM* buildTree(SYM* psym[], int N)
{
    SYM* temp = (SYM*)malloc(sizeof(SYM));
    if (temp == NULL)
    {
        printf("function \"buildTree\" : ");
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

    return buildTree(psym, N - 1);
}

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

//записывает в поток out перекодированные данные из потока fp в виде строкового представления бинарного кода
//возвращает размер "хвоста" 
int write101(SYM* chart, FILE* fp, FILE* out, int size)
{
    int ch;
    unsigned char c;
    int counter = 0;//количество бит которое занимает закодированный код без "хвоста"

    while ((ch = fgetc(fp)) != EOF)
    {
        c = (unsigned char)ch;
        for (int i = 0; i < size; i++)
        {
            if (chart[i].symbol == c)
            {
                //запись в файл .101 строкового представления перекодированного символа и подсчет количества бит
                counter += fprintf(out, "%s", chart[i].code);
            }
        }
    }

    return 8 - (counter % 8); //вычисление размера "хвоста" в битах
}

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

//записывает в поток out сигнатуру файла и перекодированную информацию из потока fp
void writeCode(FILE* fp, FILE* out, int numberOfSymbols, SYM* chart, int tail, int size, char* id, char* fileType)
{
    unsigned char buf[8] = { 0 }; //массив для временного хранения строкового представления байта
    int ch;

    fwrite(id, sizeof(char), strlen(id), out); //запись итендификатора файла
    fwrite(&numberOfSymbols, sizeof(int), 1, out); //запись количества уникальных символов

    //запись каждого уникального символа и его частоты встречаемости
    for (int i = 0; i < numberOfSymbols; i++)
    {
        fwrite(&chart[i].symbol, sizeof(char), 1, out);
        fwrite(&chart[i].frequency, sizeof(float), 1, out);
    }

    fwrite(&tail, sizeof(int), 1, out); //запись размера "хвоста" в битах
    fwrite(&size, sizeof(int), 1, out); //запись размера исходного файла в байтах
    int length = strlen(fileType);//считывание длины разрешения исходного файла в символах
    fwrite(&length, sizeof(int), 1, out);//запись длины в символах расширения исходного файла
    fwrite(fileType, sizeof(char), length, out);//запись расширения исходного файла

    //запись содержимого файла .101 в бинарном виде
    while (!feof(fp))
    {
        int i;
        //считывание одного байта в строковом представлении
        for (i = 0; i < 8; i++)
        {
            if ((ch = fgetc(fp)) == EOF)
            {
                break;
            }
            buf[i] = (unsigned char)ch;
        }
        //если есть хотя бы один бит для записи формируем байт и записываем его
        if (i)
        {
            fputc(pack(buf), out);
        }
    }
}