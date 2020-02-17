#include "Header1.h"
#include "Header2.h"

extern SYM* chart;
extern SYM** psym;
extern SYM* root;
extern int numberOfSymbols;
extern int size;
extern int tail;
extern char id[]; //�������������, ������������ ��� ������������� "�����" ������
extern char inFileName[]; //��� ��������� �����
extern char fileType[]; //���������� �����

extern char outFileName[]; //��� ��������� �����
//extern char OFN[];
extern char file101[];
//extern char op1[];
//extern char op2[];
//extern char op3[];
//extern char commands[][5];

int Compress()
{
    FILE* fp = NULL;
    FILE* out = NULL;

    //����������� ���������� �����
    for (int i = 0, k = -1; i < strlen(inFileName); i++)
    {
        if (k >= 0)
            fileType[k++] = inFileName[i];
        if (inFileName[i] == '.' && k == -1)
            k = 0;
    }

    //�������� ��������� �����
    if ((fp = fopen(inFileName, "rt")) == NULL)
        return Error(1, inFileName);

    //��������� ������� ������������� ��������
    if ((chart = findOut(chart, fp, &numberOfSymbols, &size)) == NULL)
        return Error(2, NULL);
    fclose(fp);

    //���������� ������� ������������� �������� �� �������� ������� �������������
    qsort(chart, numberOfSymbols, sizeof(SYM), compar);

    //�������� ������� ���������� �� �������� ������� �������������
    if ((psym = makePsym(psym, chart, numberOfSymbols)) == NULL)
        return Error(2, NULL);

    //��������������� ������ �� ��������� ��������
    if ((root = buildTree(psym, numberOfSymbols)) == NULL)
        return Error(2, NULL);

    //����������� ����������� ���� ��� ������� ����������� �������
    makeCodes(root);

    //�������� ��������� �����
    fp = fopen(inFileName, "rt");
    if (fp == NULL)
        return Error(1, inFileName);

    //�������� ����� 101 ��� �������� ���������� ������������� ���������������� ����������
    out = fopen(file101, "wt");
    if (fp == NULL)
        return Error(1, file101);

    //���������� ����� 101, ��������� - ������ ������
    tail = write101(chart, fp, out, numberOfSymbols);

    fclose(fp);
    fclose(out);

    //�������� ����� 101 ��� ������
    fp = fopen(file101, "rt");
    if (fp == NULL)
        return Error(1, file101);

    //�������� ����� ��� ������ ���������������� ���������� � �������� ����
    out = fopen(outFileName, "wb");
    if (fp == NULL)
        return Error(1, outFileName);

    //������ ���������������� �������� ������ � �������� ���� � ����
    writeCode(fp, out, numberOfSymbols, chart, tail, size, id, fileType);

    fclose(fp);
    fclose(out);

    //������������ ������, �������� ���������� chart, psym, root ��������������� NULL
    cleanVars();

    printf("Successful!\n\n");
    return 0;
}

int Decompress(int way)
{
    FILE* fp = NULL;
    FILE* out = NULL;

    //�������� ����� � ���������������� �������� �����������
    fp = fopen(inFileName, "rb");
    if (fp == NULL)
        return Error(1, inFileName);

    //��������� ���������� �� ����
    if (!check(fp, id))
        return Error(3, inFileName);

    //��������� ���������� ���������� �������� �� ��������� �����
    fread(&numberOfSymbols, sizeof(int), 1, fp);

    //��������������� ������� ���������� ��������
    if ((chart = buildChart(chart, numberOfSymbols, fp)) == NULL)
        return Error(2, NULL);

    //�������� ������� ���������� �� �������� chart
    if ((psym = makePsym(psym, chart, numberOfSymbols)) == NULL)
        return Error(2, NULL);

    //���������� ������
    if ((root = buildTree(psym, numberOfSymbols)) == NULL)
        return Error(2, NULL);

    //����������� ����������� ���� ��� ������� ����������� �������
    makeCodes(root);

    //��������� ������ "������" � ����� �� ��������� �����
    fread(&tail, sizeof(int), 1, fp);

    //��������� ������ ��������� ����� � ������ �� ��������� �����
    fread(&size, sizeof(int), 1, fp);

    int length;//����� ���������� �����

    //��������� ����� ���������� � ��������� ��� ������������
    fread(&length, sizeof(int), 1, fp);
    if (length > 10)
        return Error(4, inFileName);

    //��������� ���������� ��������� �����
    fread(fileType, sizeof(char), length, fp);

    //���������� ���� �������� ��������������� ����������
    if (way)
    {
        //�������������� � ����� ���������� ��������� ����� � �������
        printf("***********\n");
        printText(fp, root, tail);
        printf("\n***********\n");
    }
    else
    {
        //�������� ����� ��� ������ ��������������� ����������
        out = fopen(outFileName, "wt");
        if (out == NULL)
            return Error(1, outFileName);

        //�������������� � ����� ���������� ��������� ����� � �������� ����
        writeDownText(fp, out, root, tail);
        fclose(out);
    }

    fclose(fp);

    //������������ ������, �������� ���������� chart, psym, root ��������������� NULL
    cleanVars();

    printf("Successful!\n\n");
    return 0;
}