// Code to read numbers from standard input into various linked lists,
// all manipulated using pointer-to-pointer (to make it easy to change
// the linking structure of the list).

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Node for building our linked list.
struct NodeTag {
  // Value in this node.
  int value;

  // Pointer to the next node.
  struct NodeTag *next;
};

// A short type name to use for a node.
typedef struct NodeTag Node;

// Structure representing a whole linked list, with a head pointer and
// a record of how many items are ont he list.  This is kind analogous to
// an object we might build in Java, but in C, all the functions that operate
// on this object are just plain-old functions.  We have to pass them an instance
// of the object they are supposed to work on.
typedef struct  {
  // Pointer to the first node on a list.
  Node *head;

  // Number of items on the list, just to have something else.
  // to keep on our lists.
  int length;
} List;

/** The easiest function.  Use a pointer to the head pointer to insert
    a new node at the start of the lsit. */
void insertAtHead( List *list, int val )
{
  // Get a pointer to the head pointer (we don't really need to do this in this
  // case, but it'll be a good warmup for the later exercises, which are
  // more interesting.)

  // Think of this as a pointer to the link we're going to change.
  Node **link = &( list->head );

  // Make a new node, n, to contain the given value, val.
  Node *n = (Node *)malloc(sizeof(Node));
  n->value = val;

  // Link this node in at the head of the list.  This requires two
  // things, both of which we can do via link, without directly
  // accessing the head pointer.

  // First, copy the old value of the head pointer (the pointer link
  // points to) to the next pointer in node n.
  n->next = *link;

  // Now, set the head pointer (the pointer link points to) to n, so
  // our new node is at the front of the list.
  *link = n;

  // We just added a node to this list, so its one node longer now.
  list->length++;
}

/** The second easiest function.  Use a pointer to a Node pointer to
    find the last pointer at the end of the list.  Then, assign it to
    point to a new node instead.  Here, we run though the list every
    time looking for the null pointer at the end.  This could be more
    efficient with a tail pointer. */
void insertAtTail( List *list, int val )
{
  // Get a pointer to the head pointer.  If the iist is empty, that's
  // the pointer we need to change anyway.
  Node **link = &( list->head );

  // Move link ahead until it points to a null pointer.  When it does, we know we've reached
  // the null pointer at the end of the list.
  while (*link){
    link = &((*link)->next);
  }

  // Make a new node, n, and store val in it.
  Node *n = (Node *)malloc(sizeof(Node));
  n->value = val;

  // We can add the new node to the end of the list just by using link, the pointer to the
  // link we need to change.

  // First, n is our new last node, so its next pointer needs to be
  // null (don't really need link for this but you can use it if you
  // want to, since it should be pointing to the null pointer at the
  // end of the list).
  n->next = NULL;

  // Now, set the pointer link is pointing to to n.  This will add n to the end of the list.
  *link = n;

  // We just added a node to this list, so its one node longer now.
  list->length++;
}

/** The most interesting function.  Use a pointer to a Node pointer to
    find the right place to insert a new node in order keep the list
    sorted.  It's a lot like insertAtTail(), but the condition for
    finding the right link to change is a little more complicated */
void insertSorted( List *list, int val )
{
  // Get a pointer to the head pointer.  If the iist is empty, or if
  // val is smaller than anything on the list, that's the pointer we need to change.
  Node **link = &( list->head );

  // Move link ahead until it points to a null pointer or to a poitner
  // to a node with a value larger than val.  Either way, that's when
  // link points to the pointer we need to change.
  while (*link && (*link)->value < val){
      link = &((*link)->next);
  }

  // Make a new node, n, and store val in it.
  Node *n = (Node *)malloc(sizeof(Node));
  n->value = val;

  // We can add the new node to the right place in the list via the
  // link pointer, since it points to the pointer we need to change to
  // insert this new node.

  // First, make the next pointer out of n point to the node that *link is pointing
  // to.  This will link in that node so it occurs after n in the list.
  n->next = *link;

  // Now, set the pointer link is pointing to to n.  This will finish inserting the
  // new node n right before the first node that contains a value larger than val.
  *link = n;

  // We just added a node to this list, so its one node longer now.
  list->length++;
}

/** Print out the size and contents of the given list. */
void printList( List *list )
{
  // Print the size first, really just so we have a reason for maintaining it.
  printf( "%d elements :", list->length );

  // Then, print the elements on the rest of the line.
  for ( Node *n = list->head; n; n = n->next )
    printf( " %d", n->value );
  printf( "\n" );

}

/** Free the memory for all the nodes on the given list. */
void freeList( List *list )
{
  // Free all the nodes on the list.
  while ( list->head ) {
    Node *n = list->head;
    list->head = n->next;
    free( n );
  }

  // Then, set its head pointer and length appropriately.
  // We don't free the list object itself since the caller allocated
  // that (and, it's stack allocated in this example anyway).
  list->head = NULL;
  list->length = 0;
}

int main( int argc, char *argv[] )
{
  // Check command-line arguments and try to open a file given on the
  // command line.
  FILE *fp;
  if ( argc != 2 || ( fp = fopen( argv[ 1 ], "r" ) ) == NULL ) {
    printf( "Can't open file: %s\n", argv[ 1 ] );
    exit( EXIT_FAILURE );
  }

  // First, read from the file, building a list from the front.
  {
    // Structure holding our list.
    List list = { NULL, 0 };

    // Read all the values, inserting at the front.
    int val;
    while ( fscanf( fp, "%d", &val ) == 1 )
      insertAtHead( &list, val );

    // Print out the list (should be in reverse order given in the file);
    printList( &list );

    freeList( &list );
  }

  // Rewind the file, so we can read everything again.
  fseek( fp, SEEK_SET, 0 );

  // Now, read from the file, building a list from the back.
  {
    // Structure holding our list.
    List list = { NULL, 0 };

    // Read all the values, inserting at the tail.
    int val;
    while ( fscanf( fp, "%d", &val ) == 1 )
      insertAtTail( &list, val );

    // Print out the list (should be in the same order as the file);
    printList( &list );

    freeList( &list );
  }

  // Rewind the file, so we can read everything again.
  fseek( fp, SEEK_SET, 0 );

  // Now, read from the file, building a list in sorted order.
  {
    // Structure holding our list.
    List list = { NULL, 0 };

    // Read all the values, inserting at the right place to keep the list sorted.
    int val;
    while ( fscanf( fp, "%d", &val ) == 1 )
      insertSorted( &list, val );

    // Print out the list (should be in sorted order);
    printList( &list );

    freeList( &list );
  }

  fclose( fp );

  return EXIT_SUCCESS;
}
