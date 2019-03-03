#include "HaffTree.h"
#define _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


SYM * MakingTableOfFrequencies(SYM * syms, const char * fileName, int * pcountUniqSymb)
{
	FILE * fp = fopen(fileName, "rb");				//checking file for reading
	if (!fp)
	{
		puts("Cannot read original file");
		exit(-1);
	}

	int freq[256] = { 0 };							//creating array for frequencies of symbols
	int totalNumberOfSymb = 0;						//total number of symbols in the document for counting frequencies
	int chTemp = 0;
	while ((chTemp = fgetc(fp)) != EOF)//forming array with frequencies, where index is a ASCII code of the element
	{
		for (int i = 0; i < SIZE1; i++)
			if (chTemp == i)
			{
				freq[i] += 1;
				totalNumberOfSymb++;
				break;
			}
	}

	for (int i = 0; i < SIZE1; i++)//counting unique symbols
	{
		if (0 != freq[i])
			*pcountUniqSymb += 1;
	}

	for (int i = 0, j = 0; i < SIZE1; i++, j++)//counting frequencies and forming array of structures
	{
		if (freq[i])
		{
			syms[j].freq = (float)freq[i] / totalNumberOfSymb;
			syms[j].ch = i;
			syms[j].left = NULL;
			syms[j].right = NULL;
		}
		else
			j--;
	}
	fclose(fp);//closing initial file
	return syms;
}

SYM * SortingArrByFrequencies(SYM * syms, int countUniqSymb)
{
	SYM tempSym;									//sorting the array from more to less
	for (int i = 0; i < countUniqSymb; i++)
	{
		for (int j = 0; j < countUniqSymb - 1; j++)
		{
			if (syms[j].freq < syms[j + 1].freq)
			{
				tempSym = syms[j];
				syms[j] = syms[j + 1];
				syms[j + 1] = tempSym;
			}
		}
	}
	return syms;
}


struct SYM* BuildTree(struct SYM *psym[], int N)//creating  Haffman tree
{
	// creating temporary node 
	struct SYM *temp=(struct SYM*)malloc(sizeof(struct SYM)); 
	// filling freq box of the temp node with sum of the last and previous elements of psym array 
	temp->freq = psym[N - 2]->freq+psym[N - 1]->freq; 
	// connecting new node with two last nodes
	temp->left = psym[N - 1]; 
	temp->right = psym[N - 2]; 
	temp->code[0] = 0; 
	temp->ch = '\0';
	if(N==2)							//we formed root with freq 1
		return temp; 
	// adding temp to the proper place, taking into account the sorting from more to less 
	for (int i = 0; i < N; i++)
	{ 
		if (temp->freq > psym[i]->freq)	//finding psym[i] with freq.less, than temp. frequency
		{
			for (int j = N - 1; j > i; j--)
				psym[j] = psym[j - 1];	//from end till 'i' element moving array of psyms by one position to the left
			psym[i] = temp;				//putting new element to the right place
			break;
		}
	}
		return BuildTree(psym, N-1);
}

void MakeCodes(SYM *root) 
{ 
	if (root->left) 
	{ 
		strcpy(root->left->code, root->code); 
		strcat(root->left->code, "0"); 
		MakeCodes(root->left); 
	} 
	if (root->right) 
	{ 
		strcpy(root->right->code, root->code); 
		strcat(root->right->code, "1"); 
		MakeCodes(root->right); 
	} 
}
