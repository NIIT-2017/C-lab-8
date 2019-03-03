#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "source.h"
/****************************************************************************
 Анализатор:
 1. Считает количество каждого символа, который встретился в сжимаемом файле
 2. Считает частоту встречаемости символов
 3. Заполняет структуру SYM
 4. Создает дерево для всех символов, отсортитрованное по убыванию частоты
 ****************************************************************************/

extern int uniqueCharCount, totalcount;

//Функция считывает посимвольно данные из файла, записывает в структуру данные о символе, в т.ч. его встречаемость и общее количество
void fillBuf(struct SYM buf[256], char *fileName){
    FILE * fp;
    char ch;
    int count = 0;
    fp = fopen(fileName, "rb");
    if (!fp) {
        printf("%s", fileName);
        perror("not found!");
        exit(1);
    }
    //считываем символы из файла и считаем, сколько раз встретился каждый символ
    while((ch=fgetc(fp)) != EOF){
        buf[(int)ch].count++; // счетчик для каждого символа (необходимо использовать явное приведение типов для ch)
        count++; // общий счетчик символов
    }
    //считаем частоту для каждого символа
    for(int i = 0; i < 256; i++){
        buf[i].ch = (char)i;
        buf[i].freq = (float)(buf[i].count) / count;
    }
    fclose(fp);
    return;
}

/*
 Функция добавляет "листья" на дерево
 */
struct SYM * addNode (struct SYM * root, struct SYM node) {
    //сначала создаем корень, который примет значения первого символа
    //node - i-тый элемент массива chArr типа SYM
    if (root == NULL){
        root = (struct SYM*)malloc(sizeof(struct SYM)); // выделяем память под структуру SYM
        root->ch = node.ch;
        root->count = node.count;
        root->freq = node.freq;
        root->left = NULL;
        root->right = NULL;
    }
    //далее раскидываем листья:меньшие влево, большие вправо
    else if(root->freq > node.freq)
        root->left = addNode(root->left, node);
    else
        root->right = addNode(root->right, node);
    
    return root;
}

/*
 функция для построения дерева
 */
struct SYM * createTree (struct SYM chArr[256]){
    struct SYM * root = NULL; // указатель на корень
    //для каждого обнаруженного символа (находится в структуре buf) вызываем функцию добавления листьев addNode
    for(int i = 0; i<256; i++){
        root = addNode(root, chArr[i]);
    }
    return root;
}

/*
 Компаратор для qsort: сортируем массив структур по встречаемости символа
 */
int compareByFreq(const void *_a, const void *_b)
{
    struct SYM * pa;
    struct SYM * pb;
    pa = (struct SYM*)_a;
    pb = (struct SYM*)_b;
    if(pa->count > pb->count)
        return -1;
    else
        return 1;
}

/*
 функция считает количество уникальных символов
 */
 int *uniqueCharCounter(struct SYM chArr[], int totalCount, int uniqueCharCount){
     int counters[2] = {0};
     for(int i = 0; chArr[i].count != 0; i++){
        if(chArr[i].count!=0){
            uniqueCharCount++;
            totalCount += chArr[i].count;
        }
    }
     counters[0] = uniqueCharCount;
     counters[1] = totalCount;
     return counters;
}

/*
 функция создает дополнительные узлы в которых хранится сумма частоты встречаемости
 двух последних элементов. Для вехнего узла, т.е. корня общая частота будет равна 1.0,
 а префиксный код - '0'
 */
struct SYM* buildTree(struct SYM *psym[], int N)
{
    // создаём временный узел
    struct SYM *temp=(struct SYM*)malloc(sizeof(struct SYM));
    // в поле частоты записывается сумма частот
    // последнего и предпоследнего элементов массива psym
    temp->freq=psym[N-2]->freq+psym[N-1]->freq;
    // связываем созданный узел с двумя последними узлами
    temp->left=psym[N-1];
    temp->right=psym[N-2];
    temp->code[0]=0;
    //вставляем дополнительные узлы в дерево до тех пор,
    //пока не дойдем до двух последних элементов
    if (N == 2)
    {
        return temp; // мы сформировали корневой элемент с частотой 1.0
    }
    else
    {
        for(int i = 0; i < N; i++)
            if (temp->freq > psym[i]->freq)
            {
                for (int j = N - 1; j > i; j--)
                    psym[j] = psym[j - 1];
                psym[i] = temp;
                break;
            }
    } 
    return buildTree(psym,N-1);
}
/*
 функция формирует двоичный код для симовла
 */
