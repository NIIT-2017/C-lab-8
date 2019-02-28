#include"decompress.h"

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
		strcpy(pathOut, argv[1]);
		printf("pathOut=\'%s\'\n", pathOut);

		FILE *pf_abc;
		pf_abc = fopen(pathOut, "rb");
		if (pf_abc == NULL) { printf(" incoming (.abc) file open error\n"); exit(EXIT_FAILURE); }
		
		char buf1[4] = { 0 };
		fread(buf1, 3, 1, pf_abc);//read signature
		if (strcmp("abc", buf1) != 0)
		{printf("can't decompress this file"); exit(EXIT_FAILURE);}

		int size = 0;
		fread(&size, 1, 1, pf_abc);
		size++;//"number of unique characters +1 "
		
		struct SYM * pSYM;//pointer to dynamic array
		pSYM = (struct SYM *) malloc(size * sizeof(struct SYM));

		for (int i = 0; i < size; i++)	//read and fill fields of array
		{
			fread(&pSYM[i].ch, 1, 1, pf_abc);
			fread(&pSYM[i].freq, 4, 1, pf_abc);
			pSYM[i].code[0] = "";
			pSYM[i].left = NULL;
			pSYM[i].right = NULL;
		}
		struct SYM * psym[256]; //array of pointers
		for (int i = 0; i < size; i++)
			psym[i] = &pSYM[i];

		struct SYM * root;
		root = buildTree(psym, size);
		makeCodes(root);
		
		int bits2Bite = 0;
		fread(&bits2Bite, 1, 1, pf_abc);
		fread(expansion, 5, 1, pf_abc); 		

		FILE *pf_101;
		pf_101 = fopen("101.txt", "wb");
		if (pf_101 == NULL) { printf("file open error\n"); return -1; }

		unsigned char ch = 0;
		unsigned char buf[8] = { 0 };
		while (fread(&ch, 1, 1, pf_abc))
		{
			unPack(ch, buf);
			fwrite(buf, 8, 1, pf_101);
		}
		fseek(pf_101, -bits2Bite, SEEK_END);
		char magic = '7';
		fwrite(&magic, 1, 1, pf_101);
		fclose(pf_abc);
		fclose(pf_101); //made 101.txt
		
		
		strcpy(pathIn, pathOut); //restore path
		int length = strlen(pathIn);
		int i = length; int j = 0;
		while (pathIn[i] != '.')
			i--, j++;
		pathIn[length - j+1] = '\0';
		strcat(pathIn, expansion);
		restoreFrom101File(pathIn, root, magic);
		printf("pathIn=\'%s\'\n", pathIn);
		free(pSYM);
		return 0;
	}
}