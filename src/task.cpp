#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"task.h"
                                   //      COMPRESSOR    

//for  int Analyzer(char* argv, struct SYM* syms, struct SYM** psysms, int* sumCh);
static void initialize(struct SYM* syms); //Initialization of a array SYM by empty contents
static int analysis_fp(struct SYM* syms, FILE* fp); //The analysis of a contained file
static void calculation_freq(struct SYM* syms, int count_simbol); //Calculation of frequency of occurrence of symbols
static void sort(struct SYM* syms); //Sorting of a array SYM
static int fill_psysms(struct SYM* syms, struct SYM** psysms); //Print of symbols and their frequencies of occurrence

//for buildTree(struct SYM* psym[], int N)
static void add_temp(struct SYM* psym[], struct SYM* temp, int N);
static void sort_psym(struct SYM* psym[], int N);

//for void Packing(FILE* fp_101, FILE* fp_finish, int N, struct SYM* syms, int sum_ch, char* argv);
static int lengthTail(FILE* fp_101);
static char* expansionFile(char* argv, char*expansion);
static void makeTabl(FILE* fp_finish, int N, struct SYM* syms);
static unsigned char pack(unsigned char buf[]);

int Analyzer(char* argv, struct SYM* syms, struct SYM** psysms, int* sumCh)
{
	FILE* fp_in;
	fp_in = fopen(argv, "rb");  //"rb"  for Analyzer
	if (!fp_in)
	{
		perror("File :");
		exit(1);
	}

	initialize(syms);
	*sumCh = analysis_fp(syms, fp_in); //for transfer in main() values
	calculation_freq(syms, *sumCh);
	sort(syms);

	if (fclose(fp_in) != 0)
		puts("Error closing the file!");   //"rb"  for Analyzer

	return fill_psysms(syms, psysms);
}
static void initialize(struct SYM* syms)
{
	for (int i = 0; i < SIZE; i++)
	{
		(syms + i)->ch = i;
		(syms + i)->freq = 0;
		(syms + i)->code[0] = '\0';
		(syms + i)->left = NULL;
		(syms + i)->right = NULL;
	}
}
static int analysis_fp(struct SYM* syms, FILE* fp)
{
	int count = 0;
	char ch;

	while ((ch = getc(fp)) != EOF)
	{
		for (int i = 0; i < SIZE; i++)
		{
			if (ch == (syms + i)->ch) // end of line has \r\n.
			{
				(syms + i)->freq++;
				count++;
				continue;
			}
		}
	}

	return count;
}
static void calculation_freq(struct SYM* syms, int count_simbol)
{
	for (int i = 0; i < SIZE; i++)
		(syms + i)->freq = (float)((syms + i)->freq / count_simbol);
}
static void sort(struct SYM* syms)
{
	SYM buf;
	for (int i = 0; i < SIZE - 1; i++)
	for (int j = i + 1; j < SIZE; j++)
	if ((syms + i)->freq < (syms + j)->freq)
	{
		buf.freq = (syms + i)->freq;
		buf.ch = (syms + i)->ch;

		(syms + i)->freq = (syms + j)->freq;
		(syms + i)->ch = (syms + j)->ch;

		(syms + j)->freq = buf.freq;
		(syms + j)->ch = buf.ch;
	}
}
static int fill_psysms(struct SYM* syms, struct SYM** psysms)
{
	int N = 0;
	for (int i = 0; i < SIZE; i++)
	{
		if ((syms + i)->freq != 0)
		{
			*(psysms + i) = (syms + i);
			N++;
		}
	}

	return N;
}

struct SYM* buildTree(struct SYM* psym[], int N)
{
	struct SYM *temp = (struct SYM*)malloc(sizeof(struct SYM));

	temp->freq = psym[N - 2]->freq + psym[N - 1]->freq;
	temp->left = psym[N - 1];
	temp->right = psym[N - 2];
	temp->code[0] = 0;
	temp->ch = '\0';

	if (N == 2)
		return temp; //root our tree

	add_temp(psym, temp, N);
	sort_psym(psym, N - 1);

	return buildTree(psym, N - 1);
}
static void add_temp(struct SYM* psym[], struct SYM* temp, int N)
{
	psym[N - 1] = NULL;
	psym[N - 2] = temp;
}
static void sort_psym(struct SYM* psym[], int N)
{
	SYM* BUF;
	for (int i = 0; i < N - 1; i++)
	for (int j = i + 1; j < N; j++)
	if (psym[i]->freq < psym[j]->freq)
	{
		BUF = psym[i];
		psym[i] = psym[j];
		psym[j] = BUF;
	}
}

void makeCodes(struct SYM* root)
{
	if (root->left)
	{
		strcpy(root->left->code, root->code);
		strcat(root->left->code, "0");
		makeCodes(root->left);
	}

	if (root->right)
	{
		strcpy(root->right->code, root->code);
		strcat(root->right->code, "1");
		makeCodes(root->right);
	}
}
void printTree(struct SYM* root, int level)
{
	int i;
	if (root == NULL)
		return;

	for (i = 0; i < level; i++)
		printf("   ");
	printf("%f  %c  %s\n", root->freq, root->ch, root->code);

	printTree(root->left, level + 1);
	printTree(root->right, level + 1);
}

