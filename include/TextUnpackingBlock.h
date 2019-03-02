#ifndef TEXT_UNPACKING_BLOCK
#define TEXT_UNPACKING_BLOCK

void MakeDecompressedFile(SYM * SYMarray, const char * packedFileName, const char * unpacked101File, int * SYMcount, int * tail, char * extention, unsigned long * origFileSize);
void Decoding(const char * unpacked101File, int SYMcount, SYM * SYMarray, char * extention, char * decodedFileName);

#endif