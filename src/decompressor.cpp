#include "decompr.h"

int main(int argc, char* argv[])
{
	FILE *fp_in; // compressed file 
	FILE *fp_101; // 101-file
	FILE *fp_out; // original file
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

	char arr[10] = { 0 };

	if ((fp_in = fopen(argv[1], "rb")) == NULL)
	{
		puts("File not found!");
		exit(EXIT_FAILURE);
	}
	//read headline
	fread(arr, 3, 1, fp_in);
	if (strcmp("abc", arr) != 0)
	{
		printf("File cannot be unpacked!");
		exit(EXIT_FAILURE);
	}
	fread(&num, 1, 1, fp_in);
	uniqueCount = num + 1;
	for (int i = 0; i < uniqueCount; i++)
	{
		fread(&syms[i].ch, 1, 1, fp_in);
		fread(&syms[i].freq, 4, 1, fp_in);
	}
	fread(&tail, 4, 1, fp_in);
	fread(arr, 3, 1, fp_in);
	arr[3] = '\0';
	//end read headline
	printf("Headline was read!\n");

	for (int i = 0; i < uniqueCount; i++)
	{
		psym[i] = &syms[i];
	}
	PSYM root = buildTree(psym, uniqueCount);  // make tree
	makeCodes(root);
	printf("Tree was built!\n");
	printStruct(syms, uniqueCount);

	if ((fp_101 = fopen("101_out.txt", "wb")) == NULL)
	{
		puts("File not found!");
		exit(EXIT_FAILURE);
	}

	while (fread(&temp, 1, 1, fp_in))
	{
		unPack(buf, temp);
		fwrite(buf, 8, 1, fp_101);
	}
	printf("File 101 was written!\n");
	fseek(fp_101, -tail, SEEK_END);
	int size = 0;
	size = ftell(fp_101);

	fclose(fp_101);
	char name[20] = { 0 };

	for (int i = 0; argv[1][i] != '\0'; i++)
	{
		name[i] = argv[1][i];
		if (name[i] == '.')
		{
			name[i + 1] = '\0';
			break;
		}
	}
	strcat(name, arr);

	decoding("101_out.txt", name, root, size); //decoding
	printf("File 101 was decoded!\n");
	fclose(fp_in);
	printf("File was successfully decompressed!\n");

	return 0;
}
	