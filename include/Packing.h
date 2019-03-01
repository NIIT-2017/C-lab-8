#ifndef PACKING_H
#define PACKING_H


union CODE {
	unsigned char ch;
	struct {
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

//coding block
void MakeCodedFile(const char * fileInitial, const char * fileCoded, int countUniqSymb, SYM * syms);

//packing block
char * FindFileExtention(const char * fileInitial, unsigned char * fileExtention);
unsigned char CountingLengthOfTail(const char * fileCoded, int * pquantityOfLoopsForCompressing);
void MakeCompressedFile(const char * fileCoded, const char * fileResult, int quantOfLoopsForCompressing, 
						int tail, int countUniqSymb, SYM * syms, const char * fileInitial, long sizeOfOriginalFile);
unsigned char Pack(unsigned char buf[]);
#endif