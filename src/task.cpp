#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"

struct SYM *buildTree(struct SYM *psym[],int N)                  // рeкурсивная функция создания дерева Хаффмана
{
    struct SYM *temp=(struct SYM*)malloc(sizeof(struct SYM));    // создаём временный узел
    temp->freq=psym[N-2]->freq+psym[N-1]->freq;                  // в поле частоты записывается сумма частот двух последних элементов массива psym
    temp->left=psym[N-1];                                        // связываем созданный узел с двумя последними узлами
    temp->right=psym[N-2];
    temp->code[0]=0;
    if(N==2)                                                     // сформировали коневой элемент дерева с частотой 1.0
        return temp;
    else                                                         // внесение temp в массив psym в нужное место элемента дерева Хаффмана
    {                                                            // сохраняя порядок убывания частоты
        for(int i=0;i<N;i++)
            if (temp->freq>psym[i]->freq)
            {
                for(int j=N-1;j>i;j--)
                    psym[j]=psym[j-1];
                    psym[i]=temp;
                break;
            }
    }
    return buildTree(psym,N-1);
}

void makeCodes(struct SYM *root)                  // рекурсивная процедура получения префиксного кода (ф-я кодирования)
{
    if(root->left)                                // процедура обходит дерево, копируя кодовые комбинации от верхних узлов к нижним
    {
        strcpy(root->left->code,root->code);      // и добавляет 0 при левой ветви
        strcat(root->left->code,"0");
        makeCodes(root->left);
    }
    if(root->right)                               // или 1 при правой ветви
    {
        strcpy(root->right->code,root->code);
        strcat(root->right->code,"1");
        makeCodes(root->right);
    }
}

unsigned char pack(unsigned char buf[])            // ф-я упаковщик, на вход подаётся массив из 8 символов (единицы и нули)
{
    union CODE code;
    code.byte.b1=buf[0]-'0';
    code.byte.b2=buf[1]-'0';
    code.byte.b3=buf[2]-'0';
    code.byte.b4=buf[3]-'0';
    code.byte.b5=buf[4]-'0';
    code.byte.b6=buf[5]-'0';
    code.byte.b7=buf[6]-'0';
    code.byte.b8=buf[7]-'0';
    return code.ch;                                 // результатом работы является один сжатый байт
}
