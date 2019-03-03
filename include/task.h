struct SYM                    //представление символа
{
        unsigned char ch;     //ASCII-код
        float freq;           //частота встречаетмости
        char code[256];       //массив для нового кода
        struct SYM *left;     //левый потомок в дереве
        struct SYM *right;    //правый потомок в дереве
};

struct SYM *buildTree(struct SYM *psym[],int N);
void makeCodes(struct SYM *root);
unsigned char pack(unsigned char buf[]);

union CODE {
    unsigned char ch;           // переменная содержащая код(байт) для записи в сжатый файл
    struct {                    // в каждый разряд поля битов byte, находящегося внутри объединения CODE записать 0 или 1
        unsigned short b1:1;    // в зависимости от значения символа ’0’ или ’1’ из .101 файла
        unsigned short b2:1;    // то в поле ch окажется байт с нужной информацией, который запишется в сжатый файл
        unsigned short b3:1;
        unsigned short b4:1;
        unsigned short b5:1;
        unsigned short b6:1;
        unsigned short b7:1;
        unsigned short b8:1;
    }byte;
};
