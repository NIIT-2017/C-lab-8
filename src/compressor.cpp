#include "compr.h"

int main(int argc, char* argv[])
{
	FILE *fp_in; // original file
	FILE *fp_101; // 101-file
	FILE *fp_out; //compressed file
	SYM syms[N] = { 0 }; // array of structures for writing characters
	PSYM psym[N] = { 0 };

	unsigned int count = 0;  // total characters
	int uniqueCount = 0;  // unique characters
	int tail = 0; // tail size
	unsigned char ch = 0;
	unsigned char buf[8] = { 0 };
	int point = 0;
	unsigned char temp = 0;
	unsigned char num = 0;

	if (argc < 2)
	{
		printf("Enter file name, then enter \"cmp\" for compress or enter \"decmp\" for decompress");
		exit(EXIT_FAILURE);
	}

	if ((fp_in = fopen(argv[1], "rb")) == NULL)
	{
		puts("File not found!");
		exit(EXIT_FAILURE);
	}


	while (fgetc(fp_in) != EOF)
		count++;

	rewind(fp_in);

	for (unsigned int i = 0;i < count;i++)
	{
		ch = fgetc(fp_in);
		createStruct(syms, ch, count, &uniqueCount);
	}

	printf("Array of structures was created!\n");
	sortStruct(syms, uniqueCount);
	printStructFriq(syms, uniqueCount);
	printf("Array of structures was sorted!\n");

	for (int i = 0; i < uniqueCount; i++)
	{
		psym[i] = &syms[i];
	}

	PSYM root = buildTree(psym, uniqueCount);
	makeCodes(root);
	printf("Tree was built!\n");
	printStruct(syms, uniqueCount);

	int ch1; // код символа из файла

	if ((fp_101 = fopen("101.txt", "wb")) == NULL)
	{
		puts("File not found!");
		exit(EXIT_FAILURE);
	}

	rewind(fp_in);

	unsigned int count101 = 0;  //number of characters in the file fp_101
	while ((ch1 = fgetc(fp_in)) != -1)
	{
		for (int i = 0;i < uniqueCount;i++)
			if (syms[i].ch == (unsigned char)ch1)
			{
				fputs(syms[i].code, fp_101); // выводим строку с кодом
				count101 += strlen(syms[i].code);
				break; // прерываем поиск
			}
	}

	tail = 8 - count101 % 8;
	for (int i = 0;i < tail;i++)
		fputc('0', fp_101);

	fclose(fp_in);
	fclose(fp_101);

	printf("File 101 was created!\n");

	fp_out = fopen("out.abc", "wb");
	fp_101 = fopen("101.txt", "rb");

	if (fp_101 == NULL)
	{
		puts("File not found!");
		exit(EXIT_FAILURE);
	}
	if (fp_out == NULL)
	{
		puts("File not found!");
		exit(EXIT_FAILURE);
	}

	//headline
	num = uniqueCount - 1; //число уникальных символов, придет из анализатора, -1 чтоб убраться в 1 байт см практику
	fwrite("abc", sizeof(char), 3, fp_out);// пишем заголовок для файла
	fwrite(&num, sizeof(char), 1, fp_out);
	for (int i = 0;i < uniqueCount;i++)
	{
		fwrite(&syms[i].ch, 1, 1, fp_out);
		fwrite(&syms[i].freq, 4, 1, fp_out);
	}
	fwrite(&tail, 1, 1, fp_out);

	char str[5] = { 0 };
	int k = 0, j = 0;

	//read file extension
	for (k = (strlen(argv[1])) - 3;argv[1][k] != '\0';k++, j++)
		str[j] = argv[1][k];
	str[j] = '\0';
	//printf("DOT %s\n",str);

	fwrite(str, sizeof(char), 3, fp_out);
	// headline end

	while (fread(buf, 8, 1, fp_101))
	{
		temp = pack(buf);
		fwrite(&temp, 1, 1, fp_out);
	}

	fclose(fp_out);
	fclose(fp_101);
	return 0;
}
