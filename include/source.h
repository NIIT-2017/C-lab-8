//структура для символа
typedef struct SYM // представление символа
{
    unsigned char ch; // ASCII-код
    int count = 0; // количество символов
    float freq = 0; // частота встречаемости
    char code[256] = { '\0' }; // массив для нового кода
    struct SYM *left = NULL; // левый потомок в дереве
    struct SYM *right = NULL; // правый потомок в дереве
} SYM;

//поля битов
union CODE {
    unsigned char ch;
    struct {
        unsigned short b1:1;
        unsigned short b2:1;
        unsigned short b3:1;
        unsigned short b4:1;
        unsigned short b5:1;
        unsigned short b6:1;
        unsigned short b7:1;
        unsigned short b8:1;
    }
    byte;
};

//функции анализатора
void fillBuf(struct SYM * buf, char *fileName);
struct SYM * addNode (struct SYM * root, struct SYM * node);
struct SYM * createTree (struct SYM buf [256]);
int compareByFreq(const void * a, const void * b); // компаратор qsort
int *uniqueCharCounter(struct SYM chArr[], int totalCount, int uniqueCharCount);
//функции анализатора кода
struct SYM* buildTree(struct SYM *psym[], int N);
void makeCodes(struct SYM *root);
int fillCodeFile (char * filename, char * file101, struct SYM * syms, int uniqueCharCount);
//функция сжимает файл и создает подпись : сигнатура, количество уникальных символов, таблица сиволов, длина хвоста, размер исходного файла
void compressFile(char *fileName, char *fileHFC, char *file101, int uniqueCharCount, char * fileExtantion, SYM * syms, int tail, unsigned char * biArr);
//поиск расширения файла
char * Extension(char  *fileName, char *fileExtension);
//упаковщик через битовые поля
unsigned char Pack(unsigned char buf[]);

/**********************
 Функции декомпрессора
 *********************/
//считывает зашифрованные символы и возвращает массив с двоичным кодом
unsigned char * unPack(unsigned char codedSymbols, char buf[8]);
//превращаем файл в нолики и единички
void decompressFile(char * filename, int *decUniqCharcount, SYM * decodeSyms, int decodeTail, long sizeOfOriginFile, char * originFileExtention, char * decodedFileName);
void getUnPackFile(char * decodedFileName, char * originFileExtention, char * nameOfUnPackFile, int decUniqCharcount, SYM * decodeSyms);
