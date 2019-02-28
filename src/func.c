#include"compress.h"

void path(char * pathIn, char * pathOut, char * expansion)
{
	int length = strlen(pathIn);
	int i = length; int j = 0;
	while (pathIn[i] != '.')
		i--, j++;
	for (int i = 0; i < j; i++)
		expansion[i] = pathIn[length - j+1 + i];
	for (int i = 0; i <= length - j; i++)
		pathOut[i] = pathIn[i];
	strcat(pathOut, "abc");
}

unsigned long getArr(const char * fileName, int * arr)
{
	long count = 0;
	FILE *pf_in;
	pf_in = fopen(fileName, "rb");
	if (pf_in == NULL) { printf("incoming file open read error\n"); exit(EXIT_FAILURE); }
	int i = 0;
	while ((i = fgetc(pf_in)) != EOF)
		arr[i]++, count++;
	fclose(pf_in);
	return count;
}

int compare(const void * pArr1, const void * pArr2)
{
	if (((struct SYM *)pArr1)->freq < ((struct SYM *)pArr2)->freq)
		return 1;
	if (((struct SYM *)pArr1)->freq > ((struct SYM *)pArr2)->freq)
		return -1;
	if (((struct SYM *)pArr1)->freq == ((struct SYM *)pArr2)->freq)
		return 0;
}

int arr2SYM(int * arr, unsigned long count, struct SYM ** pSYM)
{
	int size = 0;
	for (int i = 0; i < 256; i++)		
		arr[i] ? size++ : 0;
								
	*pSYM = (struct SYM *) malloc(size * sizeof(struct SYM));

	for (int i = 0, j = 0; i < 256; i++)
	{
		if (arr[i])
		{
			(*pSYM)[j].ch = i;
			(*pSYM)[j].freq = (float)arr[i] / count;
			(*pSYM)[j].code[0] = "";
			(*pSYM)[j].left = NULL;
			(*pSYM)[j].right = NULL;
			j++;
		}
	}
	qsort(*pSYM, size, sizeof(struct SYM), compare);
	return size;
}

struct SYM* buildTree(struct SYM *psym[], int N)
{
	struct SYM * temp = (struct SYM*)malloc(sizeof(struct SYM));
	temp->freq = psym[N - 2]->freq + psym[N - 1]->freq;
	temp->left = psym[N - 1];
	temp->right = psym[N - 2];
	temp->code[0] = '\0';
	if (N == 2)
		return temp;

	int i = N - 3;
	while (i >= 0 && (temp->freq >= psym[i]->freq))
	{
		psym[i + 1] = psym[i];
		i--;
	}
	psym[i + 1] = temp;
	return buildTree(psym, N - 1);
}

void makeCodes(struct SYM *root)
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

int make101File(const char * fileName, struct SYM ** pSYM, int size)
{
	FILE *pf_in;
	pf_in = fopen(fileName, "rb");
	if (pf_in == NULL) { printf("incoming file open error\n"); exit(EXIT_FAILURE); }
	
	FILE *pf_101;
	pf_101 = fopen("101.txt", "wb");
	if (pf_101 == NULL) { printf("101.txt file open error\n"); exit(EXIT_FAILURE); }
	
	int ch;
	unsigned long count = 0;
	while ((ch = fgetc(pf_in)) != -1)
	{
		for (int i = 0; i < size; i++)
			if ((*pSYM)[i].ch == (unsigned char)ch)
			{
				fputs( (*pSYM)[i].code, pf_101 );
				count += strlen( (*pSYM)[i].code );
				break;
			}
	}
	count = 8 - count % 8;
	count == 8 ? count = 0 : 1;
	  for (int i = 0; i < count; i++)
		fputc('0', pf_101);
	fclose(pf_in);
	fclose(pf_101);
	return count;
}

unsigned char pack(unsigned char buf[])
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

void makeOutFile(const char * fileOutName, const char * expansion, int size, struct SYM ** pSYM, int bits2Bite)
{
	FILE *pf_101;
	pf_101 = fopen("101.txt", "rb");
	if (pf_101 == NULL) { printf("101 file open read error\n"); exit(EXIT_FAILURE); }

	FILE *pf_out;
	pf_out = fopen(fileOutName, "wb");
	if (pf_out == NULL) { printf("outcoming file open write error\n"); exit(EXIT_FAILURE); }

	fwrite("abc", 3, 1, pf_out);
	char temp = (char)(size - 1);//"number of unique characters -1"
	fwrite(&temp, 1, 1, pf_out);

	for (int i = 0; i < size; i++)
	{
		fwrite(&(*pSYM)[i].ch, 1, 1, pf_out);
		fwrite(&(*pSYM)[i].freq, 4, 1, pf_out);
	}
	fwrite(&bits2Bite, 1, 1, pf_out);
	fwrite(expansion, 5, 1, pf_out);

	char ch = 0;
	unsigned char buf[8];
	while (fread(buf, 8, 1, pf_101))
	{
		ch = pack(buf);
		fwrite(&ch, 1, 1, pf_out);
	}

	fclose(pf_out);
	fclose(pf_101);
}
