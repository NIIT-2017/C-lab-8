#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include "task2.h"


struct SYM *buildTree(struct SYM *psym[], int N) // рeкурсивна€ функци€ реализации алгоритма ’офмана
{
	// создаЄм временный узел
	struct SYM *temp = (struct SYM*)malloc(sizeof(struct SYM));
	// в поле частоты записываетс€ сумма частот последнего и предпоследнего элементов массива psym
	temp->freq = psym[N - 1]->freq + psym[N - 2]->freq;
	// св€зываем созданный узел с двум€ последними узлами
	temp->left = psym[N - 1];
	temp->right = psym[N - 2];
	temp->code[0] = 0;

	if (N == 2)  // мы сформировали корневой элемент с частотой 1.0
		return temp;
	else // добавл€ем temp в нужную позицию psym, сохран€€ пор€док убывани€ частоты
	{
		for (int i = 0; i < N; i++)
			if (temp->freq > psym[i]->freq)
			{
				for (int j = N - 1; j > i; j--)
					psym[j] = psym[j - 1];

				psym[i] = temp;
				break;
			}
	}
	return buildTree(psym, N - 1);
}

//======================================================================================

/*
данна€ функци€ обходит дерево, копиру€ кодовые комбинации от верхних узлов нижним
и добавл€€ "0" или "1" в зависимости от левой или правой ветви
*/
void makeCodes(struct SYM *root) // рекурсивна€ функци€ получени€ префиксного кода
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

//======================================================================================
