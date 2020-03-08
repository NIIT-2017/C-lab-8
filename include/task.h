struct SYM // ������������� �������
{
	unsigned char ch; // ASCII ���
	float freq; // �������
	char code[256]; // ������ ��� ������ ����
	struct SYM* left; // ����� ������� � ������
	struct SYM* right; // ������ ������� � ������
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
// ������� ��� ����������� : ������ ��� ��������� ����� � ������� ��������� ����� ����� �����������
int findPoint(char** argv);
// ���������� ��� ��������� �����
void getName(char** argv, char* name, int point);
//�������� ���������� ��������� �����
void getExtension(char** argv, char* extension, int point);
// ���������� ������ ������ �� ����� � ������ "syms", ������������ ���������� ������� �������, ���������� ����� ���������� �������� � �����
int read(FILE* fp, SYM* syms);
//���� ���� ����, ������� ��������� � ���������� �������� 1. � ��������� ������ ���������� 0.
int IfEmpty(int sum);
// ��������� ������� � ������� "syms"
void countFreq(int sum, SYM* syms);
// ���������� ������� ��� ���������� ������� "syms"
int cmp(const void* a, const void* b);
//��������� ������ ���������� �� ������� "syms" ����������, ������� ������� > 0, ���������� ���������� ���������� ��������
int fillinginpointers(SYM* syms, SYM** psyms);
//���� ���� �������� ������ ���� ������, ������ ������ ����, ������� �������� ������ ���������
void kompressor(char ch, int count, char* name, char* extension);
//������ ������ ��������
struct SYM* buildTree(struct SYM* psyms[], int count);
// ��������� ����, ������ ��� ��� ������� ������� � ������
void makeCodes(struct SYM* root);
//��������� ������� �� ��������� ����� � ���������� ��� ������� ������� � 101_file. ���������� ����� ����� ���� 101.
int coder(FILE* fp_in, SYM* syms, int count);
// ������ ��������� � ������ ����
void writeHeader(FILE* fp_compressed, int* count, SYM* syms, int* tail, int* sum, char* extension);
//������ ���� ����
unsigned char pack(unsigned char buf[]);
// ����� 101 ����, ��������� �� �����, ����������� �� � ������� � ������� ������ ����
void packer(FILE* fp_compressed);
///������������
//�������� ��� ��� �������������� �����. ����� "1", ����� ������� ������� �� ��������� �����, ����� "." ��� ���������� ����������.
void newName(char** argv, char* name, int point);
//���� �������� ���� �������� ������ ���� ������, ������������ �������� ����.
void unkompressor(FILE* fp_compressed, char* name);
// ����� ������ �� ������� �����, ������������� ���, ���������� ��� � decodes.txt
void makeDecodes(FILE* fp_compressed);
// ����������� ���� ������ � ��� �� ������� ����� � 101 ����
void unpack(unsigned char ch, unsigned char buf[]);
// ������� ������ � ������ �������� � ������������ � ����� �� ����� 101
char decoder(FILE* fp_decodes, PSYM root);