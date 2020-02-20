#include "Header1.h"
#include "Header2.h"

extern SYM* chart;
extern SYM** psym;
extern SYM* root;
extern int numberOfSymbols;
extern int size;
extern int tail;
extern char id[];
extern char inFileName[];
extern char FTN[];
extern char fileType[];

extern char outFileName[]; //имя выходного файла
extern char file101[];

int Compress()
{
    //освобождение памяти, значения переменных chart, psym, root устанавливается NULL
    cleanVars();
    //если имя выходного файла не указано, дополнение имя файла, используемого по умолчанию, расширением по умолчанию
    strcpy(fileType, FTN);
    extension();
    
    FILE* fp = NULL;
    FILE* out = NULL;

    //определение разрешения файла
    for (int i = 0, k = -1; i < strlen(inFileName); i++)
    {
        if (k >= 0)
            fileType[k++] = inFileName[i];
        if (inFileName[i] == '.' && k == -1)
            k = 0;
    }

    //открытие исходного файла
    if ((fp = fopen(inFileName, "rt")) == NULL)
        return Error(1, inFileName);

    //получение таблицы встречаемости символов
    if ((chart = findOut(chart, fp, &numberOfSymbols, &size)) == NULL)
        return Error(2, NULL);
    fclose(fp);

    //сортировка таблицы встречаемости символов по убыванию частоты встречаемости
    qsort(chart, numberOfSymbols, sizeof(SYM), compar);

    //создание массива указателей на элементы таблицы встречаемости
    if ((psym = makePsym(psym, chart, numberOfSymbols)) == NULL)
        return Error(2, NULL);

    //конструирование дерева по алгоритму Хаффмана
    if ((root = buildTree(psym, numberOfSymbols)) == NULL)
        return Error(2, NULL);

    //определение префиксного кода для каждого уникального символа
    makeCodes(root);

    //открытие исходного файла
    fp = fopen(inFileName, "rt");
    if (fp == NULL)
        return Error(1, inFileName);

    //открытие файла 101 для хранения строкового представления перекодированной информации
    out = fopen(file101, "wt");
    if (fp == NULL)
        return Error(1, file101);

    //заполнение файла 101, результат - размер хвоста
    tail = write101(chart, fp, out, numberOfSymbols);

    fclose(fp);
    fclose(out);

    //открытие файла 101 для чтения
    fp = fopen(file101, "rt");
    if (fp == NULL)
        return Error(1, file101);

    //открытие файла для записи перекодированной информации в бинарном виде
    out = fopen(outFileName, "wb");
    if (fp == NULL)
        return Error(1, outFileName);

    //запись перекодированных исходных данных в бинарном виде в файл
    writeCode(fp, out, numberOfSymbols, chart, tail, size, id, fileType);

    fclose(fp);
    fclose(out);

    ////освобождение памяти, значения переменных chart, psym, root устанавливается NULL
    //cleanVars();

    printf("Successful!\n\n");
    return 0;
}

int Decompress(int way)
{
    //освобождение памяти, значения переменных chart, psym, root устанавливается NULL
    cleanVars();
    
    FILE* fp = NULL;
    FILE* out = NULL;
    long int dcSize = 0;

    //открытие файла с перекодированной исходной информацией
    fp = fopen(inFileName, "rb");
    if (fp == NULL)
        return Error(1, inFileName);

    //проверяем допустимый ли файл
    if (!check(fp, id))
        return Error(3, inFileName);

    //считываем количество уникальных символов из заголовка файла
    fread(&numberOfSymbols, sizeof(int), 1, fp);

    //восстанавливаем таблицу уникальных символов
    if ((chart = buildChart(chart, numberOfSymbols, fp)) == NULL)
        return Error(2, NULL);

    //создание массива указателей на элементы chart
    if ((psym = makePsym(psym, chart, numberOfSymbols)) == NULL)
        return Error(2, NULL);

    //построение дерева
    if ((root = buildTree(psym, numberOfSymbols)) == NULL)
        return Error(2, NULL);

    //определение префиксного кода для каждого уникального символа
    makeCodes(root);

    //считываем размер "хвоста" в битах из заголовка файла
    fread(&tail, sizeof(int), 1, fp);

    //считываем размер исходного файла в байтах из заголовка файла
    fread(&size, sizeof(int), 1, fp);

    int length;//длина расширения файла

    //считываем длину расширения и проверяем его допустимость
    fread(&length, sizeof(int), 1, fp);
    if (length > 10)
        return Error(4, inFileName);

    //считываем разрешение исходного файла
    fread(fileType, sizeof(char), length, fp);

    //определяем куда выводить восстановленную информацию
    if (way)
    {
        //восстановление и вывод содержание исходного файла в консоль
        printf("***********\n");
        dcSize = printText(fp, root, tail);
        printf("\n***********\n");
    }
    else
    {
        //восстановление исходного расширения файла
        extension();

        //открытие файла для записи восттановленной информации
        out = fopen(outFileName, "wt");
        if (out == NULL)
            return Error(1, outFileName);

        //восстановление и вывод содержание исходного файла в выходной файл
        dcSize = writeDownText(fp, out, root, tail);
        fclose(out);
    }

    fclose(fp);

    printf("\norigin file's size = %li bytes\ndecompressed file's size = %li bytes\n", size, dcSize);
    if (size == dcSize)
        printf("Successful!\n\n");
    else
        printf("Information lost!\n\n");
    return 0;
}