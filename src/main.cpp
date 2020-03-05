#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"task.h"

int main(int agrc, char** argv)
{
	struct SYM syms[SIZE];
	struct SYM* psysms[SIZE];
	struct SYM* root;
	FILE* fp_in;
	FILE* fp_101;
	FILE* fp_finish;  //The packed file
	FILE* fp_FENIX; //The unpacked file
	int N; //count type simbols
	int sum = 0; //sum ch in file, with '\r' 
	int* p = &sum;

	N = Analyzer(argv[1], syms, psysms, p);
	root = buildTree(psysms, N);
	makeCodes(root);
	//printTree(root, 0); //if the user will want to see a tree	

	fp_in = fopen(argv[1], "rb");         //for Coder
	if (!fp_in)
	{
		perror("File :");
		exit(1);
	}
	fp_101 = fopen("101.txt", "wb");
	if (!fp_101)
	{
		perror("File :");
		exit(1);
	}
	CoderLine(&syms[0], N, fp_in, fp_101);  //Coder
	if (fclose(fp_in) != 0)
		puts("Error closing the file!");
	if (fclose(fp_101) != 0)
		puts("Error closing the file!");  //for Coder


	fp_101 = fopen("101.txt", "rb");  // for makeFirstLine
	if (!fp_101)
	{
		perror("File :");
		exit(1);
	}
	fp_finish = fopen("finish.txt", "ab");  //"ab" for make a file
	if (!fp_finish)
	{
		perror("File :");
		exit(1);
	}
	int simbol_format = makeFirstLine(fp_101, fp_finish, N, syms, sum, argv[1]);  //makeFirstLine
	if (fclose(fp_finish) != 0)
		puts("Error closing the file!");
	if (fclose(fp_101) != 0)
		puts("Error closing the file!");  // for makeFirstLine


	fp_101 = fopen("101.txt", "rb");      //for Packing
	if (!fp_101)
	{
		perror("File :");
		exit(1);
	}
	fp_finish = fopen("finish.txt", "ab");  //"ab" for make a file
	if (!fp_finish)
	{
		perror("File :");
		exit(1);
	}
	Packing(fp_101, fp_finish, N, syms, sum, argv[1]);  // Packing
	if (fclose(fp_finish) != 0)
		puts("Error closing the file!");
	if (fclose(fp_101) != 0)
		puts("Error closing the file!");  //for Packing


	//    DE - COMPRESSOR   

	//for UnPacking
	fp_finish = fopen("finish.txt", "rb");  //"rb" for only read a file
	if (!fp_finish)
	{
		perror("File :");
		exit(1);
	}
	fp_FENIX = fopen("FENIX.txt", "ab");  //"ab" for make a file
	if (!fp_FENIX)
	{
		perror("File :");
		exit(1);
	}

	UnPacking(fp_finish, fp_FENIX, simbol_format);   //UnPacking

	if (fclose(fp_finish) != 0)
		puts("Error closing the file!");
	if (fclose(fp_FENIX) != 0)
		puts("Error closing the file!");    //for UnPacking


	return 0;
}