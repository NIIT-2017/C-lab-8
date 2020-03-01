#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "head1.h"

struct SYM* buildTree(struct SYM* psym[], int N) {
    //make a temporary node
    struct SYM* temp = (struct SYM*)malloc(sizeof(struct SYM));
    // write down the frequency sum (means the sum of last element and penultimate element) in frequancy field
    temp->freq = psym[N - 2]->freq + psym[N - 1]->freq; 
    //linking created node to 2 last nodes
    temp->left = psym[N - 1];                                 
    temp->right = psym[N - 2];
    temp->code[0] = 0;                                                        
    //We formed root element with the frequancy 1.0                                                         
    if (N == 2) {
        return temp;
    }
    //Adding temp in suitable position keeping the order of decreasing frequancy
    if (psym[N - 3]->freq > temp->freq) {
        psym[N - 2] = temp;
    }
    else {
        psym[N - 2] = psym[N - 3];
        psym[N - 3] = temp;
    }
    return buildTree(psym, N - 1);
}

void makeCodes(struct SYM* root) {
    if (root -> left) {
        strcpy(root->left->code, root->code);
        strcat(root->left->code, "0");
        makeCodes(root->left);
    }
    if (root->right) {
        strcpy(root->right->code, root->code);
        strcat(root->right->code, "1");
        makeCodes(root->right);
    }
} 