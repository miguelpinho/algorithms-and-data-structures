/*
 *  File name: restriciton.h
 *
 *  Authors: Miguel Malaca & Miguel Pinho
 *
 *  Description: Module responsible por processing the restriction. It reads
 *    them from the respective file, updates them during the car processing
 *    and applies them to the garage variables.
 *
 *  Implementation details:
 *    A group of LinkedLists was used for each type of restriction, has they
 *    they have different effects on the garage. Each restriction is represented
 *    by a start intant, an end instant and a value (its interpertation depends
 *    on the type, defined by where it is stored).
 *
 */

#ifndef RESTRICT_H
#define RESTRICT_H 

#include "defs.h"
#include "LinkedList.h"

typedef struct _restrict_data restrict_data;

restrict_data *init_restrict( FILE *restrict_fp , int M , int N );
int update_restrict( restrict_data *r_data , int *t , int new_t );
void apply_restrict_complete( restrict_data *r_data , int *info , int size_info , int *floor_flags , int size_ff );
void apply_restrict_parcial( restrict_data *r_data , int *floor_flags , int size_ff );
void destroy_restrict( restrict_data *r_data );

#endif
