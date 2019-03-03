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
	unsigned char temp = 0;
	unsigned char num = 0;

	if (argc < 2)
	{
		printf("You did not enter file name");
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

	int ch1; // character code from file

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
				fputs(syms[i].code, fp_101); // write code line in the 101-file 
				count101 += strlen(syms[i].code);
				break; // interrupt search
			}
	}

	tail = 8 - count101 % 8;
	for (int i = 0;i < tail;i++) // add tail
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

	//write headline
	num = uniqueCount - 1; //number of unique characters minus 1 (to get out in 1 byte)
	fwrite("abc", 3, 1, fp_out);// unique abbreviation
	fwrite(&num, 1, 1, fp_out);
	for (int i = 0;i < uniqueCount;i++) // symbol and frequency table
	{
		fwrite(&syms[i].ch, 1, 1, fp_out);
		fwrite(&syms[i].freq, 4, 1, fp_out);
	}
	fwrite(&tail, 4, 1, fp_out); // tail

	char str[5] = { 0 }; //read file extension
	int k = 0, j = 0;
	for (k = (strlen(argv[1])) - 3;argv[1][k] != '\0';k++, j++) 
		str[j] = argv[1][k];
	str[j] = '\0';

	fwrite(str, 3, 1, fp_out); // file extension
	// headline end

	while (fread(buf, 8, 1, fp_101))  //packing of 8 characters 
	{
		temp = pack(buf);
		fwrite(&temp, 1, 1, fp_out);
	}

	fclose(fp_out);
	fclose(fp_101);
	return 0;
}
