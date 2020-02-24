#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define SIZE1 256
#define SIZE2 10000

typedef struct data
{
	int freq;
	int isSymb;
	unsigned char symb;
	char code[SIZE2];
	struct data*left, *right;

}DATA;

typedef struct list
{
	DATA value;
	struct list *next;

}LIST;

union CODE
{
	unsigned char ch;
	struct
	{
		unsigned short b1 : 1;
		unsigned short b2 : 1;
		unsigned short b3 : 1;
		unsigned short b4 : 1;
		unsigned short b5 : 1;
		unsigned short b6 : 1;
		unsigned short b7 : 1;
		unsigned short b8 : 1;
	} byte;
};


void Add(LIST **pphead, const DATA*val/*то что мы вставляем*/);
void PrintData(const DATA*p);
void Print(const LIST*phead);
void PrintTree(const DATA*phead);
DATA*MakeDataFromFreq(int freq, char symb);
DATA*MakeDataFromNodes(/*const*/ DATA*first/*left*/,/* const */DATA*second/*right*/);
LIST*MakeListFromFreq(int freq[]); //(int *freq);
DATA *MakeTreeFromList(LIST*p);
void MakeCodeOfSymbol(const DATA*phead);
int NumberOfUniqueSymbols(const int*freq);
unsigned char pack(unsigned char buf[]);
int MakeTail(char *str);


#endif