FILE* CoderLine(struct SYM* syms, int N, FILE*fp_in, FILE* fp_101)
{
	int ch;

	while ((ch = fgetc(fp_in)) != -1)
	{
		for (int i = 0; i < N; i++)
		if (syms[i].ch == (unsigned char)ch)
		{
			fputs(syms[i].code, fp_101);
			break;
		}
	}

	return fp_101;
}

void Packing(FILE* fp_101, FILE* fp_finish, int N, struct SYM* syms, int sum_ch, char* argv)
{
	unsigned char buf[BIT] = { '0' };
	char ch;
	unsigned char CH = '0'; //simbol for fp_finish
	int zero_for_byte = BIT;
	int i;

	for (i = 0; (ch = getc(fp_101)) != EOF; i++)
	{
		buf[i] = ch;
		--zero_for_byte;

		if (i == BIT - 1) // [0] [1] [2] [3] [4] [5] [6] [7]
		{
			CH = pack(buf);
			fputc(CH, fp_finish);
			i = -1;
			zero_for_byte = BIT;
		}
	}

	//for the last simbol
	if (zero_for_byte < BIT)
	{
		for (int j = i; j < BIT; j++)
			buf[j] = '0';
		CH = pack(buf);
		fputc(CH, fp_finish);
	}
}
int makeFirstLine(FILE* fp_101, FILE* fp_finish, int N, struct SYM* syms, int sum_ch, char* argv)
{
	fputs(SIGNATURA, fp_finish);

	fwrite(&N, sizeof(int), 1, fp_finish);

	makeTabl(fp_finish, N, syms);   //for tabl with simbol and his freq

	int zero_for_byte = lengthTail(fp_101);
	fwrite(&zero_for_byte, sizeof(int), 1, fp_finish);//tail with '0'

	fwrite(&sum_ch, sizeof(int), 1, fp_finish);

	char expansion[SIZE];
	expansionFile(argv, expansion); //format of the first file 
	int simbol_format = strlen(expansion);
	fwrite(&expansion, sizeof(char), simbol_format, fp_finish);

	return simbol_format;
}
static int lengthTail(FILE* fp_101)
{
	char ch;
	int count = 0;
	while ((ch = getc(fp_101)) != EOF)
		count++;

	return BIT - (count % BIT);
}
static char* expansionFile(char* argv, char* expansion)
{
	char ch;
	int i = 0, j = 0; //for copy expansion past '.'

	while (argv[i]) //!='\0'
	{
		if (j != 0)
		{
			expansion[j] = argv[i + 1];  //+'\0'
			j++;
		}

		if (argv[i] == '.' && j == 0)
		{
			expansion[j] = argv[i + 1];
			j++;
		}
		i++;
	}

	return expansion;
}
static void makeTabl(FILE* fp_finish, int N, struct SYM* syms)
{
	for (int i = 0; i < N; i++)
	{
		fwrite(&((syms + i)->ch), sizeof(unsigned char), 1, fp_finish);
		fwrite(&((syms + i)->freq), sizeof(float), 1, fp_finish);
	}
}
static unsigned char pack(unsigned char buf[])
{
	union CODE code;
	code.byte.b1 = buf[0] - '0';
	code.byte.b2 = buf[1] - '0';
	code.byte.b3 = buf[2] - '0';
	code.byte.b4 = buf[3] - '0';
	code.byte.b5 = buf[4] - '0';
	code.byte.b6 = buf[5] - '0';
	code.byte.b7 = buf[6] - '0';
	code.byte.b8 = buf[7] - '0';

	return code.ch;
}

void deleteAll(struct SYM* root)
{
	if (root->left)
		deleteAll(root->left);

	if (root->right)
		deleteAll(root->right);

	free(root);
}

                                      //    DE - COMPRESSOR  

//for void UnPacking(FILE* fp_finish, FILE* fp_FENIX, int simbol_format)
static int whoseFile(FILE* fp_finish);
static unsigned char* dePack(unsigned char CH, unsigned char* de_buf);
static void makeDeFile101(unsigned char* de_buf, FILE* fp_DE101);
static FILE* clearTail(FILE* fp_DE101, int zero_tial);
static int makeFENIXfile(FILE* fp_DE101, FILE* fp_FENIX, int type, struct SYM* de_sysms);

