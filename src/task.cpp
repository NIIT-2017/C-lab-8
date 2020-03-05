#include "task.h"
Byte* readFile(FILE *inputFile, unsigned long long * numberOfAllSymbols)
{
    Byte *arr=(Byte*)malloc(sizeof(Byte)*SIZE);
    //Sets start values for arr
    for (int i = 0; i < SIZE; i++)
    {
        arr[i].num = 0;
        arr[i].code = i;
    }
    unsigned char buf;
    //Reads all file and counts all symbols that it met
    while (fread(&buf, sizeof(unsigned char), 1, inputFile))
    {
        //printf("\n%c", buf);
        arr[buf].num++;
        (*numberOfAllSymbols)++;
    }
    return arr;
}
int comparator(const void* a, const void* b)
{
    return ((Byte*)b)->num-((Byte*)a)->num;
}
int comparatorFrequency(const void* a, const void* b)
{
    if (((*(Node**)a))->frequency < ((*(Node**)b))->frequency)
        return 1;
    else
        return -1;
}
void setCode(Node* head,int current,int symbol)
{
    //Sets last symbol in code
    head->code[current] = symbol+'0';
    head->code[current + 1] = '\0';
    if (head->left)
    {
        strcpy(head->left->code, head->code);
        setCode(head->left, current + 1, 0);
    }
    if (head->right)
    {
        strcpy(head->right->code, head->code);
        setCode(head->right, current + 1, 1);
    }
}
char *findCode(Node** nodesArr, int sizeOfNodesArr, unsigned char buf)
{
    for (int i = 0; i < sizeOfNodesArr; i++)
    {
        if (buf == nodesArr[i]->symbol)
        {
            return nodesArr[i]->code;
        }
    }
    return 0;
}
unsigned char pack(unsigned char buf[])
{
    union Code code;
    //Sets evety bit in union from buf
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
void unPack(unsigned char ch,char *buf)
{
    union Code code;
    code.ch = ch;
    //Sets every bit to chars in buf
    buf[0] = code.byte.b1 + '0';
    buf[1] = code.byte.b2 + '0';
    buf[2] = code.byte.b3 + '0';
    buf[3] = code.byte.b4 + '0';
    buf[4] = code.byte.b5 + '0';
    buf[5] = code.byte.b6 + '0';
    buf[6] = code.byte.b7 + '0';
    buf[7] = code.byte.b8 + '0';
}

Node*** TreeMemAllocate(int size)
{
    Node*** matrix = (Node***)malloc(sizeof(Node**) * size);
    //Allocate matrix for each row in matrix
    for (int i = 0; i < size; i++)
    {
        matrix[i] = (Node**)malloc(sizeof(Node*) * (size - i));
    }
    //Allocates memory for each node in first row
    for (int i = 0; i < size; i++)
    {
        matrix[0][i] = (Node*)malloc(sizeof(Node));
        //Sets leaves to null
        matrix[0][i]->left = 0;
        matrix[0][i]->right = 0;
    }
    return matrix;
}
Node* makeTree(Node ***matrix,int size)
{
    //Start from 1 because row 0 is alredy set
    for (int i = 1; i < size; i++)
    {
        int j;
        //Set each pointer in row to element in row before
        for (j = 0; j < (size - 1 - i); j++)
        {
            matrix[i][j] = matrix[i - 1][j];
        }
        //Allocate memory for last element because it is new sum of two elements under it
        matrix[i][j] = (Node*)malloc(sizeof(Node));
        //Set sum of two frequencies to this node
        matrix[i][j]->frequency = matrix[i - 1][j]->frequency + matrix[i - 1][j + 1]->frequency;
        //Set pointers for leaves that in row under
        matrix[i][j]->left = matrix[i - 1][j];
        matrix[i][j]->right = matrix[i - 1][j + 1];
        qsort(matrix[i], size - i, sizeof(Node*), comparatorFrequency);
    }
    //Set all codes in tree
    setCode(matrix[size - 1][0]->left, 0, 0);
    setCode(matrix[size - 1][0]->right, 0, 1);
    //for (int i = 0; i < size; i++)
        //printf("\nCode %c : %s", matrix[0][i]->symbol, matrix[0][i]->code);
    return matrix[size-1][0];
}
void deleteTree(Node**** matrix, int size)
{
    //Free each element in row
    for (int i = 0; i < size; i++)
    {
            free((*matrix)[0][i]);
    } 
    //Free each row
    for (int i = 0; i < size; i++)
    {
        free((*matrix)[i]);
    }
    //Free pointer to row
    free(*matrix);
    (*matrix) = 0;
}
void compressFile(const char*inputFileName,const char* outputFileName)
{
    unsigned long long numberOfAllSymbols = 0;
    FILE* inputFile = fopen(inputFileName, "rb");
    Byte* arr = readFile(inputFile, &numberOfAllSymbols);
    qsort(arr, SIZE, sizeof(Byte), comparator);
    int size = 0;
    //Counts number of symbols that we have met in file at least one time
    for (int i = 0; i < SIZE; i++)
        if (arr[i].num > 0)
            size++;
    Node*** matrix =TreeMemAllocate(size);
    //Computes frequensy of symbols and set each symbol
    for (int i = 0; i < size; i++)
    {
        matrix[0][i]->frequency = (float)arr[i].num / (float)numberOfAllSymbols;
        matrix[0][i]->symbol = arr[i].code;
    }
    makeTree(matrix,size);
    encodeFile(matrix[size - 1][0], matrix[0], size, inputFile, outputFileName);
    fclose(inputFile);
    free(arr);
    deleteTree(&matrix, size);
}
void encodeFile(Node* head, Node** nodesArr, int sizeOfNodesArr, FILE* inputFile, const char* outputFileName)
{
    char tmpFileName[] = "Tmp.txt";
    FILE* outFile = fopen(outputFileName, "wb");
    //Writes number of symbols in alphabet
    fwrite(&sizeOfNodesArr, sizeof(sizeOfNodesArr), 1, outFile);
    rewind(inputFile);
    //Temporary file to write all codes
    FILE* tempFile = fopen(tmpFileName, "wb");
    unsigned char buf;
    char* code;
    long long int totalLength = 0;
    int tail = 0;
    while (fread(&buf, sizeof(char), 1, inputFile))
    {
        //Finds a code for current symbol
        code = findCode(nodesArr, sizeOfNodesArr, buf);
        totalLength += strlen(code);
        fprintf(tempFile, "%s", code);
    }
    tail = 8 - totalLength % 8;
    //Add the tail to temporary file
    for (int i = 0; i < tail; i++)
        fprintf(tempFile, "%c", 0);
    fclose(tempFile);
    fwrite(&totalLength, sizeof(totalLength), 1, outFile);
    fwrite(&tail, sizeof(tail), 1, outFile);
    //Writes symbols and its frequency in output file
    for (int i = 0; i < sizeOfNodesArr; i++)
    {
        fwrite(&(nodesArr[i]->symbol), sizeof(char), 1, outFile);
        fwrite(&(nodesArr[i]->frequency), sizeof(double), 1, outFile);
    }
    tempFile = fopen(tmpFileName, "rb");
    //Buffer for reading temporary file
    unsigned char packBuf[9];
    while (!feof(tempFile))
    {
        //Read 8 chars to buffer
        fread(packBuf, sizeof(char), 8, tempFile);
        packBuf[8] = '\0';
        //printf("\nPack Buf : %s", packBuf);
        //Pack 8 chars as bits to one char
        unsigned char ch = pack(packBuf);
        fwrite(&ch, sizeof(char), 1, outFile);
    }
    fclose(tempFile);
    remove(tmpFileName);
    fclose(outFile);
}
void decompressFile(const char* inputFileName, const char* outputFileName)
{
    FILE* inputFile = fopen(inputFileName,"rb");
    int size;
    //Read the size of the alphabet
    fread(&size, sizeof(size), 1, inputFile);
    long long int totalLength;
    //Read the length
    fread(&totalLength, sizeof(totalLength), 1, inputFile);
    int tail;
    //Read the tail
    fread(&tail, sizeof(int), 1, inputFile);
    Node*** codes = TreeMemAllocate(size);
    //Read symbols and frequencies for them
    for (int i = 0; i <size; i++)
    {
        fread(&(codes[0][i]->symbol), sizeof(char), 1, inputFile);
        fread(&(codes[0][i]->frequency), sizeof(double), 1, inputFile);
       // printf("\nRead code %c : %f", codes[0][i]->symbol, codes[0][i]->frequency);
    }
    Node* head=makeTree(codes, size);
    //Temporary array for bits in input file
    char* arr = (char*)malloc(8);
    unsigned char ch;
    Node* tmp=head;
    FILE* decompressed = fopen(outputFileName, "wb");
    //Read the entire  file
    while(fread(&ch, sizeof(char), 1, inputFile))
    {
        unPack(ch,arr);
        //It goes through tree and in the end it writes a symbol according to the code
        for (int j=0; j < 8; j++)
        {
            if (arr[j] == '0')
                tmp = tmp->left;
            else
                tmp = tmp->right;
            if (!tmp->left && !tmp->right)
            {
                fprintf(decompressed, "%c", tmp->symbol);
                tmp = head;
            }
        }
    }
    fclose(decompressed);
    fclose(inputFile);
    deleteTree(&codes, size);
}