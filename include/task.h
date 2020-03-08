struct SYM // представление символа
{
	unsigned char ch; // ASCII код
	float freq; // частота
	char code[256]; // массив для нового кода
	struct SYM* left; // левый потомок в дереве
	struct SYM* right; // правый потомок в дереве
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
// функции для анализатора : читает имя исходного файла и находит последнюю точку перед расширением
int findPoint(char** argv);
// возвращает имя исходного файла
void getName(char** argv, char* name, int point);
//получает расширение исходного файла
void getExtension(char** argv, char* extension, int point);
// записывает каждый символ из файла в массив "syms", подсчитывает количество каждого символа, возвращает общее количество символов в файле
int read(FILE* fp, SYM* syms);
//если файл пуст, выводит сообщение и возвращает значение 1. в противном случае возвращает 0.
int IfEmpty(int sum);
// заполняет частоту в массиве "syms"
void countFreq(int sum, SYM* syms);
// сравнивает частоту для сортировки массива "syms"
int cmp(const void* a, const void* b);
//заполняет массив указателей из массива "syms" элементами, частота которых > 0, возвращает количество уникальных символов
int fillinginpointers(SYM* syms, SYM** psyms);
//если файл содержит только один символ, делает сжатый файл, который содержит только заголовок
void kompressor(char ch, int count, char* name, char* extension);
//строит дерево Хаффмана
struct SYM* buildTree(struct SYM* psyms[], int count);
// генератор кода, делает код для каждого символа в дереве
void makeCodes(struct SYM* root);
//считывает символы из исходного файла и записывает код каждого символа в 101_file. Возвращает общую длину кода 101.
int coder(FILE* fp_in, SYM* syms, int count);
// запись заголовка в сжатый файл
void writeHeader(FILE* fp_compressed, int* count, SYM* syms, int* tail, int* sum, char* extension);
//пакует один байт
unsigned char pack(unsigned char buf[]);
// берет 101 файл, разбивает на байты, преобразует их в символы и создает сжатый файл
void packer(FILE* fp_compressed);
///ДЕКОМПРЕССОР
//создайте имя для распакованного файла. пишем "1", чтобы сделать отличие от исходного файла, пишем "." для добавления расширения.
void newName(char** argv, char* name, int point);
//Если исходный файл содержит только один символ, восстановите исходный файл.
void unkompressor(FILE* fp_compressed, char* name);
// берет символ из сжатого файла, распаковывает код, записывает код в decodes.txt
void makeDecodes(FILE* fp_compressed);
// преобразует один символ в код из сжатого файла в 101 файл
void unpack(unsigned char ch, unsigned char buf[]);
// находит символ в дереве Хаффмана в соответствии с кодом из файла 101
char decoder(FILE* fp_decodes, PSYM root);