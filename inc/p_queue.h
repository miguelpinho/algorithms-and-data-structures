/*
 *  File name: p_queue.h
 *
 *  Part of the code, for heap processing, was taken from the code given in 
 *  a AED lab.
 *
 *  Used by: Miguel Malaca & Miguel Pinho
 *
 *  Description: C-Implementation of priority queue. Has functions to insert
 *      Item with priority and extract or access the Item with most prority.
 *
 *  Implementation details:
 *    Priority queue is implemented using an heap.
 *
 */

#ifndef P_QUEUE_H
#define P_QUEUE_H

#include "defs.h"

typedef struct _p_queue p_queue;

p_queue* new_p_queue( int size, int (*less) (Item, Item), void (*free_item)(Item) );
void destroy_p_queue( p_queue *h );
void clear_p_queue( p_queue *h );
void insert_p_queue( p_queue *h , Item this );
Item remove_p_queue( p_queue *h );
Item peek_p_queue( p_queue *h );
int size_p_queue( p_queue *h );
int isEmpty_p_queue( p_queue *h );

#endif
