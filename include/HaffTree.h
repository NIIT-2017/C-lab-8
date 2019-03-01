#ifndef HAFFTREE_H
#define HAFFTREE_H
#define SIZE1 256

struct SYM						//symbol's representation
{
	unsigned char ch;			//ASCII-code
	float freq;					//frequency 
	char code[SIZE1];			//array for Haffman code 
	struct SYM *left;			//left child 
	struct SYM *right;			//right child
};
typedef struct SYM SYM;

//making Table of frequencies
SYM * MakingTableOfFrequencies(SYM * syms, const char * fileName, int * pcountUniqSymb);
SYM * SortingArrByFrequencies(SYM * syms, int countUniqSymb);

//building Haffman tree
struct SYM* BuildTree(struct SYM *psym[], int N);
void MakeCodes(SYM *root);

#endif