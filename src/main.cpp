#include "Header1.h"
#include "Header2.h"

SYM* chart; //������� ������������� ��������
SYM** psym; //������ ���������� �� �������� ������� chart
SYM* root; //�������� ������� ������
int numberOfSymbols; //���������� ���������� ��������
int size; //������ ��������� ����� � ������
int tail; // ����� "������"

char id[] = "HFCmprsGAN";  //�������������, ������������ ��� ������������� "�����" ������
char OFN[] = "OutFile"; //��� ����� � ����������� ������ ����������� �� ���������, ���������� ����������������� ��� ����������
char FTN[] = "txt"; //���������� ��������� ����� �� ���������
char file101[] = "101.txt"; //��� ����� ��� �������� ���������� ������������� ���������������� ����������
char inFileName[20] = { 0 }; //��� ��������� �����
char fileType[10] = { 0 };//���������� �����
char outFileName[20] = { 0 }; //��� ��������� �����
char op1[100] = { 0 }; //������ ����� ������� ��� ����������� - ��� �������
char op2[100] = { 0 }; //������ ����� ������� ��� ����������� - ��� ����� ��� ���������
char op3[100] = { 0 }; //������ ����� ������� ��� ����������� - ��� ����� ��� ������ ����������
char commands[6][6] = { "c", "d", "dcmd", "ins", "exit", "f" }; //������� ������

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