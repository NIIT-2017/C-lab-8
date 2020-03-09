#include "Header1.h"
#include "Header2.h"

SYM* chart; //таблица встречаемости символов
SYM** psym; //массив указаьелей на элементы массива chart
SYM* root; //корневой элемент дерева
int numberOfSymbols; //количество уникальных символов
int size; //размер исходного файла в байтах
int tail; // длина "хвоста"

char id[] = "HFCmprsGAN";  //итендификатор, используемый для распознования "своих" файлов
char OFN[] = "OutFile"; //имя файла с результатом работы компрессора по умолчанию, расширение восстанавливается при распоковке
char FTN[] = "txt"; //расширение выходного файла по умолчанию
char file101[] = "101.txt"; //имя файла для хранения строкового представления перекодированной информации
char inFileName[20] = { 0 }; //имя исходного файла
char fileType[10] = { 0 };//расширение файла
char outFileName[20] = { 0 }; //имя выходного файла
char op1[100] = { 0 }; //первая часть команды для компрессора - код команды
char op2[100] = { 0 }; //вторая часть команды для компрессора - имя файла для обработки
char op3[100] = { 0 }; //третья часть команды для компрессора - имя файла для вывода результата
char commands[6][6] = { "c", "d", "dcmd", "ins", "exit", "f" }; //каталог команд

int RunCompressor()
{
    if (!strcmp(op1, commands[0]))
        return Compress();
    else if (!strcmp(op1, commands[1]))
        return Decompress(0);
    else
        return Decompress(1);
}

int main(int argc, char* argv[])
{
    chart = NULL;
    psym = NULL;
    root = NULL;
    numberOfSymbols = 0;
    size = 0;
    tail = 0;
    strcpy(outFileName, OFN);
    int flag = 0;

    if (argc >= 3)
    {
        flag = readCommand_C(argc, argv);
        if (flag > 0)
            return RunCompressor();
    }
    else
    {
        printf("This is Compressor\n");
        printf("Please, enter a command. For instructions enter \"ins\"\n");
        while (flag = readCommand_D())
        {
            if (flag > 0)
                RunCompressor();
            printf("Please, enter a command. For instructions enter \"ins\"\n");
        }
    }

    return 0;
}