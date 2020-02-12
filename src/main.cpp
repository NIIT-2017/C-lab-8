#include "Header1.h"


//int main(int argc, char* argv[])
int main()
{
    SYM* chart = NULL;
    SYM** psym = NULL;
    SYM* root = NULL;
    int numberOfSymbols = 0; //количество уникальных символов
    int size = 0; //размер исходного файла в байтах
    int tail = 0; // длина "хвоста"
    char id[] = "HFCmprsGUN"; //итендификатор, используемый для распознования "своих" файлов
    char fileName[20] = { 0 }; //имя исходного файла
    char fileType[10] = { 0 }; //расширение файла

    printf("Enter name of the file\n");
    if (scanf("%s", fileName) != 1)
    {
        printf("scanf error!\n");
        return 1;
    }

    for (int i = 0, k = -1; i < strlen(fileName); i++)
    {
        if (k >= 0)
            fileType[k++] = fileName[i];
        if (fileName[i] == '.' && k == -1)
            k = 0;
    }

    printf("%s\n", fileType);
    
    FILE* fp = NULL;
    FILE* out = NULL;
    //fp = fopen(argv[1], "rt");

    printf("%s\n", fileName);
    //if ((fp = fopen("programm.txt", "rt")) == NULL)
    if ((fp = fopen(fileName, "rt")) == NULL)
    {
        printf("openning file error\n");
        return 1;
    }

    if ((chart = findOut(chart, fp, &numberOfSymbols, &size)) == NULL) //получение таблицы встречаемости символов
        return 1;
    fclose(fp);
    //printf("\nnumber of symbols = %i\n", numberOfSymbols);
    //printChart(chart, numberOfSymbols);
    qsort(chart, numberOfSymbols, sizeof(SYM), compar); //сортировка таблицы встречаемости символов по убыванию частоты встречаемости
    printChart(chart, numberOfSymbols); // вывод таблицы встречаемости соимволов в формате: /код символа / графическое отображение / частота встречаемости/
    printf("***********\n");
    psym = makePsym(psym, chart, numberOfSymbols); //создание массива указателей на элементы таблицы встречаемости
    root = buildTree(psym, numberOfSymbols); //конструирование дерева по алгоритму Хаффмана
    makeCodes(root); //определение префиксного кода для каждого уникального символа

    /*for (int i = 0; i < numberOfSymbols; i++)
    {
        printf("symbol %i = '%c', code = %s\n", chart[i].symbol, (char)chart[i].symbol, chart[i].code);
    }*/

    fp = fopen("programm.txt", "rt");
    if (fp == NULL)
    {
        printf("openning file error\n");
        return 1;
    }
    out = fopen("out.txt", "wt");
    if (fp == NULL)
    {
        printf("openning file error\n");
        return 1;
    }

    tail = write101(chart, fp, out, numberOfSymbols); //создание файла 101, результат - размер хвоста

    fclose(fp);
    fclose(out);

    fp = fopen("out.txt", "rt");
    if (fp == NULL)
    {
        printf("openning file error\n");
        return 1;
    }
    out = fopen("code.txt", "wb");
    if (fp == NULL)
    {
        printf("openning file error\n");
        return 1;
    }

    writeCode(fp, out, numberOfSymbols, chart, tail, size, id, fileType); //запись перекодированных исходных данных в бинарном виде в файл

    //printf("\nsize of File = %i\ntail = %i\n\n", size, tail);

    fclose(fp);
    fclose(out);

    free(chart);
    free(psym);
    free(root);

    chart = NULL;
    psym = NULL;
    root = NULL;

    fp = fopen("code.txt", "rb");
    if (fp == NULL)
    {
        printf("openning file error\n");
        return 1;
    }

    //проверяем подходщий ли файл
    if (!check(fp, id))
    {
        printf("invalid file!\n");
        return 3;
    }
    
    fread(&numberOfSymbols, sizeof(int), 1, fp); //считываем количество уникальных символов из заголовка файла
    //printf("\nnumber of symbols = %i\n", numberOfSymbols);
    chart = buildChart(chart, numberOfSymbols, fp); //восстанавливаем таблицу уникальных символов

    psym = makePsym(psym, chart, numberOfSymbols); //создание массива указателей на элементы chart
    root = buildTree(psym, numberOfSymbols); //построение дерева
    makeCodes(root); //определение префиксного кода для каждого уникального символа

    /*for (int i = 0; i < numberOfSymbols; i++)
    {
        printf("symbol %i = '%c', code = %s\n", chart[i].symbol, (char)chart[i].symbol, chart[i].code);
    }*/

    fread(&tail, sizeof(int), 1, fp);//считываем размер "хвоста" в битах из заголовка файла
    fread(&size, sizeof(int), 1, fp);//считываем размер исходного файла в байтах из заголовка файла
    unsigned char length;
    if (length = fgetc(fp) > 10)
    {
        printf("invalid length fileType!\n");
        return 4;
    }
    fread(fileType, sizeof(char), length, fp);//считываем разрешение исходного файла
    printf("%s\n", fileType);
    //printf("tail = %i, size = %i\n", tail, size);

    printf("***********\n");
    printText(fp, root, tail); //восстановление и вывод содержание исходного файла
    printf("\n***********\n");

    fclose(fp);

    free(chart);
    free(psym);
    free(root);

    return 0;
}