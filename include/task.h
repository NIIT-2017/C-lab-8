#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SIZE 256
//Structure for packing and unpacking
union Code
{
    unsigned char ch;
    struct 
    {
    // Each b gives us access for each bi accordingly
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short b3 : 1;
    unsigned short b4 : 1;
    unsigned short b5 : 1;
    unsigned short b6 : 1;
    unsigned short b7 : 1;
    unsigned short b8 : 1;
    }byte;
};
//This structure i use to make an alphabet of each byte that we can meet in file
struct Byte
{
    //Number of this kind of symbols in file
    unsigned long long num;
    unsigned char code;
};
//I use this structure to create Haffman's tree
struct Node {
    double frequency;
    unsigned char symbol;
    //Bit code of this node
    char code[140];
    //Leaves of this node
    Node* left=0, * right=0;
};
//This function reads file and returns Byte array with number of all symbols that it met.
//Also it sets numberOFAllSymbols that it met in file
Byte* readFile(FILE* inputFile, unsigned long long* numberOfAllSymbols);
//Compares num of two structs.Returns num of b minus num of a
int comparator(const void* a, const void* b);
//Compares frequency of a and b.If frequency of b more than frequency in a function returns 1.
//Else it returns -1.
int comparatorFrequency(const void* a, const void* b);
//Recuresive function which sets code for node,copies it's code for leaves and calls itself for leaves.
void setCode(Node* head, int current, int symbol);
//Returns code for current char - buf
char* findCode(Node** nodesArr, int sizeOfNodesArr, unsigned char buf);
//Makes tree and calls encodeFile
void compressFile(const char* inputFileName, const char* outputFileName);
//Writes encoded file 
void encodeFile(Node* head, Node** nodesArr, int sizeOfNodesArr, FILE* inputFile, const char* outputFileName);
//Makes tree and decompresses file
void decompressFile(const char* inputFileName, const char* outputFileName);
//Packs 8 chars that represent one byte in one char and return it
unsigned char pack(unsigned char buf[]);
//Sets 8 bits of char in array of chars
void unPack(unsigned char ch, char* buf);
//Allocates memory for tree
Node*** TreeMemAllocate(int size);
//Sets codes for tree
Node* makeTree(Node*** matrix, int size);
//Frees tree's memory
void deleteTree(Node**** matrix, int size);