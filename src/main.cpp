#include "Header1.h"
#include "Header2.h"

SYM* chart;
SYM** psym;
SYM* root;
int numberOfSymbols; //���������� ���������� ��������
int size; //������ ��������� ����� � ������
int tail; // ����� "������"

char id[] = "HFCmprsGAN";  //�������������, ������������ ��� ������������� "�����" ������
char OFN[] = "OutFile.txt";
char file101[] = "101.txt";
char inFileName[20] = { 0 }; //��� ��������� �����
char fileType[10] = { 0 };//���������� �����
char outFileName[20] = { 0 }; //��� ��������� �����
char op1[100] = { 0 };
char op2[100] = { 0 };
char op3[100] = { 0 };
char commands[5][5] = { "c", "d", "dcmd", "ins", "exit" };

int main()
{
    chart = NULL;
    psym = NULL;
    root = NULL;
    numberOfSymbols = 0;
    size = 0;
    tail = 0;
    strcpy(outFileName, OFN);

    printf("This is Compressor\n");
    printf("Please, enter a command. For instructions enter \"ins\"\n");
    int flag = 0;
    while (flag = readCommand())
    {
        if (flag > 0)
        {
            if (!strcmp(op1, commands[0]))
                flag = Compress();
            else if (!strcmp(op1, commands[1]))
                flag = Decompress(0);
            else
                flag = Decompress(1);
            if (flag)
                return flag;
        }
        printf("Please, enter a command. For instructions enter \"ins\"\n");
    }

    return 0;
}