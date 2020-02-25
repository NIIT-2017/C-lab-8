#include <stdio.h>
#include <string.h>
#include "task2.h"

unsigned char searchTree(struct SYM* node, char * code)
{

	if (node == NULL)
		return 0;
	if (strcmpi(node->code, code) == 0)
		return node->ch;
	char temp[LEN]; 
	char tempR[LEN];
	char tempL[LEN];
	int n = strlen(node->code); 
	if (n+1 < strlen(code))
	{
		strncpy(temp, code, n+1); 
		temp[n + 1] = '\0';
	}
	else strcpy(temp, code);

	strcpy(tempR, node->code);
	strcat(tempR, "1");
	strcpy(tempL, node->code);
	strcat(tempL, "0");

	if (strcmpi(temp, tempL) < 0 || strcmpi(temp, tempL) == 0)
		searchTree(node->left, code);
	else if(strcmpi(temp, tempR) > 0 || strcmpi(temp, tempR) == 0)
		searchTree(node->right, code);

}

void printTree(struct SYM * node)
{
	if (node != NULL)
	{
		printTree(node->left);
		if (node->ch != 0)
			printf("%c  %s\n", node->ch, node->code);
		printTree(node->right);
	}
}