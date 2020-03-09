#include "Header1.h"
#include <string.h>

//������� ������ ���������� �� ������� SYM ���������� � chart
//���������� ��������� �� ������ ������� ������� psym
SYM** makePsym(SYM** psym, SYM* chart, int size)
{
    psym = (SYM**)malloc(size * sizeof(SYM*));
    if (psym == NULL)
    {
        printf("function \"makePsym\" : ");
        return NULL;
    }

    for (int i = 0; i < size; i++)
        psym[i] = &chart[i];

    return psym;
}

//������������ � qsort, ��������� �������� � psym �� �������� ������� �������������.
int compar2(const void* p1, const void* p2)
{
    SYM* f1 = *(SYM**)p1;
    SYM* f2 = *(SYM**)p2;

    if ((f1->frequency) > (f2->frequency)) return -1;
    if ((f1->frequency) == (f2->frequency)) return 0;
    if ((f1->frequency) < (f2->frequency)) return 1;
}

//������ �������� ������ �� ��������� �������� ��������� psym
//���������� ��������� �� ������ ������
SYM* buildTree(SYM* psym[], int N)
{
    SYM* temp = (SYM*)malloc(sizeof(SYM));
    if (temp == NULL)
    {
        printf("function \"buildTree\" : ");
        return NULL;
    }
    temp->frequency = psym[N - 2]->frequency + psym[N - 1]->frequency;
    temp->left = psym[N - 1];
    temp->right = psym[N - 2];
    temp->code[0] = 0;
    if (N == 2)
        return temp;
    psym[N - 2] = temp;
    qsort(psym, N - 1, sizeof(SYM*), compar2);//���������� psym �� �������� ������� ������������� ��������

    return buildTree(psym, N - 1);
}

//���������� ���������� ��� ��� ������� ������� ����������� � chart 
void makeCodes(SYM* root)
{
    if (root->left)
    {
        strcpy(root->left->code, root->code);
        strcat(root->left->code, "0");
        makeCodes(root->left);
    }
    if (root->right)
    {
        strcpy(root->right->code, root->code);
        strcat(root->right->code, "1");
        makeCodes(root->right);
    }
}

//���������� � ����� out ���������������� ������ �� ������ fp � ���� ���������� ������������� ��������� ����
//���������� ������ "������" 
int write101(SYM* chart, FILE* fp, FILE* out, int size)
{
    int ch;
    unsigned char c;
    int counter = 0;//���������� ��� ������� �������� �������������� ��� ��� "������"

    while ((ch = fgetc(fp)) != EOF)
    {
        c = (unsigned char)ch;
        for (int i = 0; i < size; i++)
        {
            if (chart[i].symbol == c)
            {
                //������ � ���� .101 ���������� ������������� ����������������� ������� � ������� ���������� ���
                counter += fprintf(out, "%s", chart[i].code);
            }
        }
    }

    return 8 - (counter % 8); //���������� ������� "������" � �����
}

//��������������� ��������� ������������� ����� � ��������
//���������� ���� ���������� � ������� ���������� ���� unsigned char
unsigned char pack(unsigned char buf[])
{
    union CODE code;
    code.byte.b1 = buf[0] - '0';
    code.byte.b2 = buf[1] - '0';
    code.byte.b3 = buf[2] - '0';
    code.byte.b4 = buf[3] - '0';
    code.byte.b5 = buf[4] - '0';
    code.byte.b6 = buf[5] - '0';
    code.byte.b7 = buf[6] - '0';
    code.byte.b8 = buf[7] - '0';
    return code.ch;
}

//���������� � ����� out ��������� ����� � ���������������� ���������� �� ������ fp
void writeCode(FILE* fp, FILE* out, int numberOfSymbols, SYM* chart, int tail, int size, char* id, char* fileType)
{
    unsigned char buf[8] = { 0 }; //������ ��� ���������� �������� ���������� ������������� �����
    int ch;

    fwrite(id, sizeof(char), strlen(id), out); //������ �������������� �����
    fwrite(&numberOfSymbols, sizeof(int), 1, out); //������ ���������� ���������� ��������

    //������ ������� ����������� ������� � ��� ������� �������������
    for (int i = 0; i < numberOfSymbols; i++)
    {
        fwrite(&chart[i].symbol, sizeof(char), 1, out);
        fwrite(&chart[i].frequency, sizeof(float), 1, out);
    }

    fwrite(&tail, sizeof(int), 1, out); //������ ������� "������" � �����
    fwrite(&size, sizeof(int), 1, out); //������ ������� ��������� ����� � ������
    int length = strlen(fileType);//���������� ����� ���������� ��������� ����� � ��������
    fwrite(&length, sizeof(int), 1, out);//������ ����� � �������� ���������� ��������� �����
    fwrite(fileType, sizeof(char), length, out);//������ ���������� ��������� �����

    //������ ����������� ����� .101 � �������� ����
    while (!feof(fp))
    {
        int i;
        //���������� ������ ����� � ��������� �������������
        for (i = 0; i < 8; i++)
        {
            if ((ch = fgetc(fp)) == EOF)
            {
                break;
            }
            buf[i] = (unsigned char)ch;
        }
        //���� ���� ���� �� ���� ��� ��� ������ ��������� ���� � ���������� ���
        if (i)
        {
            fputc(pack(buf), out);
        }
    }
}