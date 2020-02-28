#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// COMPRESSING

void makeFileName(char * filenameIn, char * name, char * filename101, char * extension)
{
    for (int i = 0; i < strlen(filenameIn); i++)
    {
        if (filenameIn[i] == '.')
        {
            name[i] = '\0';
            int j = 0;
            while (filenameIn[++i] != '\0')
            {
                extension[j] = filenameIn[i];
                j++;
            }
            extension[j] = '\0';
            break;
        }
        else
            name[i] = filenameIn[i];
    }
    strcpy(filename101, name);
    strcat(filename101, ".101");
    filename101[strlen(filenameIn) + strlen(".101")] = '\0';
}

void initSyms(TSYM * psym[], TSYM syms[SIZE], int * uCount, char * filenameIn)
{
    FILE * fp_in = fopen(filenameIn, "rb");
    if (fp_in == NULL)
    {
        printf("Input file open error!\n");
        return;
    }
    int sym;
    long total = 0;
    long count[SIZE] = { 0 };
    float countFreq = 0;

    while ((sym = fgetc(fp_in)) != EOF)
    {
        count[sym]++;
        total++;
    }
    if (total == 0)
    {
        printf("Unable to compress. File is empty!\n");
        exit(1);
    }
    for (int i = 0; i < SIZE; i++)
    {
        if ((syms[i].freq = (float)count[i] / total) > 0.000000001)
        {
            syms[i].ch = (unsigned char)i;
            syms[i].code[0] = (char)"";
            syms[i].left = NULL;
            syms[i].right = NULL;
            (*uCount)++;
        }
    }
    if ((*uCount) == 1)
    {
        printf("Unable to compress. File consists of only one unique symbol.\n");
        exit(1);
    }
    for (int i = 0; i < (*uCount); i++)
        psym[i] = &syms[i];
    fclose(fp_in);
}

void sortSyms(TSYM syms[SIZE])
{
    TSYM tmp;
    for (int i = 0; i < SIZE; i++)
    {
        int j = i;
        while (j > 0 && syms[j - 1].freq < syms[j].freq)
        {
            tmp = syms[j - 1];
            syms[j - 1] = syms[j];
            syms[j] = tmp;
            j--;
        }
    }
}

TSYM * buildTree(TSYM * psym[], int N)
{
    TSYM * temp = (TSYM *)malloc(sizeof(TSYM));
    temp->freq = psym[N - 2]->freq + psym[N - 1]->freq;
    temp->left = psym[N - 1];
    temp->right = psym[N - 2];
    temp->code[0] = '\0';
    if (N == 2)
        return temp;
    else
    {
        for (int i = 0; i < N; i++)
        {
            if (temp->freq > psym[i]->freq)
            {
                for (int j = N - 1; j > i; j--)
                    psym[j] = psym[j - 1];
                psym[i] = temp;
                break;
            }
        }
    }
    return buildTree(psym, N - 1);
}

void makeCodes(TSYM * root)
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

