#include "Header1.h"

//��������� ���������� �� ����
//���������� �����
int check(FILE* fp, char* id)
{
    short length = strlen(id);
    int flag = 1;
    for (int i = 0; i < length; i++)
        if (id[i] != fgetc(fp))
            flag = 0;

    return flag;
}

//��������������� ������� ���������� �������� chart
//���������� ��������� �� ������ ������� chart
SYM* buildChart(SYM* chart, int numberOfSymbols, FILE* fp)
{
    chart = (SYM*)malloc(numberOfSymbols * sizeof(SYM));
    if (chart == NULL)
    {
        printf("function \"buildTree\" : ");
        return NULL;
    }

    for (int i = 0; i < numberOfSymbols; i++)
    {
        chart[i].symbol = (char)fgetc(fp);
        fread(&chart[i].frequency, sizeof(float), 1, fp);
        chart[i].code[0] = 0;
        chart[i].left = NULL;
        chart[i].right = NULL;
    }

    return chart;
}

//� buf ��������� ��������� ������������� ch
void doBuf(unsigned short* buf, char ch)
{
    CODE code;
    code.ch = ch;
    buf[0] = code.byte.b1;
    buf[1] = code.byte.b2;
    buf[2] = code.byte.b3;
    buf[3] = code.byte.b4;
    buf[4] = code.byte.b5;
    buf[5] = code.byte.b6;
    buf[6] = code.byte.b7;
    buf[7] = code.byte.b8;
    buf[8] = 2;
}

//��������� ���������� �� ����� �� ������ �����, ��������� �� �� ����
//���������� ���� � ������� ����������
unsigned short getDigit(int* i, unsigned short* buf, FILE* fp, int tail)
{
    //����������� 2 �����, ��� �� ���������� ����� ����� � "�������" ��������� ���� �� ������ "������"
    static unsigned char ch1;
    static unsigned char ch2;
    int c = 0;

    //������ ���������� 2 ������ ���������� �� ����������� � �������� � ��������� ���
    if (*i == -1)
    {
        ch1 = (unsigned char)fgetc(fp);
        if (feof(fp))
            return 2;
        doBuf(buf, ch1); //�������� �������� ������������� �����
        ch2 = (unsigned char)fgetc(fp);
        if (feof(fp))//���� ������ ����� ����� "�������" ��������� �������� ���� �� ������ "������"
            buf[8 - tail] = 2;
        *i = 0;
    }
    //����������� ���������� ������ ����� �������
    else if (*i == 8 && !feof(fp))
    {
        ch1 = ch2;
        doBuf(buf, ch1);//�������� �������� ������������� �����
        ch2 = (unsigned char)fgetc(fp);
        if (feof(fp))//���� ������ ����� ����� "�������" ��������� �������� ���� �� ������ "������"
        {
            buf[8 - tail] = 2;
        }
        *i = 0;
    }

    return buf[*i];    
}

//��������������� � ������� � ������� ���������� ��������� �����
void printText(FILE* fp, SYM* root, int tail)
{
    SYM* temp = root;
    unsigned short buf[9] = { 0 };//������ ��� �������� ���������� ������������� �����, 9 ������� ����� ��� ����������� ����� ����� ��� ������� ������� "������"
    unsigned short digit;//��� �������� �������� ������� ������������ ����
    int i = -1;//������� ����������� ������ ���� � �����, "-1" ���������� ��� �� ���� ���� ��� �� ������

    //��������� �������� ���������� � ������������ �� ������ ���� �� ������� "����"� ����� ����� - ������� ������ � ��������� � "�����" ������
    while ((digit = getDigit(&i, buf, fp, tail)) != 2)
    {
        if (digit == 0)
            temp = temp->left;
        else if (digit == 1)
            temp = temp->right;
        if ((temp->left == NULL) || (temp->right == NULL))
        {
            printf("%c", temp->symbol);
            temp = root;
        }
        i++;
    }
}

//��������������� � ������� � ���� ���������� ��������� �����
void writeDownText(FILE* fp, FILE* out, SYM* root, int tail)
{
    SYM* temp = root;
    unsigned short buf[9] = { 0 };//������ ��� �������� ���������� ������������� �����, 9 ������� ����� ��� ����������� ����� ����� ��� ������� ������� "������"
    unsigned short digit;//��� �������� �������� ������� ������������ ����
    int i = -1;//������� ����������� ������ ���� � �����, "-1" ���������� ��� �� ���� ���� ��� �� ������

    //��������� �������� ���������� � ������������ �� ������ ���� �� ������� "����"� ����� ����� - ������� ������ � ��������� � "�����" ������
    while ((digit = getDigit(&i, buf, fp, tail)) != 2)
    {
        if (digit == 0)
            temp = temp->left;
        else if (digit == 1)
            temp = temp->right;
        if ((temp->left == NULL) || (temp->right == NULL))
        {
            fputc(temp->symbol, out);
            temp = root;
        }
        i++;
    }
}