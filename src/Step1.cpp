#include "Header1.h"

//������� ������� ������������� �������� � �������: /��� ������� / ����������� ����������� / ������� �������������/
int printChart(SYM* chart, int size)
{
    if (chart != NULL)
        for (int i = 0; i < size; i++)
        {
            printf("symbol %i = '%c', frequency = %f\n", chart[i].symbol, (char)chart[i].symbol, chart[i].frequency);
        }
    else
        printf("The chart is empty!\n\n");

    return -1;
}

//��������� ���� �� ������ � ������� chart;
//��������� ������ ������� ��� -1 ���� ������ �� ������.
int IsConteined(SYM* chart, unsigned char ch, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (chart[i].symbol == ch)
            return i;
    }
    return -1;
}

//��������� ������� SYM ��� ������ ����������� ������� � ����� ������� chart � ������������� ��� ����;
//���������� ��������� �� ������ ������� chart ��� NULL � ������ ����������� ���������� realloc.
SYM* addSymbol(SYM* chart, int size, unsigned char ch)
{
    SYM* newChart = (SYM*)realloc(chart, (size + 1) * sizeof(SYM));
    if (newChart != NULL)
    {
        newChart[size].symbol = ch;
        newChart[size].frequency = 0;
        newChart[size].code[0] = 0;
        newChart[size].left = NULL;
        newChart[size].right = NULL;
    }
    else
    {
        printf("function \"addSymbol\": ");
        return NULL;
    }

    return newChart;
}

//��������� ������������ ������ ��������, ������� ���� � ����� (����� fp);
//���������� ���������� ���������� ��������;
//���������� ������� ������������� ������� �������;
//���������� ��������� �� ������ ������� chart ��� NULL � ������ ����������� ���������� addSymbol.
SYM* findOut(SYM* chart, FILE* fp, int* numbersOfSymbols, int* size)
{
    long int counter = 0; //���������� �������� � ������� �����
    long int symbols[256] = { 0 }; //������ ��� �������� ���������� ������� �������, ������� ���� � ������� �����
    int ch = 0;
    unsigned char c;
    int index = 0;

    while ((ch = fgetc(fp)) != EOF)
    {
        c = (unsigned char)ch;
        //��������� ���� �� ������ � chart.
        if ((index = IsConteined(chart, c, *numbersOfSymbols)) >= 0)
            symbols[index]++;
        //���� �������� ��� - ��������� ��� � ����� chart.
        else
        {
            if ((chart = addSymbol(chart, *numbersOfSymbols, c)) != NULL) 
            {
                symbols[*numbersOfSymbols]++;
                (*numbersOfSymbols)++;
            }
            else
                return NULL;
        }
        counter++;
    }

    //��������� ������� ������������� ������� �������
    for (int i = 0; i < *numbersOfSymbols; i++)
        chart[i].frequency = (float)symbols[i] / counter;

    //������ ��������� �����
    *size = counter;
    return chart;
}

//������������ � qsort, ��������� �������� � chart �� �������� ������� �������������.
int compar(const void* p1, const void* p2)
{
    SYM* f1 = (SYM*)p1;
    SYM* f2 = (SYM*)p2;

    if (f1->frequency > (f2->frequency)) return -1;
    if (f1->frequency == (f2->frequency)) return 0;
    if (f1->frequency < (f2->frequency)) return 1;
}