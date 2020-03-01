#include "task.h"

void clean_buf(char* buf, int size)
{
    for (int i = 0; i < size; i++)
        buf[i] = 0;
}

tSYM create_sym(uc ch, float part)
{
    tSYM sym;
    sym.ch = ch;
    sym.freq = part;
    clean_buf(sym.code, CODE_SIZE);
    sym.left = NULL;
    sym.right = NULL;
    return sym;
}

int create_syms(FILE* file, tSYM* syms)
{
    int ch;
    float count = 0;
    float part = 0;
    int number_of_syms = 0;
    while ((ch = fgetc(file)) != -1)
        count++;
    part = (1 / count);
    rewind(file);
    while ((ch = fgetc(file)) != -1)
    {
        if (fabs(syms[(uc)ch].freq - 0) < 0.0000001f)
        {
            //syms[(uc)ch] = create_sym((uc)ch, part);
            syms[(uc)ch].ch = ch;
            syms[(uc)ch].freq = part;
            clean_buf(syms[(uc)ch].code, CODE_SIZE);
            syms[(uc)ch].left = NULL;
            syms[(uc)ch].right = NULL;
            number_of_syms++;
        }
        else
            syms[(uc)ch].freq += part;
    }
    return number_of_syms;
}

void sort_syms(tSYM* buf, int size)
{
    float max_freq = 0;
    tSYM temp;
    for (int i = 0; i < size; i++)
    {
        for (int j = i; j < size; j++)
            if (buf[j].freq != 0 && buf[j].freq > max_freq)
                max_freq = buf[j].freq;

        for (int j = i; j < size; j++)
            if (buf[j].freq != 0 && buf[j].freq == max_freq)
            {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        max_freq = 0;
    }
}

void create_psyms(tSYM* syms, pSYM* psyms, int number_of_syms)
{
    for (int i = 0; i < number_of_syms; i++)
        psyms[i] = &syms[i];
}

void print(pSYM* buf, int size)
{
    for (int i = 0; i < size; i++)
        if (buf[i] != NULL)
        {
            if (buf[i]->ch == ' ')
                printf("%f - 'space'\n", buf[i]->freq);
            else if (buf[i]->ch == '\n')
                printf("%f - '\\n'\n", buf[i]->freq);
            else
                printf("%f - %c\n", buf[i]->freq, buf[i]->ch);
        }
}

void move_arr(pSYM p_sym[], pSYM temp, int left, int right)
{
    for (int j = right; j > left - 1; j--)
        p_sym[j + 1] = p_sym[j];
    p_sym[left] = temp;
}

pSYM build_tree(pSYM p_sym[], int N, int size)
{
    pSYM temp = (pSYM)malloc(sizeof(tSYM));
    temp->freq = p_sym[N - 2]->freq + p_sym[N - 1]->freq;
    temp->right = p_sym[N - 2];
    temp->left = p_sym[N - 1];
    temp->code[0] = 0;

    if (N == 2)
        return temp;
    int i = N - 1;
    while (i >= 0 && p_sym[i]->freq <= temp->freq)
        i--;
    move_arr(p_sym, temp, i + 1, size - 1);

    return build_tree(p_sym, N - 1, size+1);
}

void makeCodes(pSYM root)
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

uc pack(uc buf[])
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

void coder(FILE* in, FILE* out)
{
    int ch;
    int count = 0;
    uc buf[8] = { NULL };
    while ((ch = fgetc(in)) != -1)
    {
        
        buf[count] = (uc)ch;
        count++;
        if (count > 7)
        {
            fputc(pack(buf), out);
            count = 0;
            clean_buf((char*)buf, 8); 
        }
    }
    if (ch == -1 && count > 0)
    {
        for (int i = count; i < 8; i++)
            buf[i] = '0';
        fputc(pack(buf), out);
    }
}

void repack(uc ch, char* buf)
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
    buf[8] = '\0';
}

void decoder(FILE* in, FILE* out)
{
    int ch;
    char buf[9] = { NULL };
    while ((ch = fgetc(in)) != -1)
    {
        repack(ch, buf);
        fputs(buf, out);
    }
}

int file_101_to_decopressed_file(FILE * fp_101, FILE* fp, tSYM * syms, uli size_of_101, int number_of_syms)
{
    int size_of_out_file = 0;
    int ch;
    char buf[CODE_SIZE] = { 0 };
    for (int i = 0, count = 0; count < size_of_101; count++)
    {
        ch = fgetc(fp_101);
        buf[i] = ch;
        i++;
        for (int j = 0; j < number_of_syms; j++)
        {
            if (strcmp(buf, syms[j].code) == 0)
            {
                fputc(syms[j].ch, fp);
                size_of_out_file++;
                clean_buf(buf, CODE_SIZE);
                i = 0;
                break;
            }
        }
    }
    return size_of_out_file;
}