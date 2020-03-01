struct SYM // symbol
{
    unsigned char ch; // ASCII-code
    float freq; // frequency
    char code[256]; // array for a new code
    struct SYM* left; // the left child in the tree
    struct SYM* right; // the right child in the tree
};
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
typedef struct SYM TSYM;
typedef TSYM* PSYM;
//functions for analyzer:
//reads the name of a source file and finds last point before extension
int findPoint(char** argv); 
// gets the name of a source file
void getName(char** argv, char* name, int point);
//gets the extension of a source file
void getExtension(char** argv, char* extension, int point);
/*writes every simbol from the file to array "syms", 
counts the number of every symbol, 
returns total quantity of symbols in the file
*/
int read(FILE* fp, SYM* syms); 
//if the file is empty prints the message and returns 1. Else returns 0.
int checkIfEmpty(int sum);
//fills the frequency in the array "syms"
void countFreq(int sum, SYM* syms); 
//compares the qrequency for soting of the array "syms"
int cmp(const void* a, const void* b); 
//fills the array of pointers from the array "syms" with items which frequency > 0, returns quantity of unique symbols
int fillp(SYM* syms, SYM** psyms);
//if the file contains only one symbol, makes compressed file which contains only header
void packIfOne(char ch, int count, char* name, char* extension);
//builds the Huffman's tree
struct SYM* buildTree(struct SYM* psyms[], int count);
//code generator, makes code for every symbol in the tree
void makeCodes(struct SYM* root); 
//reads chars from the source file and writes the code of every symbol to 101_file. Returns total length of the 101 code.
int coder(FILE* fp_in, SYM* syms, int count); 
//writes header to cpmpressed file
void writeHeader(FILE* fp_compressed, int* count, SYM* syms, int* tail, int* sum, char* extension);
//packs one byte
unsigned char pack(unsigned char buf[]);
//takes 101 file, devides into bytes, transform them to symbols and makes compressed file
void bytepacker(FILE* fp_compressed);
//DECOMPRESSOR
//make the name for decompressed file. writes "1" to make the difference from the source file, writes "." to add extension then.
void getDecName(char** argv, char* name, int point);
//If the source file contained only one symbol, restore the source file. 
void unpackIfOne(FILE* fp_compressed, char* name);
// takes symbol from compressed file, unpack code, write code to decodes.txt
void makeDecodes(FILE* fp_compressed);
//converts one symbol to code from compressed file to 101 file
void unpack(unsigned char ch, unsigned char buf[]);
// finds char in the tree according to the code from 101 file
char decoder(FILE* fp_decodes, PSYM root);
