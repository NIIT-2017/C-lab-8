#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "task1.h"
#define M 256

int main(int argc, char* argv[])
{
	FILE *fp1; 
	SYM syms[M] = { 0 }; 
	PSYM psym[M] = { 0 };
	unsigned int count = 0; 
	int uniqueCount = 0; 
	int tail = 0; 
	unsigned char ch = 0;
	unsigned char buf[8] = { 0 };
	unsigned char temp = 0;
	unsigned char num = 0;
	char arr[10] = { 0 };
	
	if (argc < 2)
	{
		printf("You did not enter file name");
		exit(EXIT_FAILURE);
	}
	
	if ((fp1 = fopen(argv[1], "rb")) == NULL)
	{
		puts("File not found!");
		exit(EXIT_FAILURE);
	}
	
	fread(arr, 3, 1, fp1);
	if (strcmp("Compressed", arr) != 0)
	{
		printf("File cannot be unpacked!");
		exit(EXIT_FAILURE);
	}
	fread(&num, 1, 1, fp1);
	uniqueCount = num + 1;
	for (int i = 0; i < uniqueCount; i++)
	{
		fread(&syms[i].ch, 1, 1, fp1);
		fread(&syms[i].freq, 4, 1, fp1);
	}
	fread(&tail, 4, 1, fp1);
	fread(arr, 3, 1, fp1);
	arr[3] = '\0';

	for (int i = 0; i < uniqueCount; i++)
	{
		psym[i] = &syms[i];
	}

	PSYM root = makeTree(psym, uniqueCount);                  
	makeCodes(root);
	
	FILE *fp2;
	fp2 = fopen("101.txt", "wb");
	if (fp2 == NULL)
	{
		puts("File not found!");
		exit(EXIT_FAILURE);
	}

	while (fread(&temp, 1, 1, fp1))
	{
		uncompress(buf, temp);
		fwrite(buf, 8, 1, fp2);
	}
		fseek(fp2, 0, SEEK_END);
	int size = 0;
	size = ftell(fp2);

	fclose(fp2);
	char name[20] = { 0 };

	for (int i = 0; argv[1][i] != '\0'; i++)
	{
		name[i] = argv[1][i];
		if (name[i] == '.')
		{
			name[i + 1] = '\0';
			break;
		}
	}
	strcat(name, arr);

	decodes("101.txt", name, root, size); 
	fclose(fp1);
	

	return 0;