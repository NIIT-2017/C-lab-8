#include "task1.h"

int main(int argc, char* argv[])
{
	FILE *fp_in, *fp_out, *fp_101;
	SYM syms[M] = { NULL }; // массив структур
	PSYM psyms[M] = { NULL }; // массив указателей на структуры
	ULL ch_count = 0; // счетчик символов в иходном файле
	UI ch_count101 = 0; // счетчик символов в .101 файле
	UI ch_tail = 0; // счетчик нулей в хвосте .101 файла
	UI ch_uniq = 0; // счетчик уникальных символов
	unsigned char ch;
	unsigned char buf[8] = { 0 };
	int q = 0;

	if (argc != 3)
	{
		printf("Please, use %s FILE_NAME\n + \"C\" for compression\n + \"D\" for compression (not available in beta)\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	if (strcmp(argv[2], "c") == 0 || strcmp(argv[2], "C") == 0)
	{
		fp_in = fopen(argv[1], "rb");
		if (fp_in == NULL)
		{
			printf("File not found!\n");
			exit(EXIT_FAILURE);
		}

		while (fgetc(fp_in) != EOF)
			ch_count++;

		rewind(fp_in); 

		for(int i=0; i < ch_count; i++)
		{
			ch = fgetc(fp_in);
			ListCreator(syms, ch, ch_count, &ch_uniq);
		}

		sortList(syms, ch_uniq); 
	
		for (int i = 0; i < ch_uniq; i++)
			psyms[i] = &syms[i];

		struct SYM* root = buildTree(psyms, ch_uniq);
		makeCodes(root);
	
		fp_101 = fopen("binary.101", "wb");
		if (fp_101 == NULL)
		{
			puts("File \"binary.101\" not found!\n");
			exit(EXIT_FAILURE);
		}
		rewind(fp_in);
		int end_file = 0;
		while ((end_file = fgetc(fp_in)) != -1)
		{
			for (int i = 0; i < ch_uniq; i++)
				if (syms[i].ch == (unsigned char)ch_count101)
				{
					fputs(syms[i].code, fp_101);
					ch_count101 += strlen(syms[i].code);
					break;
				}
		}

		if (ch_count101 % 8 != 0)
			for (int i = 0; i < (8 - ch_count101 % 8); i++)
				fputc('0', fp_101);
		fclose(fp_101);

		if ((fp_101 = fopen("binary.101", "rb")) == NULL)
		{
			puts("File not found!");
			exit(EXIT_FAILURE);
		} 

		if ((fp_out = fopen("compressed.cmp", "wb")) == NULL)
		{
			puts("File not found!");
			exit(EXIT_FAILURE);
		}

		fwrite("cmp", 3, 1, fp_out);

		unsigned char temp = (unsigned char)(ch_uniq - 1);
		fwrite(&temp, 1, 1, fp_out);

		for (int i = 0; i < ch_uniq; i++)
		{
			fwrite(&syms[i].ch, 1, 1, fp_out);
			fwrite(&syms[i].freq, 4, 1, fp_out);
		}
		fwrite(&ch_tail, 1, 1, fp_out);

		char arr[5] = { 0 };
		int i = (strlen(argv[1])) - 3;
		for (i , q = 0; argv[1][i] != '\0'; i++, q++)
			arr[q] = argv[1][i];
		arr[q] = '\0';
		fwrite(arr, 3, 1, fp_out);

		while (fread(buf, 8, 1, fp_101))
		{
			temp = pack(buf);
			fwrite(&temp, 1, 1, fp_out);
		}
		
		fclose(fp_out);
		fclose(fp_in);
		fclose(fp_101);
		printf("Succes!!!\n");
	}
	
	else if (strcmp(argv[2], "d") == 0 || strcmp(argv[2], "D") == 0)
		printf("Sorry, this feature is not available in beta version.\n");

	else
		printf("Unknown argument");
	return 0;
}