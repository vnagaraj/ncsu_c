#include <stdio.h>
#include <stdlib.h>

typedef struct NodeTag {
int val;
struct NodeTag *next;
} Node;

int main()
{

Node *head = NULL;
Node **p = &head;
*p = (Node *) malloc( sizeof( Node ) );
(*p)->val = 25;
p =&(*p)->next;
return EXIT_SUCCESS;

}
