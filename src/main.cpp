#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "task.h"
#define SIZE 256
#define rasmer1byte 8

/*��������� ��������� 2 ���������. 1-� - ��� �������. 2-� - ��� ��� �����. 
"com" ������������ ��� ������ �����. "decom" ������������ ��� ���������� �����. ������ ����������, ���� ���� ����. 
���� ������������ ��� ������ / ���������� ��������������� �����. ������ �������� �������� � ������ ������.*/

int main(int argc, char* argv[])
{
	argv[1] = "decom";
	argv[2] = "in.key";
	if (strcmp(argv[1], "com") == 0)
	{
		//����������
		FILE* fp = fopen(argv[2], "rb"); //�������� ����
		if (!fp)
		{
			perror("File:");
			exit(1);
		}
		/*printf("compression is started...\r");*/
		int point = findPoint(argv);
		//�������� ���������� ".key\0", �������� ������ 64
		char* name = (char*)malloc((point) * sizeof(char));
		getName(argv, name, point);
		char extension[8] = { 0 };
		getExtension(argv, extension, point);
		struct SYM syms[SIZE] = { 0 };
		struct SYM* psyms[SIZE];
		int sum = read(fp, syms); // ����� ���������� �������� � �����
		fclose(fp);
		if (IfEmpty(sum))
			return 0;
		countFreq(sum, syms);
		qsort(syms, SIZE, sizeof(struct SYM), cmp);
		int count = fillinginpointers(syms, psyms); // ���������� ���������� ��������
		struct SYM* root = buildTree(psyms, count);
		//��������� ����
		makeCodes(root);
		//����������
		FILE* fp_in = fopen(argv[2], "rb"); // �������� ����
		if (!fp_in)
		{
			perror("File:");
			exit(1);
		}
		/*����� ����� ����� 101 ���� �� ������ �����.
         ����� �������� ��� ����� �� ������� �����.
         tail - ���������� ����������� ����� ��� ���������� ���������� �����.
		*/
		int tail = rasmer1byte - ((coder(fp_in, syms, count)) % rasmer1byte);
		fclose(fp_in);
		//PACKER
		FILE* fp_compressed = fopen(strcat(name, ".key"), "wb"); // ������ ����
		if (!fp_compressed)
		{
			perror("File:");
			exit(1);
		}
		writeHeader(fp_compressed, &count, syms, &tail, &sum, extension);
		packer(fp_compressed);
		_fcloseall();
		free(name);
		printf("The file is compressed!Everything is OK!");
	}
	else if (strcmp(argv[1], "decom") == 0)
	{
		FILE* fp_compressed = fopen(argv[2], "rb");
		if (!fp_compressed)
		{
			perror("File:");
			exit(1);
		}
		int point = findPoint(argv);
		char* name = (char*)malloc((point) * sizeof(char));
		newName(argv, name, point);
		char sign[3] = { 0 };
		int count = 0; //���������� ���������� ��������
		struct SYM syms[SIZE] = { 0 };
		int tail = 0;
		int sum = 0; // ������ ��������� �����
		char extension[8] = { 0 };
		//������ ���������
		fread(sign, 1, 3, fp_compressed);
		//�������� ���������
		if (sign[0] != 'k' || sign[1] != 'e' || sign[2] != 'y')
		{
			printf("invalid file!");
			return 0;
		}
		/*printf("decompression is started...\r");*/
		fread(&count, 4, 1, fp_compressed);
		if (count == 1)
		{
			void unkompressor(FILE * fp_compressed, char* name);
			return 0;
		}
		//������ ������� ������
		for (int i = 0; i < count; i++)
		{
			fread(&syms[i].ch, 1, 1, fp_compressed); // ������
			fread(&syms[i].freq, 4, 1, fp_compressed); // �������
		}
		fread(&tail, 1, 1, fp_compressed);
		fread(&sum, 4, 1, fp_compressed);
		for (int i = 0; i < 8; i++)
		{
			fread(&extension[i], 1, 1, fp_compressed);
		}
		struct SYM* psyms[SIZE] = { NULL };
		for (int i = 0; i < count; i++)
			psyms[i] = &syms[i];
		struct SYM* root = buildTree(psyms, count);
		makeDecodes(fp_compressed);
		fclose(fp_compressed);
		FILE* fp_decodes = fopen("101bydecom.txt", "rb"); // 101-����
		if (!fp_decodes)
		{
			perror("File:");
			exit(1);
		}
		FILE* fp_decompressed = fopen(strcat(name, extension), "wb");
		if (!fp_decompressed)
		{
			perror("File:");
			exit(1);
		}
		//��������������� �������� ����
		char symbol = 0;
		for (int i = 0; i < sum; i++)
		{
			symbol = decoder(fp_decodes, root);
			fputc(symbol, fp_decompressed);
		}
		_fcloseall();
		free(name);
		printf("The file is decompressed! Well you were afraid))))");
	}
	else
		printf("You entered something wrong!Enter the command com or decom");
	return 0;
}