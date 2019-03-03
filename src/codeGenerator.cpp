#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

struct SYM* buildTree(struct SYM *psym[], int M)
{	// создаём временный узел 
	struct SYM *temp = (struct SYM*)malloc(sizeof(struct SYM));
	// в поле частоты записывается сумма частот 
	// последнего и предпоследнего элементов массива psym
	temp->freq = psym[M - 2]->freq + psym[M - 1]->freq; // связываем созданный узел с двумя последними узлами 
	temp->left = psym[M - 1];
	temp->right = psym[M - 2];
	temp->code[0] = 0;
	if (M == 2) // мы сформировали корневой элемент с частотой 1.0 
		return temp;
	else
	{
		for (int i = 0; i < M; i++)
		{
			if (temp->freq>psym[i]->freq)
			{
				for (int j = M - 1; j > i; j--)
					psym[j] = psym[j - 1];
				psym[i] = temp;
				break;

			}

		}
		return buildTree(psym, M - 1);
	}

}

void makeCodes(struct SYM *root)
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

