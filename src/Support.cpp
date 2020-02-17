#include "Header1.h"
#define STRSIZE 200

extern SYM* chart;
extern SYM** psym;
extern SYM* root;
//extern int numberOfSymbols;
//extern int size;
//extern int tail;
//extern char id[]; //итендификатор, используемый для распознования "своих" файлов
extern char inFileName[]; //имя исходного файла
extern char fileType[]; //расширение файла

extern char outFileName[]; //имя выходного файла
extern char OFN[];
//extern char file101[];
extern char op1[];
extern char op2[];
extern char op3[];
extern char commands[][5];

//вывод в консоль инструкции к компрессору
int giveIns()
{
    printf("**************\nPattern : \"code of a command\" \"name of the origin file\" \"name of the result file\"\n");
    printf("\nCode of a command:\n");
    printf("\"c\" - compress the \"name of the origin file\" into \"name of the result file\"\n");
    printf("\"d\" - decompress the \"name of the origin file\" into \"name of the result file\"\n");
    printf("\"dcmd\" - decompress the \"name of the origin file\" into Console(stdout)\n\n");
    printf("\"name of the origin file\" and \"name of the result file\" can only consist of letters and numbers!!!\n\n");
    printf("The \"name of the result file\" isn't necessary, on default it is \"OutFile.txt\"\n\n");
    printf("For exit enter \"exit\"\n**************\n");

    return -1;
}

//вывод информации об ошибке
int Error(int code, char* s)
{
    switch (code)
    {
    case -4:
        printf("\"%s\" is too long name or command!\n", s);
        break;
    case -3:
        printf("\"%s\" is invalid command!\n", s);
        break;
    case -2:
        printf("\"%s\"invalid name of a file!\n", s);
        break;
    case 1:
        printf("\"%s\"openning file error\n", s);
        break;
    case 2:
        printf("malloc/realloc error!\n");
        break;
    case 3:
        printf("\"%s\"invalid file!\n", s);
        break;
    case 4:
        printf("\"%s\"invalid length fileType!\n", s);
        break;
    default:
        printf("unknown error!\n");
        break;
    }

    printf("For instructions enter \"ins\"\n");

    return code;
}

//определение корректности кода команды
int isCorrectOp1(char* op1)
{
    //команда должна совпадать с одной из предусмотренных
    for (int i = 0; i < 3; i++)
        if (!strcmp(commands[i], op1))
            return 1;

    return 0;
}

//определение допустимости имени файла(должны быть именная часть, точка, расширение. Использоваться могут только буквы и цифры)
int isFileNameValid(char* fileName)
{
    int i = 0;
    //в именной части должен быть хотя бы один символ
    if (!isalnum(fileName[i]))
        return 0;
    //проверяем остальные символы именной части
    while (fileName[++i])
        if (!isalnum(fileName[i]))
            break;
    
    //проверяем что после именной части стоит точка
    if (fileName[i++] != '.')
        return 0;
    //в расширении должен быть хотя бы один символ
    if (!isalnum(fileName[i]))
        return 0;
    
    //проверяем остальные символы расширения
    while (fileName[++i])
        if (!isalnum(fileName[i]))
            return 0;
    
    //имя файла корректно
    return 1;
}

//определение составляющих команды
//int ops(char* str)
//{
//    int i = 0;
//    int j = 0;
//    while (str[i])
//    {
//        if (isspace(str[i]))
//            break;
//        else
//            op1[j++] = str[i];
//        i++;
//    }
//    if (!str[i] && op1[0] != 0)
//        return 1;
//    i++;
//    j = 0;
//    while (str[i])
//    {
//        if (isspace(str[i]))
//            break;
//        else
//            op2[j++] = str[i];
//        i++;
//    }
//    if (!str[i] && op2[0] != 0)
//        return 2;
//    i++;
//    j = 0;
//    while (str[i])
//    {
//        if (isspace(str[i]))
//            break;
//        else
//            op3[j++] = str[i];
//        i++;
//    }
//    if (!str[i] && op3[0] != 0)
//        return 3;
//    
//    return 0;
//}

//обнуление массивов в которых хранятся команды
void cleanop(char* op)
{
    for (int i = 0; i < 100; i++)
        op[i] = 0;
}

//распознование команды
int readCommand()
{
    cleanop(op1);
    cleanop(op2);
    cleanop(op3);

    int number = 0;
    char str[STRSIZE] = { 0 };
    fgets(str, 200, stdin);
    if (str != NULL && (strlen(str) - 1) >= 0)
        str[strlen(str) - 1] = 0;
    else
        return Error(-3, str);

    if ((number = sscanf(str, "%s %s %s", op1, op2, op3)) == EOF)
        return Error(-3, str);
    //number = ops(str);

    printf("po1 : %s\nop2 : %s\nop3 : %s\n", op1, op2, op3);

    if (number == 3)
        if (isCorrectOp1(op1))
        {
            if (isFileNameValid(op2))
                strcpy(inFileName, op2);
            else
                return Error(-2, op2);
            if (isFileNameValid(op3))
                strcpy(outFileName, op3);
            else
                return Error(-2, op3);
        }
        else
            return Error(-3, op1);
    else if (number == 2)
        if (isCorrectOp1(op1))
            if (isFileNameValid(op2))
            {
                strcpy(inFileName, op2);
                strcpy(outFileName, OFN);
            }
            else
                return Error(-2, op2);
        else
            return Error(-3, op1);
    else if (number == 1)
        if (!strcmp(commands[4], op1))
            return 0;
        else if (!strcmp(commands[3], op1))
            return giveIns();
        else
            return Error(-3, op1);
    else
        return Error(-3, op1);

    return 1;
}

//освобождение памяти занимаемой деревом root
void freeTree(SYM* root)
{
    //"листы" дерева нельзя направлять в free потомучто эти элементы не создавались malloc\realloc
    if (root->left == NULL && root->right == NULL)
        return;
    if (root->left != NULL)
        freeTree(root->left);
    if (root->right != NULL)
        freeTree(root->right);
    //если у элемента были "листья" то это ветки, они создавались malloc
    free(root);
}

//освобождение памяти, значения переменных chart, psym, root устанавливается NULL
void cleanVars()
{
    freeTree(root);
    free(chart);
    free(psym);
    chart = NULL;
    psym = NULL;
    root = NULL;
}
