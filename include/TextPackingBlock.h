#ifndef TEXT_PACKING_BLOCK_H
#define TEXT_PACKING_BLOCK_H

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

int Coding(char *fp_in, char *fp_101, SYM *SYMarray, int count);
void MakePackedFile(char * packedFileName, char * fp_101Name, char *origFile, unsigned long origSize, int SYMcount, struct SYM * SYMarray, int tailLength);

#endif