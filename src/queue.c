/*
 *  File name: queue.c
 *
 *  Authors: Miguel Malaca & Miguel Pinho
 *
 *  Description: C-Implementation of a queue (FIFO)
 *
 *  Implementation details:
 *    The queue is implemented using a LinkedList, and pointer to the start and
 *    end of it. Insertion is done in the end and removal in the beginning.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

struct _queue {
  LinkedList *start, *end;
};


queue *new_queue( )
{
  queue *new;

  new = malloc(sizeof(queue));
  mem_check(new);

  new->start = initLinkedList();
  new->end = new->start;

  return new;
}

void destroy_queue( queue *q , void (*freeItemFnt)(Item) )
{
  freeLinkedList( q->start , freeItemFnt );

  free(q);
}
void clear_queue( queue *q , void (*freeItemFnt)(Item) )
{
  freeLinkedList( q->start , freeItemFnt );

  q->start = initLinkedList();
  q->end = q->start;
}
  
int isEmpty_queue( queue *q )
{
  return ( q->start == NULL );
}

void insert_queue( queue *q , Item this )
{
  if ( q->start == NULL ) {
    q->start = insertUnsortedLinkedList( q->start , this );
    q->end = q->start;
  } else {
    insertAfterLinkedList( q->end , this );
    q->end = getNextNodeLinkedList( q->end );
  }
}

Item remove_queue( queue *q )
{
  Item this;
  
  if ( q->start == NULL ) return NULL;

  this = getItemLinkedList( q->start );
  q->start = removeHeadLinkedList( q->start );

  /* Perhaps not necessary: */
  if ( q->start == NULL ) q->end = NULL;

  return this;
}

Item peek_queue( queue *q )
{
  if ( q-> start == NULL ) return NULL;

  return getItemLinkedList( q->start );
}
