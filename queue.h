/*
 *  File name: queue.h
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

#ifndef QUEUE_H
#define QUEUE_H

#include "defs.h"
#include "LinkedList.h"

typedef struct _queue queue;

queue *new_queue( );
void destroy_queue( queue *q , void (*freeItemFnt)(Item) );
void clear_queue( queue *q , void (*freeItemFnt)(Item) );
int isEmpty_queue( queue *q );
void insert_queue( queue *q , Item this );
Item remove_queue( queue *q );
Item peek_queue( queue *q );


#endif
