#include "Header1.h"
#define STRSIZE 200

extern SYM* chart;
extern SYM** psym;
extern SYM* root;
extern int numberOfSymbols;
extern char inFileName[];
extern char fileType[];

extern char outFileName[];
extern char OFN[];
extern char op1[];
extern char op2[];
extern char op3[];
extern char commands[][6];

//����� � ������� ���������� � �����������
int giveIns()
{
    printf("**************\nPattern : \"code of a command\" \"name of the origin file\" \"name of the result file\"\n");
    printf("\nCode of a command:\n");
    printf("\"c\" - compress the \"name of the origin file\" into \"name of the result file\"\n");
    printf("\"d\" - decompress the \"name of the origin file\" into \"name of the result file\"\n");
    printf("\"dcmd\" - decompress the \"name of the origin file\" into Console(stdout)\n");
    printf("\"f\" - display list of symbols and its frequency\n\n");
    printf("\"name of the origin file\" and \"name of the result file\" can only consist of letters and numbers!!!\n\n");
    printf("The \"name of the result file\" isn't necessary, on default it is \"OutFile.txt\".\nAlso when you use decompressing u can write a file name without an extension.\n");
    printf("For exit enter \"exit\"\n**************\n");

    return -1;
}

//����� ���������� �� ������
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

    return code;
}

//������������ ������ ���������� ������� root
void freeTree(SYM* root)
{
    //"�����" ������ ������ ���������� � free ������-��� ��� �������� �� ����������� malloc\realloc
    if (root->left == NULL && root->right == NULL)
        return;
    if (root->left != NULL)
        freeTree(root->left);
    if (root->right != NULL)
        freeTree(root->right);
    //���� � �������� ���� "������" �� ��� �����, ��� ����������� malloc
    free(root);
}

//������������ ������, �������� ���������� chart, psym, root ��������������� NULL
void cleanVars()
{
    if (root != NULL)
        freeTree(root);
    free(chart);
    free(psym);
    chart = NULL;
    psym = NULL;
    root = NULL;
}

//����������� ������������ ���� �������
int isCorrectOp1(char* op1)
{
    //������� ������ ��������� � ����� �� ���������������
    for (int i = 0; i < 3; i++)
        if (!strcmp(commands[i], op1))
            return 1;

    return 0;
}

//����������� ������������ ����� �����(������ ���� ������� �����, �����, ����������. �������������� ����� ������ ����� � �����)
int isFileNameValid(char* fileName, int flag)
{
    int i = 0;
    //� ������� ����� ������ ���� ���� �� ���� ������
    if (!isalnum(fileName[i]))
        return 0;
    //��������� ��������� ������� ������� �����
    while (fileName[++i])
        if (!isalnum(fileName[i]))
            break;
    
    //��������� ��� ����� ������� ����� ����� �����
    if (fileName[i++] != '.')
    {
        //��������� ���� ���������� ���������� ����� �� ��������� �������� ������ ������� �����
        if (flag && !strcmp(op1, commands[1]))
            return 1;
        else
            return 0;
    }

    //� ���������� ������ ���� ���� �� ���� ������
    if (!isalnum(fileName[i]))
        return 0;
    
    //��������� ��������� ������� ����������
    while (fileName[++i])
        if (!isalnum(fileName[i]))
            return 0;
    
    //��� ����� ���������
    return 1;
}

//��������� �������� � ������� �������� �������
void cleanop(char* op)
{
    for (int i = 0; i < 100; i++)
        op[i] = 0;
}

//������ �� ���������� � ������������ �������� �������. ���������� ��������� ������ : ����� ���������� � ������� �������������
int analysisCommand(int number)
{
    //��������� ������� � ����� ����������
    if (number == 3)
    {
        if (isCorrectOp1(op1))
        {
            if (isFileNameValid(op2, 0))
                strcpy(inFileName, op2);
            else
                return Error(-2, op2);

            if (isFileNameValid(op3, 1))
                strcpy(outFileName, op3);
            else
                return Error(-2, op3);
        }
    }
    //��������� ������� � ����� ����������
    else if (number == 2)
    {
        if (isCorrectOp1(op1))
        {
            if (isFileNameValid(op2, 0))
            {
                strcpy(inFileName, op2);
                strcpy(outFileName, OFN);
            }
            else
                return Error(-2, op2);
        }
    }
    //��������� ������� � ����� ���������
    else if (number == 1)
    {
        //���� ������� "exit"
        if (!strcmp(commands[4], op1))
        {
            //������������ ������, �������� ���������� chart, psym, root ��������������� NULL
            cleanVars();
            return 0;
        }
        else if (!strcmp(commands[3], op1))
            return giveIns();
        else if (!strcmp(commands[5], op1))
            return printChart(chart, numberOfSymbols);
        else
            return Error(-3, op1);
    }
    else
        return Error(-3, op1);

    return 1;
}

//������������� ������� �������� ��� ������� ���������
int readCommand_C(int argc, char** argv)
{
    int number = 2; //����� ���������� ������ �������

    strcpy(op1, argv[1]);
    strcpy(op2, argv[2]);
    if (argc >= 4)
    {
        strcpy(op3, argv[3]);
        number++;
    }

    return analysisCommand(number);
}

//������������� ������� �������� � ���������� ������
int readCommand_D()
{
    cleanop(op1);
    cleanop(op2);
    cleanop(op3);

    int number = 0; //����� ���������� ������ �������
    char str[STRSIZE] = { 0 }; //������ ��� �������� �������� �������
    fgets(str, 200, stdin);
    if (str != NULL && (strlen(str) - 1) >= 0)
        str[strlen(str) - 1] = 0;
    else
        return Error(-3, str);

    //������ ������� �� �����
    if ((number = sscanf(str, "%s %s %s", op1, op2, op3)) == EOF)
        return Error(-3, str);

    return analysisCommand(number);
}

//��������� ����� ������ �����������, ���� ��� ���
void extension()
{
    int i = 0;
    while (outFileName[i])
        if (outFileName[i++] == '.')
            return;
    outFileName[i] = '.';
    outFileName[i + 1] = 0;
    strcat(outFileName, fileType);
}

