#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "task.h"
#define SIZE 256

int main (int argc, char *argv[])
{
    srand(time(0));

    FILE *fp_in;                       // указатель на исходный файл
    FILE *fp_101;                      // указатель на файл с бинарным кодом (временный с нулями и единицами)
    FILE *fp_vvk;                      // указатель на сжатый файл
    int start_time = clock();

    char resultName[SIZE] = {0};       // массив для записи имени сжатого файла с новым расширением
    strcpy(resultName, argv[1]);

    fp_101 = fopen("temp.txt","wb");                   // открываем файл для записи бинарного кода
    fp_vvk = fopen(strcat(resultName, ".vvk"),"wb");   // открываем файл для записи сжатой информации и присваиваем ему новое расширение

    int tempCh;                        // в эту переменную читается информация из файла
    int count = 0;                     // счётчик количества уникальных символов в исходнике
    int globalCount = 0;               // счётчтк количества всех символов в исходнике
    int count101 = 0;                  // счётчик количества символов из 0 и 1 в промежуточном .101 файле
    int quantity[SIZE] = {0};          // инициализируем массив количества уникальных символов
    struct SYM symbols[SIZE];          // инициализируем массив структур типа SYM
    struct SYM *psym[SIZE];            // инициализируем массив указателей на структуры типа SYM
    float sumFreq = 0;                 // сумма частот встречаемости

    if(argc != 2)
    {
        printf("Имя нужного файла: %s \n", argv[0]);
        return 1;
    }

    if((fp_in = fopen(argv[1],"rb")) == NULL)               // исключение ошибки чтения/нахождения файла
    {
        puts("Файл не найден!");
        return 1;
    }

    while((tempCh=fgetc(fp_in))!=EOF)                       // побайтно читаем исходник и составлять таблицу встречаемости
    {
        for(int i = 0; i < SIZE; i++)
        {
            if (tempCh==symbols[i].ch)
            {
                quantity[i]++;
                globalCount++;
                break;
            }
            if (symbols[i].ch == 0)
            {
                symbols[i].ch = (unsigned char)tempCh;
                quantity[i] = 1;
                count++; globalCount++;
                break;
            }
        }
    }

    for(int i = 0; i < count; i++)                             // рассчёт частоты встречаемости
        symbols[i].freq = (float)quantity[i] / globalCount;

    for(int i = 0; i < count; i++)                             // в массив указателей заносим адреса структур
        psym[i] = &symbols[i];

    struct SYM temp;                                           // сортировка по убыванию
    for(int i = 1; i < count; i++)
        for(int j = 0; j < count - 1; j++)
            if(symbols[j].freq < symbols[j + 1].freq)
            {
                temp=symbols[j];
                symbols[j] = symbols[j + 1];
                symbols[j + 1] = temp;
            }

    for(int i = 0; i < count; i++)
    {
        sumFreq += symbols[i].freq;
        printf("Ch= %d\tFreq= %f\tPPP= %c\t\n", symbols[i].ch, symbols[i].freq, psym[i]->ch);
    }
    printf("\nSlova = %d\tsumFreq=%f\n", globalCount, sumFreq);

    struct SYM *root = buildTree(psym, count);      // вызов функции создания дерева Хаффмана
    makeCodes(root);                                // вызов функции получения кода
    rewind(fp_in);                                  // возвращаем указатель в файле в начало файла

    while((tempCh = fgetc(fp_in)) != EOF)           // читаем исходник, и записываем полученные через makeCodes коды в .101 файл
    {                                               // каждый прочитанный символ просматривается в массиве структур SYM в поле ch
        for(int i = 0; i < count; i++)
            if(symbols[i].ch == tempCh)             // когда совпадение произошло, необходимо вывести в .101 файл содержимое поля code
            {
                fputs(symbols[i].code, fp_101);     // выводим строку с кодом
                break;                              // прерываем поиск
            }
    }
    fclose(fp_101);

    int i = 0;
    fp_101 = fopen("temp.txt", "rb");               // вновь открываем промежуточный .101 файл с бинарным кодом (для чтения)

    while((tempCh = fgetc(fp_101)) != EOF)          // считаем количество единиц инулей в .101 файле (определяем его размер)
        count101++;

    int tail = count101 % 8;                        // находим остаток, размер хвоста файла (количество символов не кратных 8 в .101 файле)

    /* _____________________________формируем заголовок сжатого файла через поля байтов__________________________ */

    fwrite("vvk", 3, 1, fp_vvk);                    // сигнатура (подпись формата)
    fwrite(&count, 3, 1, fp_vvk);                   // количество уникальных символов (число строк в таблице встречаемости)
    for(i = 0; i < count; i++)                      // сама таблица встречаемости (пары: код символа - частота)
    {
        fwrite(&symbols[i].ch, 3, 1, fp_vvk);
        fwrite(&symbols[i].freq, 4, 1, fp_vvk);
    }
    fwrite(&tail, 1, 1, fp_vvk);                    // длина хвоста (остаток деления размера .101 файла на 8)

    /* _____________________________________________конец заголовка_____________________________________________ */

    rewind(fp_101);                                 // возвращаем указатель в промежуточном файле в начало файла

    unsigned char buf[8] = {0};                     // инициализируем массив единиц и нулей для union
    union CODE code;                                // инициализируем переменную code для ф-и pack
    char j = 0;
    for(int i = 0; i < count101 - tail; i++)        // читаем .101 файл, занося последовательно каждые 8 элементов в массив для побитовой обработки в union
    {
        buf[j] = fgetc(fp_101);
        if(j == 7)
        {
            pack(buf);
            fputc(code.ch, fp_vvk);
            j = 0;
        }
        j++;
    }

    j = 0;
    for(int i = 0; i <= tail; i++)                  // записываем хвост
    {
        buf[j] = fgetc(fp_101);
        if(j == tail)
        {
            pack(buf);
            fputc(code.ch, fp_vvk);
        }
        j++;
    }

    /* ______________________________________Всякие необязательные штуки____________________________________ */

    int end_time = clock();
    int search_time = end_time - start_time;
    printf("Затраченное на сжатие время: %f секунд\n", (search_time/1000.0)/60);
    fseek(fp_in, 0, SEEK_END);                                                      // переместить внутренний указатель в конец файла
    int size = ftell(fp_in);                                                        // вернуть текущее положение внутреннего указателя
    printf("Размер вашего файла до сжатия составлял: %d байт\n", size);
    fseek(fp_vvk, 0, SEEK_END);
    int size2 = ftell(fp_vvk);
    printf("Размер вашего файла после сжатия составляет: %d байт\n", size2);

    fcloseall();  //закрываем все открытые файлы
    return 0;
}