void makeCodes(struct SYM *root)
{
    if(root->left)
    {
        strcpy(root->left->code,root->code);
        strcat(root->left->code,"0");
        makeCodes(root->left);
    }
    if(root->right)
    {
        strcpy(root->right->code,root->code);
        strcat(root->right->code,"1");
        makeCodes(root->right);
    }
}
/*
 функция записывает двоичный код в файл .101 и возвращает длину хвоста (остаток от деления на 8)
 */
int fillCodeFile (char * filename, char * file101, struct SYM * syms, int uniqueCharCount) {
    FILE * fp_in, *fp_out;
    unsigned char ch;
    int j;
    int tail = 0;
    int count = 0;
    fp_in = fopen(filename, "rb");
    fp_out = fopen(file101, "wb");
    while((j=fread(&ch, sizeof(unsigned char), 1, fp_in)) == 1){
        for(int i = 0; i < uniqueCharCount; i++)
            if(syms[i].ch == ch){
                fputs(syms[i].code, fp_out);
                count+=strlen(syms[i].code);
                break;
            }
    }

    fclose(fp_in);
    if(count % 8 != 0){
            tail = count % 8;
            for(int i = (8 - tail); i != 0; i--){
                fwrite("0", sizeof(char), 1, fp_out);
            }
        }
    fclose(fp_out);
    return 8 - tail;
    
}
/*
 Поиск расширения файла
 */
char * Extension(char  *fileName, char *fileExtention){
    for(int i = 0; i<=strlen(fileName); i++){
        if(fileName[i]=='.'){
             fileExtention = &fileName[i];
            break;
        }
    }
    return fileExtention;
}


/*
 Функция формирует подпись для сжатого файла
 */
void compressFile(char *fileName, char *fileHFC, char *file101, int uniqueCharCount, char * fileExtantion, SYM * syms, int tail, unsigned char * biArr){
    FILE *fp_101, *fp_out, *fp_originFile;
    
    fp_originFile = fopen(fileName, "rb");
    if(!fp_originFile){
        puts("Cannot read your file!");
        exit(1);
    }
    
    fp_out = fopen(fileHFC, "wb");
    if(!fp_out){
        puts("Cannot write coded file!");
        exit(1);
    }
    
    fp_101 = fopen("101", "rb");
    if (!fp_101)
    {
        puts("Cannot read file 101");
        exit(1);
    }
    
    
    
    //считаем размер исходного файла
    fseek( fp_originFile, 0L, SEEK_END );
    long sizeOriginFile = ftell(fp_originFile);
    fclose(fp_originFile);

    
    //подпись формата сжатого файла
    fwrite("|&|", 3*sizeof(unsigned char), 1, fp_out);
    //количество уникальных символов
    fwrite(&uniqueCharCount, sizeof(int), 1, fp_out);
    //запись символов и их встречаемости
    for(int i = 0; i<uniqueCharCount; i++){
        fwrite(&syms[i].ch, sizeof(unsigned char), 1, fp_out);
        fwrite(&syms[i].freq, sizeof(float), 1, fp_out);
    }
    //запись хвоста
    fwrite(&tail, sizeof(unsigned int), 1, fp_out);
    //запись размера исходного файла
    fwrite(&sizeOriginFile, sizeof(long), 1, fp_out);
    //запись расширения исходного файла
    fwrite(fileExtantion, strlen(fileExtantion)*sizeof(char), 1, fp_out);
    
    //сжимаем
    fseek(fp_101, 0, SEEK_SET);
    
    unsigned char chPack;
    size_t temp = 0;
    while((temp=fread(biArr, 8, 1, fp_101))){
        chPack = Pack(biArr);
        fwrite(&chPack, 1, 1, fp_out);
    }
   

    fclose(fp_out);
    fclose(fp_101);
    
    
}

/*
Функция упаковщик
*/
unsigned char Pack(unsigned char buf[])
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