void UnPacking(FILE* fp_finish, FILE* fp_FENIX, int simbol_format)
{
	struct SYM* de_sysms;
	struct SYM** de_psysms;
	struct SYM* de_root;
	FILE* fp_DE101;

	if (whoseFile(fp_finish))
	{
		puts("This file can not to de UNPACKING!");
		exit(1);
	}

	int type;
	fread(&type, sizeof(int), 1, fp_finish);

	de_sysms = (struct SYM*)malloc(type*sizeof(struct SYM*));
	if (de_sysms == NULL)
	{
		puts("No memory allocated! The program completed.");
		exit(1);
	}

	de_psysms = (struct SYM**)malloc(type*sizeof(struct SYM**));
	if (de_psysms == NULL)
	{
		puts("No memory allocated! The program completed.");
		exit(1);
	}

	unsigned char buf_ch = '0';
	float buf_freq = 0.0;

	for (int i = 0; i < type; i++) //make de_sysms and de_psysms
	{
		fread(&buf_ch, sizeof(unsigned char), 1, fp_finish);
		fread(&buf_freq, sizeof(float), 1, fp_finish);

		(de_sysms + i)->ch = buf_ch;
		(de_sysms + i)->freq = buf_freq;
		(de_sysms + i)->code[0] = '\0';
		(de_sysms + i)->left = NULL;
		(de_sysms + i)->right = NULL;

		*(de_psysms + i) = (de_sysms + i);
		buf_ch = '0';
		buf_freq = 0.0;
	}

	de_root = buildTree(de_psysms, type);  //DE_TREE
	makeCodes(de_root);
	//printTree(de_root, 0);//if the user will want to see a de-tree	

	int zero_tial;
	fread(&zero_tial, sizeof(int), 1, fp_finish);

	int size_file;
	fread(&size_file, sizeof(int), 1, fp_finish);

	char format[BIT];
	fread(&format, sizeof(char), simbol_format, fp_finish); //format of our file
	format[simbol_format] = '\0';


	char CH;
	unsigned char de_buf[BIT];

	fp_DE101 = fopen("DE101.txt", "ab");  //"ab" for make a file
	if (!fp_DE101)
	{
		perror("File :");
		exit(1);
	}

	while ((CH = getc(fp_finish)) != EOF)
	{
		dePack(CH, de_buf);          //DE-CODE
		makeDeFile101(de_buf, fp_DE101); //Writing in file DE101.txt
	}

	if (fclose(fp_DE101) != 0)
		puts("Error closing the file!");

	clearTail(fp_DE101, zero_tial);

	if (size_file == makeFENIXfile(fp_DE101, fp_FENIX, type, de_sysms))  //fp_DE101 in fp_FENIX  
		puts("The FENIX_file is READY!!!");
	else
		puts("Error of DeCOMPRESSOR!");

	free(de_psysms);
	free(de_sysms);
}

static int whoseFile(FILE* fp_finish)
{
	char buf[6] = { '0' };

	for (int i = 0; i < 5; i++)
		buf[i] = getc(fp_finish);
	buf[5] = '\0';

	return (strcmp(SIGNATURA, buf) == 0) ? 0 : 1;
}
static unsigned char* dePack(unsigned char CH, unsigned char* de_buf)
{
	union CODE de_code;
	de_code.ch = CH;

	de_buf[0] = de_code.byte.b1 + '0';
	de_buf[1] = de_code.byte.b2 + '0';
	de_buf[2] = de_code.byte.b3 + '0';
	de_buf[3] = de_code.byte.b4 + '0';
	de_buf[4] = de_code.byte.b5 + '0';
	de_buf[5] = de_code.byte.b6 + '0';
	de_buf[6] = de_code.byte.b7 + '0';
	de_buf[7] = de_code.byte.b8 + '0';

	return de_buf;
}
static void makeDeFile101(unsigned char* de_buf, FILE* fp_DE101)
{
	for (int i = 0; i < BIT; i++)
		fputc(de_buf[i], fp_DE101);
}
static FILE* clearTail(FILE* fp_DE101, int zero_tial)
{
	char ch_end = EOF;
	long end_DE101;  //for clear our tail: replacement '0' on 'EOF', but in file I have 'ÿ'

	fp_DE101 = fopen("DE101.txt", "rb+");  //"rb+" for editing the file
	if (!fp_DE101)
	{
		perror("File :");
		exit(1);
	}

	fseek(fp_DE101, 0L, SEEK_END);  //end of file

	for (int i = 0, end_DE101 = ftell(fp_DE101); i <= zero_tial; i++, end_DE101--)
	{
		fseek(fp_DE101, end_DE101, SEEK_SET);
		fputc(ch_end, fp_DE101);
	}
	if (fclose(fp_DE101) != 0)
		puts("Error closing the file!");

	return fp_DE101;
}

static int makeFENIXfile(FILE* fp_DE101, FILE* fp_FENIX, int type, struct SYM* de_sysms)
{
	fp_DE101 = fopen("DE101.txt", "rb");
	if (!fp_DE101)
	{
		perror("File :");
		exit(1);
	}

	int simbol = 0; //count unpacked simbols
	char ch;
	char buf_code[BIT];

	for (int i = 0; i < BIT; i++)
	{
		buf_code[i] = getc(fp_DE101);
		if (buf_code[i] == EOF)
			return simbol;
		buf_code[i + 1] = '\0';

		for (int j = 0; j < type; j++)
		{
			if (strcmp(buf_code, (de_sysms + j)->code) == 0)
			{
				fputc((de_sysms + j)->ch, fp_FENIX);
				simbol++;
				i = -1;   //becouse in for i++
				j = -1;   //becouse in for j++
				break;
			}
		}
	}

	if (fclose(fp_DE101) != 0)
		puts("Error closing the file!");

	return simbol;
}