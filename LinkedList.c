/*
 *  File name: LinkedList.c
 *
 *  Author: 2013 Fall Semester AED Team
 *
 *  Release date: 2013/10/04
 *
 *  Description: C-Implementation of an Abstract Linked List
 *
 *  Implementation details:
 *    Linked list node: Each node of the list contains a pointer to
 *   the item stored in the node and a pointer to the next node. The
 *   item is specified in the file defs.h.
 *
 *  Version: 1.0
 *
 *  Change log: added removeHeadLinkedList (by M&M)
 *
 */



/* Header Inclusions                                              */
#include<stdio.h>
#include<stdlib.h>

/* Include Header File with Data Type and Function Prototypes     */
#include"LinkedList.h"


/*
 *  Data Type: LinkedListStruct
 *
 *  Description: Structure with:
 *                 1) Pointer to the item of the linked list node
 *                 2) Pointer to next node of the linked list.
 */
struct LinkedListStruct
{
  Item this;
  LinkedList * next;
};

LinkedList *new_node( LinkedList *next , Item this );

/*
 *  Function:
 *    initLinkedList
 *
 *  Description:
 *    Initializes a new linked list.
 *
 *  Arguments:
 *    None
 *
 *  Return value:
 *    Returns the pointer to a new linked list.
 */
LinkedList *initLinkedList(void)
{
  return NULL;
}



/*
 *  Function:
 *    freeLinkedList
 *
 *  Description:
 *    Frees the memory allocated to a linked list.
 *
 *  Arguments:
 *    Pointer to the first element of a linked list:
 *      (LinkedList *) first
 *    Function to free the memory allocated to the items:
 *      void freeItem(Item)
 *
 *  Return value:
 *    None
 */
void freeLinkedList(LinkedList *first, void (*freeItemFnt)(Item))
{
  LinkedList * next;
  LinkedList * aux;

  /* Cycle from the first to the last element                     */
  for(aux = first; aux != NULL; aux = next)
  {
    /* Keep trace of the next node                                */
    next = aux->next;

    /* Free current item                                          */
    freeItemFnt(aux->this);

    /* Free current node                                          */
    free(aux);
  }

  return;
}

LinkedList * freeHeadLinkedList(LinkedList *first , void (*freeItemFnt)(Item) )
{
  LinkedList *next;

  next = first->next;
  freeItemFnt(first->this);
  free(first);

  return next;
}

/*
 *  Function:
 *    lengthLinkedList
 *
 *  Description:
 *    Determines the length of a linked list.
 *
 *  Arguments:
 *    Pointer to the first node of the linked list:
 *        (LinkedList *) first
 *
 *  Return value:
 *    Returns the length of the linked list.
 */
int lengthLinkedList(LinkedList * first)
{
  LinkedList * aux;
  int counter;

  /* Length determination cycle                                   */
  for(aux = first, counter = 0;
      aux!=NULL;
      counter++, aux = aux->next);

  return counter;
}


/*
 *  Function:
 *    getNextNodeLinkedList
 *
 *  Description:
 *    Returns the next node of a linked list.
 *
 *  Arguments:
 *    Pointer to the current linked list node:
 *        (LinkedList *) node
 *
 *  Return value:
 *    Returns the pointer to the next node of a linked list. NULL
 *   is returned in case the current node is empty or there is no
 *   node following the current node.
 */
LinkedList * getNextNodeLinkedList(LinkedList * node)
{
  return ((node == NULL) ? NULL : node->next);
}



/*
 *  Function:
 *    getItemLinkedList
 *
 *  Description:
 *    Gets the item of a linked list node.
 *
 *  Arguments:
 *    Pointer to a linked list node:
 *        (LinkedList *) node
 *
 *  Return value:
 *    Returns the pointer to the item of a linked list node. NULL
 *   is returned if the node is NULL (or if the item is NULL).
 */
