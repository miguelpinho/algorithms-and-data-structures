/*
 *  File name: h_table
 *
 *  Authors: Miguel Malaca & Miguel Pinho
 *
 *  Description: C-implentation of an hash table, by list dispersion.
 *
 *  Implementation details:
 *    The hash table, initialized wiyh a certain size and hash function, is 
 *    implemented by an array of lists. The elements givens are dispersed 
 *    using the hash fuction throughout the various lists.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "h_table.h"

/*
 *  Data Type: graph (semi-private)
 *  Description: Structure with:
 *      1) Size of the hash_table.
 *      2) Lists array.
 *      3) Hash functions.
 *      3) Compare Item function.
 *      5) Free Item function.
 */
struct _h_table {
  int size;
  LinkedList **buckets;
  int (*hash_function)(Item, int);
  int (*compare_function)(Item, Item);
  void (*free_item)(Item);
};

/*
 *  Data Type: h_item (private)
 *  Description: Structure with:
 *      1) Item stored.
 *      2) Item's key.
 */
typedef struct _h_item {
  Item this;
  Item key;
} h_item;

h_item *new_h_item( Item this , Item key );

/* Initializes hash_table. */
h_table* new_h_table( int size , int (*hash_function)(Item, int) , int (*compare_function)(Item, Item) , void (*free_item)(Item) )
{
  h_table *new;
  int i;

  new = (h_table*) malloc( sizeof(h_table) );
  mem_check(new);

  new->size = size;
  new->buckets = (LinkedList**) malloc( size * sizeof(LinkedList*) );
  for ( i = 0 ; i < size ; i++ ) {
    new->buckets[i] = initLinkedList();
  }
  
  new->hash_function = hash_function;
  new->compare_function = compare_function;
  new->free_item = free_item;

  return new;
}

/* Dealocates all resources used by the hash_table. */
void destroy_h_table( h_table *ht )
{
  clear_h_table(ht);
  free(ht->buckets);
    
  free(ht);
}

/* Clear the hash_table. */
void clear_h_table( h_table *ht )
{
  int i;
  LinkedList *l;
  h_item *hi;
  
  for ( i = 0 ; i < ht->size ; i++ ) {
    l = ht->buckets[i];
    
    while ( l != NULL ) {
      hi = (h_item*) getItemLinkedList(l);

      ht->free_item(hi->this);
      free(hi);

      l = removeHeadLinkedList(l);
    }

    ht->buckets[i] = NULL;
  }
}

/* Insert an element in the hash. */
void put_h_table( h_table *ht , Item this , Item key )
{
  int pos = ht->hash_function(key, ht->size);

  ht->buckets[pos] = insertUnsortedLinkedList( ht->buckets[pos] , (Item*) new_h_item(this, key));
}

/* Get an element from the hash, but do not remove it. */
Item get_h_table( h_table *ht , Item key )
{
  LinkedList *list;
  h_item *hi;
  int pos = ht->hash_function(key, ht->size);
  
  for ( list = ht->buckets[pos] ; list != NULL ; list = getNextNodeLinkedList(list) ) {
    hi = (h_item*) getItemLinkedList(list);
    
    if ( ht->compare_function(key, hi->key) ) {
      return hi->this;
    }
  }

  return NULL;
}

/* Remove and return an element from the hash. */
Item remove_h_table( h_table *ht , Item key )
{
  LinkedList *aux, *prev;
  h_item *hi;
  Item this;
  int pos = ht->hash_function(key, ht->size);
  
  for ( aux = ht->buckets[pos], prev = NULL ; aux != NULL ; prev = aux, aux = getNextNodeLinkedList(aux) ) {
    hi = (h_item*) getItemLinkedList(aux);
    
    if ( ht->compare_function(key, hi->key) ) {
      this = hi->this;

      free(hi);

      if ( prev == NULL ) {
	/* Node is at the list beginning. */
	ht->buckets[pos] = removeHeadLinkedList(ht->buckets[pos]);
      } else {
	/* Node is in the middle (or end) of the list. */
	removeMiddleLinkedList(prev);
      }
      
      return this;
    }
  }

  return NULL;
}

h_item *new_h_item( Item this , Item key )
{
  h_item *new;

  new = (h_item*) malloc( sizeof(h_item) );
  mem_check(new);

  new->this = this;
  new->key = key;
  
  return new;
}
