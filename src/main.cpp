#include "task.h"

int main(int argc, char* argv[])
{
    FILE* fp;
    FILE* fp_101;
    tSYM syms[SYMS_SIZE] = { 0 };
    pSYM psyms[PSYMS_SIZE] = { NULL };
    int ch;
    int number_of_syms;
    uli size_of_101;
    uli size_of_in_file = 0;
    uli size_of_out_file = 0;
    char file_name[SIZE_FILE_NAME] = { 0 };
    char file_extansion[SIZE_FILE_EXTANSION] = { 0 };
//CASE FILE IS NOT FOUND
    fp = fopen(argv[1], "rb");
    if (!fp)
    {
        perror("File:");
        exit(1);
    }
//START READING THE FILE
    char code[SIZE_OF_UNIQ_CODE+1] = { 0 };
    for (int i = 0; i <= SIZE_OF_UNIQ_CODE; i++)
    {
        int c = fgetc(fp);
//CASE THE FILE IS EMPTY
        if (i == 0 && c == -1)  
        {
            printf("File is empty");
            exit(2);
        }
//CASE THE FILE IS NOT EMPTY
        else if (i > 0 && c == -1)
            break;
        else
            code[i] = (char)c;
    }
//**COMPRESSING OF FILE**//
    if (strcmp(argv[2], COMMAND_TO_COMPRSD) == 0)
    {
        printf("compressing...\r");
//COMPRESSING - GETTING FILE NAME AND FILE EXTANSION
        for (int i = 0; argv[1][i]; i++)
            if (argv[1][i] != '.')
                file_name[i] = argv[1][i];
            else
            {
                for (int j = 0; argv[1][i]; j++)
                {
                    file_extansion[j] = argv[1][i];
                    i++;
                }
                break;
            }
//COMPRESSING - GETTING COMPRESSED FILE NAME
        strcat(file_name, COMPRSD_EXTANSION);
//COMPRESSING - ANALYZE (GETTING TABLE OF FREQUENCY):
        rewind(fp);
        number_of_syms = create_syms(fp, syms);
//COMPRESSING - ONE KIND OF SYMBOLS
        if (number_of_syms == 1)
        {
            sort_syms(syms, SYMS_SIZE);
            syms[0].code[0] = '0';
        }
//COMPRESSING - SEVERAL KINDS OF SYMBOLS
        else
        {
            sort_syms(syms, SYMS_SIZE);
            create_psyms(syms, psyms, number_of_syms);
//COMPRESSING - GENERATION OF THE CODES (AND BUILDING THE TREE)
            pSYM root = build_tree(psyms, number_of_syms, number_of_syms);
            makeCodes(root);
        }
//COMPRESSING - TEPORARY FILE CREATION
        fp_101 = fopen(TEMP_FILE_NAME, "wb");
        rewind(fp);
        size_of_101 = 0;
        while ((ch = fgetc(fp)) != -1)
        {
            size_of_in_file++;
            for (int i = 0; i < number_of_syms; i++)
                if (syms[i].ch == (uc)ch)
                {
                    fputs(syms[i].code, fp_101);
                    size_of_101 += strlen(syms[i].code);
                    break;
                }
        }
        fclose(fp);
        fclose(fp_101);
//COMPRESSING - WRITTING OF THE TITTLE
        fp_101 = fopen(TEMP_FILE_NAME, "rb");
        fp = fopen(file_name, "wb");
        char title[SIZE_OF_UNIQ_CODE + 1] = { UNIQ_CODE };
        fwrite(title, 1, sizeof(title), fp);
        fwrite(&number_of_syms, sizeof(int), 1, fp);
        for (int i = 0; i < number_of_syms; i++)
        {
            fwrite(&syms[i].ch, sizeof(uc), 1, fp);
            fwrite(&syms[i].freq, sizeof(float), 1, fp);
        }
        fwrite(&size_of_101, sizeof(uli), 1, fp);
        fwrite(&size_of_in_file, sizeof(uli), 1, fp);
        fwrite(&file_extansion, 1, sizeof(file_extansion), fp);
//COMPRESSING - TEPORARY FILE TRANSLATION
        coder(fp_101, fp);
        fclose(fp_101);
        fclose(fp);
//COMPRESSING - TEPORARY FILE REMOVING
        remove(TEMP_FILE_NAME);
        
        printf("File has been compressed successfully!\n");
    }
//**DECOMPRESSING OF FILE**//
    else if (strcmp(argv[2], COMMAND_TO_DECOMPRSD) == 0)
    {
        printf("decompressing...\r");
//DECOMPRESSING - GETTING FILE NAME
        for (int i = 0; argv[1][i] != '.'; i++)
                file_name[i] = argv[1][i];
//DECOMPRESSING - WRONG TYPE OF COMPRESSED FILE
        if (strcmp(code, UNIQ_CODE) != 0)
        {
            printf("Wrong type of the compressed file");
            exit(3);
        }
//DECOMPRESSING - RIGHT TYPE OF COMPRESSED FILE
        else
        {
//DECOMPRESSING - READING OF TITTLE:
            fread(&number_of_syms, sizeof(int), 1, fp);
            for (int i = 0; i < number_of_syms; i++)
            {
                fread(&syms[i].ch, sizeof(uc), 1, fp);
                fread(&syms[i].freq, sizeof(float), 1, fp);
                clean_buf(syms[i].code, CODE_SIZE);
                syms[i].left = NULL;
                syms[i].right = NULL;
            }
            fread(&size_of_101, sizeof(uli), 1, fp);
            fread(&size_of_in_file, sizeof(uli), 1, fp);
            fread(&file_extansion, 1, sizeof(file_extansion), fp);
            //for (int i = 0; i < 512; i++)
            //    printf("%d - %c - %f\n", i, syms[i].ch, syms[i].freq);
//DECOMPRESSING - GETTING DECOMPRESSED FILE NAME
            strcat(file_name, file_extansion);
//DECOMPRESSING - ONE KIND OF SYMBOLS
            if (number_of_syms == 1)
            {
                fclose(fp);
                fp = fopen(file_name, "wb");
                for (int i = 0; i < size_of_101; i++)
                {
                    fputc(syms[0].ch, fp);
                    size_of_out_file++;
                }
                fclose(fp);
            }
//DECOMPRESSING - SEVERAL KINDS OF SYMBOLS
            else
            {
//DECOMPRESSING - ANALYZE (GETTING TABLE OF FREQUENCY):
                create_psyms(syms, psyms, number_of_syms);
//DECOMPRESSING - GENERATION OF THE CODES (AND BUILDING THE TREE):
                pSYM root = build_tree(psyms, number_of_syms, number_of_syms);
                makeCodes(root);
//DECOMPRESSING - TEPORARY FILE CREATION:
                fp_101 = fopen(TEMP_FILE_NAME, "wb");
                decoder(fp, fp_101);
                fclose(fp);
                fclose(fp_101);
//DECOMPRESSING - TEPORARY FILE TRANSLATION:
                fp_101 = fopen(TEMP_FILE_NAME, "rb");
                fp = fopen(file_name, "wb");
                size_of_out_file = file_101_to_decopressed_file(fp_101, fp, syms, size_of_101, number_of_syms);
                fclose(fp);
                fclose(fp_101);
//DECOMPRESSING - TEPORARY FILE REMOVING
                remove(TEMP_FILE_NAME);
            }
//DECOMPRESSING - CHECKING OF DECOMPRESSING RESULT
            if (size_of_out_file == size_of_in_file)
                printf("File has been decompressed successfully!\n");
        }
    }
    return 0;
}