struct SYM                    //представление символа
{
	unsigned char ch;     //ASCII-код
	float freq;           //частота встречаетмости
	char code[256];       //массив дл€ нового кода
	struct SYM* left;     //левый потомок в дереве
	struct SYM* right;    //правый потомок в дереве
};

struct SYM* buildTree(struct SYM* psym[], int N);
void makeCodes(struct SYM* root);
unsigned char pack(unsigned char buf[]);

union CODE {
	unsigned char ch;           // переменна€ содержаща€ код(байт) дл€ записи в сжатый файл
	struct {                    // в каждый разр€д пол€ битов byte, наход€щегос€ внутри объединени€ CODE записать 0 или 1
		unsigned short b1 : 1;    // в зависимости от значени€ символа Т0Т или Т1Т из .101 файла
		unsigned short b2 : 1;    // то в поле ch окажетс€ байт с нужной информацией, который запишетс€ в сжатый файл
		unsigned short b3 : 1;
		unsigned short b4 : 1;
		unsigned short b5 : 1;
		unsigned short b6 : 1;
		unsigned short b7 : 1;
		unsigned short b8 : 1;
	}byte;
};
