#include <stdio.h>
#include "task1.h"

int main(int argc, char* argv[])
{

	struct SYM syms[LEN];
	struct SYM* psyms[MAX_LEN]; 
	char line[MAX_LEN];

	FILE* fp_in;
	//fp_in = fopen("WarAndPeace.txt", "rb");
	fp_in = fopen(argv[1], "rb");

	int count = 0;
	int i, j;
	int ns = 0;
	int found;
	
	while (fgets(line, 256, fp_in) != NULL)
	{
		chomp(line);
		i = 0;
		while (line[i])
		{
			count++;
			found = 0;
			for (j = 0; j < ns; j++)
			{
				if ((unsigned char)line[i] == syms[j].ch)
				{
					syms[j].freq++;
					found = 1;
					break;
				}
			}
			if (found == 0)
			{
				
				psyms[ns] = (struct SYM*)malloc(sizeof(struct SYM));
				syms[ns].ch = line[i];
				syms[ns].freq = 1;
				syms[ns].code[0] = 0;
				syms[ns].left = NULL;
				syms[ns].right = NULL;
				psyms[ns] = syms + ns;
				ns++;

			}
			i++;
		}
	}
	psyms[ns] = NULL;
	int size_of_file;
	size_of_file = ftell(fp_in);  
	for (i = 0; i < ns; i++)
		syms[i].freq = syms[i].freq / count;
	qsort(syms, ns, sizeof(syms[0]), cmp); 
	struct SYM* root = (struct SYM*)malloc(sizeof(struct SYM));
	root = buildTree(psyms, ns); 
	makeCodes(root);
	FILE* fp_101;
	fp_101 = fopen("fp_101.txt", "wb");
	rewind(fp_in);
	char ch;
	while ((ch = fgetc(fp_in)) != -1)
	{
		for (i = 0; i < count; i++)
			if (syms[i].ch == (unsigned char)ch)
			{
				fputs(syms[i].code, fp_101);
				break;
			}
	}
	fclose(fp_101);
	fp_101 = fopen("fp_101.txt", "rb");
	int k = 0;
	unsigned char buf[9];
	unsigned char chpack;

	//1. Сигнатура (подпись формата)
	//2. Количество уникальных символов (число строк в таблице встречаемости)
	//3. Сама таблица встречаемости (пары: код символа - частота)
	//4. Длина хвоста
	//5. Размер исходного файла
	//6. Расширение исходного файла (для восстановления)
	FILE* fp_result;
	fp_result = fopen("result.txt", "wb");
	if (fp_result == NULL)
	{
		puts("File result open error!");
		return 1;
	}

	unsigned char buf1[3] = "dk"; 
	fwrite(buf1, 2, 1, fp_result);
	fwrite(&ns, sizeof(unsigned char), 1, fp_result);
	for (i = 0; i < ns; i++)
	{
		fwrite(&syms[i].ch, sizeof(unsigned char), 1, fp_result);
		fwrite(&(syms[i].freq), sizeof(double), 1, fp_result);
	}
	//для заголовка
	k = 0;
	while ((ch = fgetc(fp_101)) != -1)
		k++;
	unsigned char tail = k % 8;
	fwrite(&tail, sizeof(unsigned char), 1, fp_result);
	fwrite(&size_of_file, sizeof(int), 1, fp_result);
	unsigned char ext[4] = "txt";
	fwrite(ext, 3, 1, fp_result);
	k = 0;
	rewind(fp_101);
	while ((ch = fgetc(fp_101)) != EOF)
	{
		if (k<8)
		{
			buf[k] = (unsigned char)ch;
			k++;
		}
		else
		{
			buf[k] = '\0';
			chpack = pack(buf);
			fputc(chpack, fp_result);
			k = 0;
			buf[k] = (unsigned char)ch;
			k++;
		}
	}
	for (i = k + 1; i < 8; i++)
		buf[i] = '0';
	buf[8] = '\0';
	chpack = pack(buf);
	fputc(chpack, fp_result);
	fclose(fp_result);

	return 0;
}