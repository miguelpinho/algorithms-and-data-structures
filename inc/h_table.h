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

#ifndef H_TABLE_H
#define H_TABLE_H

#include "defs.h"
#include "LinkedList.h"

typedef struct _h_table h_table;

h_table* new_h_table( int size , int (*hash_function)(Item, int) , int (*compare_function)(Item, Item) , void (*free_item)(Item) );
void destroy_h_table( h_table *ht );
void clear_h_table( h_table *ht );
void put_h_table( h_table *ht , Item this , Item key );
Item get_h_table( h_table *ht , Item key );
Item remove_h_table( h_table *ht , Item key );

#endif
