struct SYM                    //������������� �������
{
	unsigned char ch;     //ASCII-���
	float freq;           //������� ��������������
	char code[256];       //������ ��� ������ ����
	struct SYM* left;     //����� ������� � ������
	struct SYM* right;    //������ ������� � ������
};

struct SYM* buildTree(struct SYM* psym[], int N);
void makeCodes(struct SYM* root);
unsigned char pack(unsigned char buf[]);

union CODE {
	unsigned char ch;           // ���������� ���������� ���(����) ��� ������ � ������ ����
	struct {                    // � ������ ������ ���� ����� byte, ������������ ������ ����������� CODE �������� 0 ��� 1
		unsigned short b1 : 1;    // � ����������� �� �������� ������� �0� ��� �1� �� .101 �����
		unsigned short b2 : 1;    // �� � ���� ch �������� ���� � ������ �����������, ������� ��������� � ������ ����
		unsigned short b3 : 1;
		unsigned short b4 : 1;
		unsigned short b5 : 1;
		unsigned short b6 : 1;
		unsigned short b7 : 1;
		unsigned short b8 : 1;
	}byte;
};
