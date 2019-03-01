#include "HaffTree.h"
#include "Packing.h"
#include "Unpack.h"
#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char * argv[])
{
	if (argc < 2)
	{
		puts("Wrong format");
		return -7;
	}
	int choice = 0;
	while ((choice != 1) && (choice != 2)) {
		printf("If you want to compress your file, enter 1\n");
		printf("If you want to decompress your file, enter 2\n");
		scanf("%d", &choice);

		if ((choice != 1) && (choice != 2))
			printf("Wrong choice, try again\n");
		else if (1 == choice)
		{
			printf("\n\nBEGIN PACKING!!!\n");//just notification
			printf("The name of compressed file is %s\n", argv[1]);
			FILE * fp_origin = fopen(argv[1], "rb");//checking file for reading
			if (!fp_origin)
			{
				puts("Cannot read original file");
				exit(-1);
			}
			getc(fp_origin);
			fseek(fp_origin, 0, SEEK_END); // moving to the end of the compressed file
			long sizeOfOriginalFile = ftell(fp_origin);  //finding size of original file for header
			fclose(fp_origin);				//closing initial file

			SYM syms[SIZE1] = { 0 };		//creating array of structures
			int countUniqSymb = 0;			//total number of unique symbols in the document 

			MakingTableOfFrequencies(syms, argv[1], &countUniqSymb);
			SortingArrByFrequencies(syms, countUniqSymb);

			//building Haffman tree
			SYM *psym[SIZE1] = { NULL };
			for (int i = 0; i < countUniqSymb; i++) //filling array of pointers with addresses of existing symbols
				psym[i] = &syms[i];

			SYM *root = BuildTree(psym, countUniqSymb);
			MakeCodes(root);
			MakeCodedFile(argv[1], "coded.txt", countUniqSymb, syms);

			unsigned char tail = 0;
			int quantityOfLoopsForCompressing = 0;
			tail = CountingLengthOfTail("coded.txt", &quantityOfLoopsForCompressing);//this func returns length of tail 

			MakeCompressedFile("coded.txt", "result.hfc", quantityOfLoopsForCompressing, tail, countUniqSymb, syms, argv[1], sizeOfOriginalFile);
			printf("Compression was made successfully!\n");
			printf("The name of decompressed file is result.hfc\n");
			printf("If you want to decompress this file, start program again,");
			printf("\nusing the name of compressed file as an argument of the command line\n");
		}
		else if (2 == choice)
		{
			//unpacking part!!!*******************************************************************
			printf("\n\nBEGIN UNPACKING!!!\n");//just notification
			printf("The name of decompressed file is %s\n", argv[1]);
			SYM newSyms[SIZE1] = { 0 };		//creating array of structures for unpacking
			int newCountUniqSymb = 0;//total number of unique symbols in the document (we will read it from header) 
			unsigned char newTail = 0;//the length of tail
			SYM *pnewSym[SIZE1] = { NULL };//creating array of pointers to the structures for unpacking
			unsigned char newFileExtention[5] = { 0 };
			long newSizeOfOriginalFile = 0;
			MakeDecompressedFile(newSyms, argv[1], &newCountUniqSymb, &newTail, newFileExtention, &newSizeOfOriginalFile);
			for (int i = 0; i < newCountUniqSymb; i++) //filling array of pointers with addresses of existing symbols
				pnewSym[i] = &newSyms[i];

			SYM *newRoot = BuildTree(pnewSym, newCountUniqSymb);
			MakeCodes(newRoot);
			unsigned char nameOfDecodedFile[256] = { 0 };
			MakeDecodedFile("decompressed.txt", newCountUniqSymb, newSyms, newFileExtention, nameOfDecodedFile);

			//checking of size
			FILE * fp_res = fopen(nameOfDecodedFile, "rb");//checking file for reading
			if (!fp_res)
			{
				puts("Cannot read original file");
				exit(-1);
			}
			fseek(fp_res, 0, SEEK_END); // moving to the end of the decompressed file
			long sizeForCheck = ftell(fp_res);
			fclose(fp_res);

			if (newSizeOfOriginalFile != sizeForCheck)
				printf("ERROR - sizes of compressed file and decompressed file are different!\n");
			else {
				printf("Decompression was made successfully!\n");
				printf("The name of decompressed file is %s\n", nameOfDecodedFile);
			}
		}
	}
	return 0;
}