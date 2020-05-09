/*
 *  File name: p_queue.c
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

#include <stdlib.h>
#include <stdio.h>
#include "p_queue.h"


/* The prority queue stucture, implement by the heap given: */
struct _p_queue {
  int (*less) (Item, Item);     /* Surprise! this field is a function pointer*/
  void (*free_item)(Item);                         /* to elements in the heap. */
  int n_elements;               /* # elements in heap */
  int size;                     /* max size of the heap. */
  Item *heapdata;               /* An array of Items. */
};



/******************************************************************************
 * FixUp()
 *
 * Arguments: h - pointer to heap structure
 *            k - index of element to fixup
 * Returns:
 * Side-Effects: none
 *
 * Description: Performs fixup from index
 *
 *****************************************************************************/



void FixUp(p_queue* h, int k)
{
  Item t;
  while ((k > 0) && (h->less) ((h->heapdata)[(k - 1) / 2], (h->heapdata)[k])) {

    /*---------------------------------------------------------*/
    t = (h->heapdata)[k];
    (h->heapdata)[k] = (h->heapdata)[(k - 1) / 2];
    (h->heapdata)[(k - 1) / 2] = t;

    k = (k - 1) / 2;
  }

  return;
}


/******************************************************************************
 * FixDown()
 *
 * Arguments: h - pointer to heap structure
 *            k - index of element to fixdown
 * Returns:
 * Side-Effects: none
 *
 * Description: performs fixdown from index
 *
 *****************************************************************************/

void FixDown(p_queue * h, int k)
{
  int j;
  Item t;

  while ((2 * k + 1) < h->n_elements) {
    j = 2 * k + 1;
    if (((j + 1) < h->n_elements) &&
        (h->less) (h->heapdata[j], h->heapdata[j + 1])) {
      /* second offspring is greater */
      j++;
    }
    if (!(h->less) (h->heapdata[k], h->heapdata[j])) {
      /* Elements are in correct order. */

      /*---------------------------------------------------------*/
      break;
    }

    /*---------------------------------------------------------*/

    /* the 2 elements are not correctly sorted, it is
       necessary to exchange them */
    t = (h->heapdata)[k];
    (h->heapdata)[k] = (h->heapdata)[j];
    (h->heapdata)[j] = t;
    k = j;
  }

  return;
}

/* Interface function: Initializes a new p_queue. */
p_queue *new_p_queue(int size, int (*less) (Item, Item), void (*free_item)(Item))
{
  p_queue *h;
  int i;

  h = (p_queue *) malloc(sizeof(p_queue));
  if (h == ((p_queue *) NULL)) {
    fprintf(stderr, "Error in malloc of heap\n");
    exit(1);
  }

  h->n_elements = 0;
  h->less = less;
  h->free_item = free_item;
  h->size = size;
  h->heapdata = (Item *) malloc(size * sizeof(Item));
  if (h->heapdata == ((Item *) NULL)) {
    fprintf(stderr, "Error in malloc of heap data\n");
    exit(1);
  }
  for( i = 0 ; i < size ; i++ ) {
    h->heapdata[i] = NULL;
  }

  return (h);
}

/* Interface function: Deallocates the whole p_queue. */
void destroy_p_queue(p_queue *h)
{
  
  clear_p_queue(h);
  free(h->heapdata);
  free(h);

}

/* Interface function: Insert a new Item in the p_queue. */
void insert_p_queue(p_queue * h, Item this)
{
  if (h->n_elements == h->size) {
    return ;
  }
  h->heapdata[h->n_elements] = this;

  h->n_elements++;
  FixUp(h, h->n_elements - 1);
}

/* Interface function: remove and return the Item with most priority. */
Item remove_p_queue(p_queue * h)
{
  Item t;

  if (h->n_elements > 0) {
    t = (h->heapdata)[0];
    (h->heapdata)[0] = (h->heapdata)[h->n_elements - 1];
    (h->heapdata)[h->n_elements - 1] = t;
    h->heapdata[h->n_elements - 1] = NULL;
    h->n_elements--;
    FixDown(h, 0);
    return t;
  }

  return NULL;
}

/* Interface function: destroy all Item in th p_queue. */
void clear_p_queue(p_queue * h)
{
  int i;

  for(i = 0; i<h->n_elements; i++)
    {
      h->free_item(h->heapdata[i]);
      h->heapdata[i] = NULL;
    }
  h->n_elements = 0;
}

/* Iterface function: access the Item with most priority. */
Item peek_p_queue( p_queue *h )
{
  if (h->n_elements > 0) {
    return (h->heapdata)[0];
  }

  return NULL;
}

int size_p_queue( p_queue *h )
{
  return h->size;
}

int isEmpty_p_queue( p_queue *h )
{
  if(h->n_elements <= 0) return 1;
  return 0;
}
