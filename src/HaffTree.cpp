#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SYM.h"
#include "HaffTree.h"

void Sorting(SYM *psym[], int N)
{
	SYM *temp = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (psym[j]->freq <= psym[i]->freq)
			{
				temp = psym[j];
				psym[j] = psym[i];
				psym[i] = temp;
			}
		}
	}
}

SYM *buildTree(SYM *psym[], int N)
{
	SYM * temp = (SYM *)malloc(sizeof(SYM));
	temp->freq = psym[N - 2]->freq + psym[N - 1]->freq;
	temp->left = psym[N - 2];
	temp->right = psym[N - 1];
	temp->code[0] = 0;
	if (N == 2)
		return temp;
	psym[N - 1] = 0;
	psym[N - 2] = temp;
	Sorting(psym, N-1);
	return buildTree(psym, N - 1);
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