#include <stdio.h>
#include "task2.h"

int main(int argc, char* argv[])
{

	FILE* fp;
	fp = fopen(argv[1], "rb");
	struct SYM syms[LEN];
	struct SYM* psyms[MAX_LEN]; 
	unsigned char buf1[3];
	unsigned char chns;
	
	fread(buf1, 2, 1, fp);
	fread(&chns, sizeof(unsigned char), 1, fp);
	buf1[2] = '\0';
	int ns = (int)chns;

	for (int i = 0; i < ns; i++)
	{
		fread(&(syms[i].ch), sizeof(unsigned char), 1, fp);
		fread(&(syms[i].freq), sizeof(double), 1, fp);
		syms[i].code[0] = 0;
		syms[i].left = NULL;
		syms[i].right = NULL;
		psyms[i] = syms + i;
	}
	
	psyms[ns + 1] = NULL;
	unsigned char chtail;
	fread(&chtail, sizeof(unsigned char), 1, fp);
	int tail = (int)chtail;
	int size_original_file = 0;
	fread(&size_original_file, sizeof(int), 1, fp);
	unsigned char ext[4];
	fread(ext, 3, 1, fp);
	ext[3] = '\0';
	struct SYM* root = (struct SYM*)malloc(sizeof(struct SYM));
	root = buildTree(psyms, ns); 
	makeCodes(root);
	FILE* fp_101;
	fp_101 = fopen("fp_101.txt", "wb");
	unsigned char ch;
	unsigned char buf[9];
	int count = 0;
	int c = 0;

	while (fread(&ch, sizeof(unsigned char), 1, fp))
	{
		unpack(ch, buf);
		fwrite(buf, 8, 1, fp_101);
		count += 8;
	}
	count = count - 8 + tail; 
	fclose(fp);
	fclose(fp_101);
	fp_101 = fopen("fp_101.txt", "rb");
	FILE* fres;
	fres = fopen("result.txt", "wb");
	int i = 0;
	char code[LEN];
	int lenc = 0;
	unsigned char temp;

	while (fread(&ch, sizeof(unsigned char), 1, fp) && i<count)
	{
		code[lenc] = ch;
		lenc++;
		code[lenc] = '\0';
		temp= searchTree(root, code);
		if (temp != 0)
		{
			fwrite(&temp, 1, 1, fres);
			lenc = 0;
			code[0] = '\0';
		}
		i++;
	}

	int size_result_file;
	size_result_file = ftell(fres);

	if (size_original_file - size_result_file != 0)
		printf("The size of the source file is %d, and the size of the file with the result is %d!\n", size_original_file, size_result_file);

	fclose(fres);
	return 0;
}