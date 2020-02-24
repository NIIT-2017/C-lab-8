#define _CRT_SECURE_NO_WARNINGS
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int NumberOfUniqueSymbols(const int*freq)
{
	int num = 0;
	for (int i = 0; i < SIZE1; i++)
	{
		if (freq[i] != 0)
		{
			//printf("%c - %u\n", i, fr[i]);
			num++;
		}
	}
	return num;
}
void Add(LIST **pphead, const DATA*val/*то что мы вставляем*/)  //добавление элемента структуры DATA в LIST и сортировка по частоте
{
	LIST*pnew = (LIST*)malloc(sizeof(LIST));//выделяем память
	while (*pphead)//адрес адреса головы списка
	{
		if (val->freq > (*pphead/*адрес текущего узла*/)->value.freq)
			//если частота добавляемого элемента больше частоты элемента текущего узла, то надо перейти к следующему узлу
			pphead = &((*pphead)->next);
		else
			break;
	}
	pnew->value = *val;
	pnew->next = *pphead;
	*pphead = pnew;
}

void PrintData(const DATA*p)
{
	printf("Freq=%i\tsymb=%c\tisSymb=%s\tleft=%x\tright=%x\tcode=%c\n",
		p->freq,
		p->isSymb ? p->symb : ' ',
		p->isSymb ? "true" : "false",
		p->left,
		p->right,
		p->code[SIZE2]
		);

}

void Print(const LIST*phead)
{
	while (phead)
	{
		PrintData(&(phead->value));
		phead = phead->next;
	}

}

//рекурсивная функция симметричного обхода дерева для печати
void PrintTree(const DATA*phead)//из этой ф-ии сделать функцию обхода для получения кода из 0 и 1
{
	if (phead)
	{
		PrintTree(phead->left);
		PrintData(phead);
		PrintTree(phead->right);
	}
}

DATA*MakeDataFromFreq(int freq, char symb) //создание одного элемента структуры DATA
{
	DATA*res = (DATA*)malloc(sizeof(DATA));
	res->freq = freq;
	res->isSymb = 1;
	res->symb = symb;
	res->code[0] = '\0';
	res->left = NULL;
	res->right = NULL;
	return res;
}

DATA*MakeDataFromNodes(/*const*/ DATA*first/*left*/,/* const */DATA*second/*right*/)
{
	DATA*res = (DATA*)malloc(sizeof(DATA));
	res->freq = first->freq + second->freq;
	res->isSymb = 0;
	res->code[0] = '\0';
	res->left = first->freq<second->freq ? first : second;
	res->right = res->left == first ? second : first;
	return res;
}

LIST*MakeListFromFreq(int freq[]) //(int *freq) //
{
	LIST*phead = NULL;
	for (int i = 0; i < SIZE1; i++)
	{
		if (freq[i])
		{
			DATA*temp = MakeDataFromFreq(freq[i], (unsigned char)i);
			Add(&phead, temp);
		}
	}
	return phead;

}

//создаем дерево из списка
DATA *MakeTreeFromList(LIST*p) //p-указатель на текущий элемент 
{
	while (p&&p->next)
	{
		DATA * temp = MakeDataFromNodes(&(p->value), &(p->next->value));
		Add(&(p/*->next->next*/), temp);
		p = p->next->next;
	}
	if (p)
		return &(p->value);
	return NULL;
}

//рекурсивная функция симметричного обхода дерева 

void MakeCodeOfSymbol(const DATA*phead)//функция обхода для получения кода из 0 и 1
{
	//if (phead&&phead->left&&phead->right)
	{
		if (phead->left)
		{
			strcpy(phead->left->code, phead->code);
			strcat(phead->left->code, "0");
			MakeCodeOfSymbol(phead->left);
		}
		if (phead->right)
		{
			strcpy(phead->right->code, phead->code);
			strcat(phead->right->code, "1");
			MakeCodeOfSymbol(phead->right);
		}
	}
}

unsigned char pack(unsigned char buf[])
{
	union CODE code;
	code.byte.b1 = buf[7] - '0';
	code.byte.b2 = buf[6] - '0';
	code.byte.b3 = buf[5] - '0';
	code.byte.b4 = buf[4] - '0';
	code.byte.b5 = buf[3] - '0';
	code.byte.b6 = buf[2] - '0';
	code.byte.b7 = buf[1] - '0';
	code.byte.b8 = buf[0] - '0';
	return code.ch;
}

int MakeTail(char *str)
{
	int len = strlen(str);
	int tail = 0;
	tail = 8 - len % 8;
	if (tail == 8)
		tail = 0;
	for (int i = 0; i<tail; i++)
		strcat(str, "0");
	return tail;
}