void coding(TSYM syms[SIZE], int uCount, long *sizeIn, long * size101, int * tail, char * filenameIn, char * filename101)
{
    FILE * fp_in = fopen(filenameIn, "rb");
    if (fp_in == NULL)
    {
        printf("Input file open error!\n");
        return;
    }
    FILE * fp_101 = fopen(filename101, "wb");
    int ch;
    while ((ch = fgetc(fp_in)) != -1)
    {
        for (int i = 0; i < uCount; i++)
        {
            if (syms[i].ch == (unsigned char)ch)
            {
                fputs(syms[i].code, fp_101);
                (*size101) += (long)strlen(syms[i].code);
                break;
            }
        }
        (*sizeIn)++;
    }
    *tail = *size101 % 8;
    fclose(fp_in);
    fclose(fp_101);
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

void fillOutputFile(TSYM syms[SIZE], int uCount, long sizeIn, long size101, int tail, char * filename101, char * filenameOut, char * extension, char * name)
{
    int ch;
    unsigned char buf[8] = { 0 };
    strcpy(filenameOut, name);
    strcat(filenameOut, ".hfm");
    FILE * fp_101 = fopen(filename101, "rb");
    if (fp_101 == NULL)
    {
        printf(".101 file open error!\n");
        return;
    }
    FILE * fp_out = fopen(filenameOut, "wb");
    fwrite("HFMN", sizeof(unsigned char), 4, fp_out);
    fwrite(&uCount, sizeof(int), 1, fp_out);
    for (int i = 0; i < uCount; i++)
    {
        fwrite(&syms[i].ch, sizeof(unsigned char), 1, fp_out);
        fwrite(&syms[i].freq, sizeof(float), 1, fp_out);
    }
    fwrite(&tail, sizeof(int), 1, fp_out);
    fwrite(&sizeIn, sizeof(long), 1, fp_out);
    fwrite(&size101, sizeof(long), 1, fp_out);
    fwrite(extension, sizeof(char), 3, fp_out);

    int i = 0;
    long j = 0;
    while ((ch = fgetc(fp_101)) != -1)
    {
        j++;
        if (j <= size101 - (long)tail)
        {
            buf[i] = (unsigned char)ch;
            if (i == 7)
            {
                fputc(pack(buf), fp_out);
                i = 0;
                continue;
            }
            i++;
        }
        else
        {
            if (tail != 0)
            {
                buf[i] = (unsigned char)ch;
                if (j == size101)
                {
                    for (int k = i + 1; k < 8; k++)
                        buf[k] = '0';
                    fputc(pack(buf), fp_out);
                    break;
                }
                i++;
            }
        }
    }
    fclose(fp_101);
    fclose(fp_out);
}

// DECOMPRESSING

void initSymsD(TSYM * psym[], TSYM syms[SIZE], int * uCount, int * tail, long * sizeIn, long * size101, long * position, char * extension, char * filenameIn)
{
    FILE * fp_inD = fopen(filenameIn, "rb");
    if (fp_inD == NULL)
    {
        printf("File open error!\n");
        exit(1);
    }
    char signature[5];
    unsigned char chD;
    float freqD;
    fread(signature, sizeof(char), 4, fp_inD);
    signature[4] = '\0';
    if (strcmp(signature, "HFMN") != 0)
    {
        printf("Not a Huffman file!\n");
        exit(1);
    }
    fread(uCount, sizeof(int), 1, fp_inD);
    for (int i = 0; i < (*uCount); i++)
    {
        fread(&chD, sizeof(unsigned char), 1, fp_inD);
        fread(&freqD, sizeof(float), 1, fp_inD);
        syms[i].ch = chD;
        syms[i].freq = freqD;
        syms[i].code[0] = (char)"";
        syms[i].left = NULL;
        syms[i].right = NULL;
        psym[i] = &syms[i];
    }
    fread(tail, sizeof(int), 1, fp_inD);
    fread(sizeIn, sizeof(long), 1, fp_inD);
    fread(size101, sizeof(long), 1, fp_inD);
    fread(extension, sizeof(char), 3, fp_inD);
    extension[3] = '\0';
    *position = ftell(fp_inD);
    fclose(fp_inD);
}

void unPack(unsigned char buf[], unsigned char ch)
{
    union CODE code;
    code.ch = ch;
    buf[0] = code.byte.b1 + '0';
    buf[1] = code.byte.b2 + '0';
    buf[2] = code.byte.b3 + '0';
    buf[3] = code.byte.b4 + '0';
    buf[4] = code.byte.b5 + '0';
    buf[5] = code.byte.b6 + '0';
    buf[6] = code.byte.b7 + '0';
    buf[7] = code.byte.b8 + '0';
    buf[8] = 0;
}

void decoding(long position, long size101, int tail, char * filenameIn, char * filename101)
{
    unsigned char buf[9];
    unsigned char ch;
    long a = (size101 + (8 - tail)) / 8;
    FILE * fp_inD = fopen(filenameIn, "rb");
    if (fp_inD == NULL)
    {
        printf("Input file open error!\n");
        exit(1);
    }
    FILE * fp_101D = fopen(filename101, "wb");
    fseek(fp_inD, position, SEEK_SET);
    for (int i = 0; i < a - 1; i++)
    {
        fread(&ch, sizeof(unsigned char), 1, fp_inD);
        unPack(buf, ch);
        fputs((char*)buf, fp_101D);
    }
    fread(&ch, sizeof(unsigned char), 1, fp_inD);
    unPack(buf, ch);
    buf[tail] = '\0';
    fputs((char*)buf, fp_101D);
    fclose(fp_inD);
    fclose(fp_101D);
}

void fillDecompFile(TSYM * rootD, long size101D, char * filename101, char * filenameOut, char * name, char * extension)
{
    TSYM * tmp = rootD;
    strcpy(filenameOut, name);
    strcat(filenameOut, ".");
    strcat(filenameOut, extension);
    FILE * fp_101D = fopen(filename101, "rb");
    FILE * fp_outD = fopen(filenameOut, "wb");
    if (fp_101D == NULL)
    {
        printf(".101 file open error!\n");
        return;
    }
    unsigned char sym;
    for (int i = 0; i < size101D; i++)
    {
        fread(&sym, sizeof(unsigned char), 1, fp_101D);
        if (sym == '0')
            tmp = tmp->left;
        else if (sym == '1')
            tmp = tmp->right;
        if ((tmp->left == NULL) && (tmp->right == NULL))
        {
            fputc(tmp->ch, fp_outD);
            tmp = rootD;
        }
    }
    fclose(fp_101D);
    fclose(fp_outD);
}