Item getItemLinkedList(LinkedList * node)
{
  /* Check if node is not empty                                   */
  if(node == NULL)
    return NULL;

  return node->this;
}

/*
 *  Function:
 *    insertUnsortedLinkedList
 *
 *  Description:
 *    Creates a new linked list node.
 *
 *  Arguments:
 *    Item to associate to the new node:
 *      Item this
 *    Pointer to the next node:
 *      (LinkedList *) next
 *
 *  Return value:
 *    Returns the pointer to the node.
 */
LinkedList *insertUnsortedLinkedList(LinkedList *next, Item this)
{
  LinkedList *new;

  /* Memory allocation */
  new = (LinkedList *) malloc(sizeof(LinkedList));

  /* Check memory allocation errors */
  if(new == NULL)
    return NULL;

  /* Initialize new node */
  new->this = this;
  new->next = next;

  return new;
}


/*
 *  Function:
 *    insertSortedLinkedList
 *
 *  Description:
 *    Inserts an item in order in an sorted linked list.
 *
 *  Arguments:
 *    Pointer to the first node of a sorted linked list:
 *        (LinkedList *) first
 *    Pointer to item to be inserted:
 *        Item item
 *    Pointer to function to compare two items:
 *        int comparisonItemFnt(void * item1, void * item2)
 *
 *        This function returns a value less, equal, or greater
 *       than zero if item1 compares less, equal, or greater than
 *       item2, respectively.
 *
 *    Pointer to integer to write error return value:
 *        (int *) err
 *
 *        0 upon sucess, 1 in case the item is NULL, and 2 in
 *   case of memory allocation failure.
 *
 *  Return value:
 *    Returns the pointer to the first node of the sorted linked list.
 */
LinkedList * insertSortedLinkedList(LinkedList * first, Item item, int (* comparisonItemFnt)(Item item1, Item item2) , void (*freeItemFnt)(Item))
{
  int cmp = 0;
  LinkedList *new, *aux;
  
  aux = first;

  if ( aux == NULL ) {
    new = new_node( first , item );
    return new;
  }

  if ( (cmp = comparisonItemFnt(item, getItemLinkedList(aux))) < 0 ) {
    new = new_node( first , item );
    return new;
  } else if( cmp == 0 ) {
    freeItemFnt(item);
    return first;
  }

  while ( aux->next != NULL && (cmp = comparisonItemFnt(item, getItemLinkedList(aux->next))) >= 0 ) {
    if ( cmp == 0 ) {
      freeItemFnt(item);
      return first;
    }
    
    aux = aux->next;
  }

  new = new_node( aux->next , item );
  aux->next = new;

  return first;
}

void insertAfterLinkedList(LinkedList *node, Item this )
{
  LinkedList *new;

  /* Memory allocation */
  new = (LinkedList *) malloc(sizeof(LinkedList));

  /* Check memory allocation errors */
  mem_check(new);

  /* Initialize new node */
  new->this = this;
  new->next = node->next;

  /* Make connection to new */
  node->next = new;
}


LinkedList * removeHeadLinkedList(LinkedList *first)
{
  LinkedList *next;

  next = first->next;
  free(first);

  return next;
}

void removeMiddleLinkedList(LinkedList *prev)
{
  LinkedList *destroy = prev->next;

  if (destroy != NULL) {
    prev->next = destroy->next;
    free(destroy);
  }
}


void printLinkedList(LinkedList *first, FILE *fp, void (*printFnt)(Item, FILE*))
{
  LinkedList *aux;

  for ( aux = first ; aux != NULL ; aux = aux->next ) {
    printFnt( aux->this , fp );
  }

  fprintf(fp, "\n");
}

LinkedList *new_node( LinkedList *next , Item this )
{
  LinkedList *new;
  
  new = (LinkedList *) malloc(sizeof(LinkedList));
  mem_check( new );
  new->next = next;
  new->this = this;

  return new;
}
