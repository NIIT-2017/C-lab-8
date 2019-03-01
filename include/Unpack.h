#ifndef UNPACK_H
#define UNPACK_H


void MakeDecompressedFile(SYM * newSyms, const char * fileName, int * pcountUniqSymb, unsigned char * pnewTail,
							unsigned char newFileExtention[5], long * pnewSizeOfOriginalFile);
unsigned char * Unpack(unsigned char codedSymb, char newBuf[8]);
void MakeDecodedFile(const char * fileDecompr, int newCountUniqSymb, SYM * newSyms,
	unsigned char * newFileExtention, unsigned char * nameOfDecodedFile);
#endif