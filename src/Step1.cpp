#include "Header1.h"

//выводит таблицу встречаемости символов в формате: /код символа / графическое отображение / частота встречаемости/
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

//проверяет есть ли символ в массиве chart;
//возращает индекс символа или -1 если символ не найден.
int IsConteined(SYM* chart, unsigned char ch, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (chart[i].symbol == ch)
            return i;
    }
    return -1;
}

//добавляет элемент SYM для нового уникального символа в конец массива chart и иницализирует его поля;
//возвращает указатель на первый элемент chart или NULL в случае неуспешного выполнения realloc.
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

//формирует динамический массив символов, которые есть в файле (поток fp);
//определяет количество уникальных символов;
//определяет частоту встречаемости каждого символа;
//возвращает указатель на первый элемент chart или NULL в случае неуспешного выполнения addSymbol.
SYM* findOut(SYM* chart, FILE* fp, int* numbersOfSymbols, int* size)
{
    long int counter = 0; //количество символов в входном файле
    long int symbols[256] = { 0 }; //массив для хранения количества каждого символа, который есть в входном файле
    int ch = 0;
    unsigned char c;
    int index = 0;

    while ((ch = fgetc(fp)) != EOF)
    {
        c = (unsigned char)ch;
        //проверяем есть ли символ в chart.
        if ((index = IsConteined(chart, c, *numbersOfSymbols)) >= 0)
            symbols[index]++;
        //если элемента нет - добавляем его в конец chart.
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

    //вычисляем частоту встречаемости каждого символа
    for (int i = 0; i < *numbersOfSymbols; i++)
        chart[i].frequency = (float)symbols[i] / counter;

    //размер исходного файла
    *size = counter;
    return chart;
}

//используется в qsort, сортирует элементы в chart по убыванию частоты встречаемости.
int compar(const void* p1, const void* p2)
{
    SYM* f1 = (SYM*)p1;
    SYM* f2 = (SYM*)p2;

    if (f1->frequency > (f2->frequency)) return -1;
    if (f1->frequency == (f2->frequency)) return 0;
    if (f1->frequency < (f2->frequency)) return 1;
}