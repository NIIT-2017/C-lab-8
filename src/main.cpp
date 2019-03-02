#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SYM.h"
#include "HaffTree.h"
#include "TextAnalysisBlock.h"
#include "TextPackingBlock.h"
#include "TextUnpackingBlock.h"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("wrong format\n");
		return 1;
	}
	FILE *origFile = fopen(argv[1], "r");
	if (!origFile)
	{
		printf("cannot open %s\n", argv[1]);
		return 2;
	}

	float frequenciesArray[MAX_SYM_COUNT] = { 0 };
	int SYMcount = MakeFreqsArray(origFile, frequenciesArray);
	fseek(origFile, 0, SEEK_END);
	unsigned long origSize = ftell(origFile);
	fclose(origFile);

	SYM * SYMarray = (SYM*)malloc(SYMcount * sizeof(SYM));

	FillingSYMarray(SYMarray, frequenciesArray);

	SYM *SYMpointers[MAX_SYM_COUNT] = { 0 };
	
	for (int i = 0; i < SYMcount + 1; i++)
		SYMpointers[i] = &SYMarray[i];
	//tree building
	SYM *root = buildTree(SYMpointers, SYMcount);
	//coding
	makeCodes(root);
	int tailLength = Coding(argv[1], "codedFile.txt", SYMarray, SYMcount);
	//packing
	MakePackedFile("packedtest.txt", "codedFile.txt", argv[1], origSize, SYMcount, SYMarray, tailLength);
	free(SYMarray);
	//unpacking
	char extention[5] = { 0 };
	
	SYM NewSYMarray[MAX_SYM_COUNT] = { 0 };
	SYM *NewSYMpointers[MAX_SYM_COUNT] = { 0 };

	MakeDecompressedFile(NewSYMarray, "packedtest.txt", "codedFileUP.txt", &SYMcount, &tailLength, extention, &origSize);

	for (int i = 0; i < SYMcount + 1; i++)
		NewSYMpointers[i] = &NewSYMarray[i];
	SYM * newRoot = buildTree(NewSYMpointers, SYMcount);
	makeCodes(newRoot);

	char decodedFileName[256] = "result";

	Decoding("codedFileUP.txt", SYMcount, NewSYMarray, extention, decodedFileName);

	origFile = fopen(argv[1], "r");
	if (!origFile)
	{
		printf("cannot open %s\n", argv[1]);
		return 2;
	}
	FILE *decodeFile = fopen(decodedFileName, "r");
	if (!origFile)
	{
		printf("cannot open %s\n", decodedFileName);
		return 3;
	}
	fseek(origFile, 0, SEEK_END);
	fseek(decodeFile, 0, SEEK_END);

	if (ftell(origFile) == ftell(decodeFile))
		printf("orig flie and decoded file are equal\n");
	else printf("orig flie and decoded file are NOT equal\n");

	return 0;
}