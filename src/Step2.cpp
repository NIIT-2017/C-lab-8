#include "Header1.h"
#include <string.h>

//makePsym:
//������� ������ ���������� �� ������� SYM ���������� � chart
//���������� ��������� �� ������ ������� ������� psym
SYM** makePsym(SYM** psym, SYM* chart, int size)
{
    psym = (SYM**)malloc(size * sizeof(SYM*));
    if (psym == NULL)
    {
        printf("malloc error\n");
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

//buildTree:
//������ �������� ������ �� ��������� �������� ��������� psym
//���������� ��������� �� ������ ������
SYM* buildTree(SYM* psym[], int N)
{
    SYM* temp = (SYM*)malloc(sizeof(SYM));
    if (temp == NULL)
    {
        printf("malloc error\n");
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

    /*for (int i = 0; i < N - 1; i++)
    {
        printf("symbol %i = '%c', frequency = %f\n", psym[i]->symbol, psym[i]->symbol, psym[i]->frequency);
    }
    printf("***********\n");*/

    return buildTree(psym, N - 1);
}

//makeCodes:
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

//write101:
//���������� � ����� out ���������������� ������ �� ������ fp � ���� ���������� ������������� ��������� ����
//���������� ������ "������" 
int write101(SYM* chart, FILE* fp, FILE* out, int size)
{
    int ch;
    unsigned char c;
    int counter = 0;//���������� ��� ������� �������� �������������� ��� ��� "������"
    //int coun = 0;

    while ((ch = fgetc(fp)) != EOF)
    {
        c = (unsigned char)ch;
        for (int i = 0; i < size; i++)
        {
            if (chart[i].symbol == c)
            {
                counter += fprintf(out, "%s", chart[i].code);//������ � ���� .101 ���������� ������������� ����������������� ������� � ������� ���������� ���
                break;
            }
        }
        //coun++;
    }

    //printf("\nnumber of symbols = %i\n", coun);
    //printf("\nnumber of characters = %i\n", counter);
    return 8 - (counter % 8); //���������� ������� "������" � �����
}

//pack:
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

//writeCode:
//���������� � ����� out ��������� ����� � ���������������� ���������� �� ������ fp
void writeCode(FILE* fp, FILE* out, int numberOfSymbols, SYM* chart, int tail, int size, char* id, char* fileType)
{
    unsigned char buf[8] = { 0 }; //������ ��� ���������� �������� ���������� ������������� �����
    //int flag = 1;
    int ch;
    //int counter = 0;
    //int coun = 0;
    FCODE fcode; //������������ ��� ������ ���������� ���� float � �������� ����

    //fprintf(out, "%s%i", id, numberOfSymbols);
    fwrite(id, sizeof(char), strlen(id), out); //������ �������������� �����
    fwrite(&numberOfSymbols, sizeof(int), 1, out); //������ ���������� ���������� ��������
    /*for (int i = 0; i < numberOfSymbols; i++)
        fprintf(out, "%c%f", chart[i].symbol,chart[i].frequency);*/

    //������ ������� ����������� ������� � ��� ������� �������������
    for (int i = 0; i < numberOfSymbols; i++)
    {
        fprintf(out, "%c", chart[i].symbol);
        fcode.ft = chart[i].frequency;
        fprintf(out, "%c%c%c%c", fcode.quad.c1, fcode.quad.c2, fcode.quad.c3, fcode.quad.c4);
    }

    //fprintf(out, "%i%i", tail, size);
    fwrite(&tail, sizeof(int), 1, out); //������ ������� "������" � �����
    fwrite(&size, sizeof(int), 1, out); //������ ������� ��������� ����� � ������
    fprintf(out, "%c%s", strlen(fileType), fileType); //������ ����� ���������� ����� � ���������� �����

    //������ ����������� ����� .101 � �������� ����
    while (!feof(fp))
    {
        int i;
        //���������� ������ ����� � ��������� �������������
        for (i = 0; i < 8; i++)
        {
            if ((ch = fgetc(fp)) == EOF)
            {
                //flag = 0;
                break;
            }
            buf[i] = (unsigned char)ch;
        }
        //counter += i;
        //���� ���� ���� �� ���� ��� ��� ������ ��������� ���� � ���������� ���
        if (i)
        {
            fputc(pack(buf), out);
            //coun++;
        }
    }
    //printf("\nnumber of zero//one were written = %i\n", counter);
    //printf("\nnumber of characters were written = %i\n", coun);
}