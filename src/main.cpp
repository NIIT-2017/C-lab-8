#include "task.h"
#include <algorithm>
void help()
{
    printf("\nOperationName DataInput.txt DataOutput.compressed\nExample : D Input.compressed Output.txt");
    printf("\nIf you want to compress file enter 'C',if you want to decompress enter 'D' as operation");
}
int main(int argc,char** argv)
{
    if (argc != 4)
    {
        help();
        return 1;
    }
    else
    {
        if (strcmp(argv[1], "D") && strcmp(argv[1], "C"))
        {
            help();
            return 1;
        }
        else
        {
            if (!strcmp(argv[1], "C"))
            {
                compressFile(argv[2], argv[3]);
                return 0;
            }
            if (!strcmp(argv[1], "D"))
            {
                decompressFile(argv[2], argv[3]);
                return 0;
            }
        }
    }
    
    //Code for faster testing
    /*char inputFileName[20]="WP.txt",outputFileName[20]="WP.compressed";
    compressFile(inputFileName, outputFileName);
    decompressFile(outputFileName, "WP_D.txt");
    char inputFileName[20], outputFileName[20];
    char operation;
    help();
    do
    {
        
        scanf("%c", &operation);
        switch (operation)
        {
        case 'C':
            //User enters input and output file names. And we call function that comresses file
            printf("\nEnter input file name : ");
            scanf("%s", inputFileName);
            printf("\nEnter output file name : ");
            scanf("%s", outputFileName);
            compressFile(inputFileName, outputFileName);
            break;
        case 'D':
            //The same as above but we call decompression function
            printf("\nEnter input file name : ");
            scanf("%s", inputFileName);
            printf("\nEnter output file name : ");
            scanf("%s", outputFileName);
            decompressFile(inputFileName, outputFileName);
            break;
        default:
            break;
        }
        //If user enters 'E' we stop our program
    } while (operation != 'E');*/
}