#include "Header1.h"

//проверяет подходящий ли файл
//возвращает метку
int check(FILE* fp, char* id)
{
    short length = strlen(id);
    int flag = 1;
    for (int i = 0; i < length; i++)
        if (id[i] != fgetc(fp))
            flag = 0;

    return flag;
}

//восстанавливаем таблицу уникальных символов chart
//возвращает указатель на первый элемент chart
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

//в buf заносится побитовое представление ch
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

//считывает информацию из файла по одному байту, разбивает ее на биты
//возвращает биты в порядке следования
unsigned short getDigit(int* i, unsigned short* buf, FILE* fp, int tail)
{
    //считывается 2 байта, что бы определить конец файла и "урезать" последний байт на размер "хвоста"
    static unsigned char ch1;
    static unsigned char ch2;
    int c = 0;

    //первое считывание 2 байтов отличается от последующих и выделено в отдельный шаг
    if (*i == -1)
    {
        ch1 = (unsigned char)fgetc(fp);
        if (feof(fp))
            return 2;
        doBuf(buf, ch1); //получаем побитное представление байта
        ch2 = (unsigned char)fgetc(fp);
        if (feof(fp))//если найден конец файла "урезаем" последний значимый байт на размер "хвоста"
            buf[8 - tail] = 2;
        *i = 0;
    }
    //последующее считывание байтов после первого
    else if (*i == 8 && !feof(fp))
    {
        ch1 = ch2;
        doBuf(buf, ch1);//получаем побитное представление байта
        ch2 = (unsigned char)fgetc(fp);
        if (feof(fp))//если найден конец файла "урезаем" последний значимый байт на размер "хвоста"
        {
            buf[8 - tail] = 2;
        }
        *i = 0;
    }

    return buf[*i];    
}

//восстановливает и выводит в консоль содержимое исходного файла
void printText(FILE* fp, SYM* root, int tail)
{
    SYM* temp = root;
    unsigned short buf[9] = { 0 };//массив для хранения строкового представления байта, 9 элемент нужен для определения конца файла при нулевом размере "хвоста"
    unsigned short digit;//для хранения значение каждого последующего бита
    int i = -1;//счетчик порядкового номера бита в байте, "-1" обозначает что ни один байт еще не считан

    //считываем побитово информацию и перемещаемся по дереву пока не находим "лист"б когда нашли - выводим символ и переходим к "корню" дерева
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

//восстановливает и выводит в файл содержимое исходного файла
void writeDownText(FILE* fp, FILE* out, SYM* root, int tail)
{
    SYM* temp = root;
    unsigned short buf[9] = { 0 };//массив для хранения строкового представления байта, 9 элемент нужен для определения конца файла при нулевом размере "хвоста"
    unsigned short digit;//для хранения значение каждого последующего бита
    int i = -1;//счетчик порядкового номера бита в байте, "-1" обозначает что ни один байт еще не считан

    //считываем побитово информацию и перемещаемся по дереву пока не находим "лист"б когда нашли - выводим символ и переходим к "корню" дерева
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