#include"compress.h"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("wrong name of the file\n");
		return 1;
	}
	else
	{
		char pathIn[256] = { 0 };
		char pathOut[256] = { 0 };
		char expansion[5] = { 0 };
		strcpy(pathIn, argv[1]);
		path(pathIn, pathOut, expansion);
		printf("pathIn=\'%s\'\n", pathIn);

		unsigned long count = 0;
		int arr[256] = { 0 };
		struct SYM * pSYM; //pointer to dynamic array
		
		count = getArr(pathIn, arr); //total number of characters
		
		int size = arr2SYM(arr, count, &pSYM); //number of unique characters
		
		struct SYM * psym[256]; //array of pointers
			for (int i = 0; i < size; i++)
				psym[i] = &pSYM[i];

		struct SYM * root;
		root = buildTree(psym, size); 
		
		makeCodes(root);
		
		int bits2Bite = make101File(pathIn, &pSYM, size);
		
		makeOutFile(pathOut,expansion, size, &pSYM, bits2Bite);
	
		free(pSYM);
		printf("pathOut=\'%s\'\n", pathOut);
		return 0;
	}
}
