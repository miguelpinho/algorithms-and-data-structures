/*
 *  File name: restriciton.c
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

#include <stdlib.h>
#include <stdio.h>
#include "restriction.h"

/*
 *  Data Type: restrict_data (semi-private)
 *  Description: Structure with:
 *      1,2) Instants when any resriction changes, either starting or ending. 
 *           They are the only ordered lists.
 *      3,4) Restrictions not yet active.
 *      5,6) Active and applieable restrictions.
 */
struct _restrict_data {
  LinkedList *instants_r_floor, *instants_r_pos;
  LinkedList *on_hold_r_floor, *on_hold_r_pos;
  LinkedList *active_r_floor, *active_r_pos;
};

/*
 *  Data Type: restriction (private)
 *  Description: Structure with:
 *      1) Start instant.
 *      2) End instant.
 *      3) Value, can represent the floor or the position number.
 */
typedef struct _restriction {
  int start, end, value;
} restriction;

/* Macro used to verify if an change instant has passed. */
#define t_passed(next_t, t) ((next_t) != NULL && (t) >= *(next_t))

restriction *new_rest( int start , int end , int value );
int is_restricted( restriction *r , int t );
void change_restrict_list( LinkedList **on_hold_r , LinkedList **active_r , int t );

/* Reads restrictions from the respective file pointer. */
restrict_data *init_restrict( FILE *restrict_fp , int M , int N )
{
  restrict_data *r_data;
  char buffer[LINE_SIZE];
  int ta, tb, px, ex, ey, ez;

  r_data = (restrict_data*) malloc(sizeof(restrict_data));
  mem_check(r_data);
  
  r_data->instants_r_floor = initLinkedList();
  r_data->instants_r_pos = initLinkedList();
  r_data->on_hold_r_floor = initLinkedList();
  r_data->on_hold_r_pos = initLinkedList();
  r_data->active_r_floor = initLinkedList();
  r_data->active_r_pos = initLinkedList();

  while ( fgets( buffer , LINE_SIZE , restrict_fp ) != NULL ) {
    if ( sscanf( buffer , "R %d %d %d %d %d" , &ta , &tb , &ex , &ey , &ez ) == 5 ) {
      r_data->on_hold_r_pos = insertUnsortedLinkedList( r_data->on_hold_r_pos , (Item) new_rest( ta , tb , get_index(ex, ey, ez, M, N) ) );

      if (tb != 0)
	r_data->instants_r_pos = insertSortedLinkedList( r_data->instants_r_pos , (Item) new_int( tb ) , cmp_int , free );
      
      r_data->instants_r_pos = insertSortedLinkedList( r_data->instants_r_pos , (Item) new_int( ta ) , cmp_int , free );
    } else if ( sscanf( buffer , "R %d %d %d" , &ta , &tb , &px ) == 3 ) {
      r_data->on_hold_r_floor = insertUnsortedLinkedList( r_data->on_hold_r_floor , (Item) new_rest( ta , tb , px ) );

      if (tb != 0)
	r_data->instants_r_floor = insertSortedLinkedList( r_data->instants_r_floor , (Item) new_int( tb ) , cmp_int , free );
      
      r_data->instants_r_floor = insertSortedLinkedList( r_data->instants_r_floor , (Item) new_int( ta ) , cmp_int , free );
    }
  }

  return r_data;
}

/* Checks if any restriction has changed, updates their lists if needed and returns a info value. */
/* Return value: res = 0 -> no restrictions changes , res = 1 -> only floor_r changed , res = 2 -> pos_r changed */
int update_restrict( restrict_data *r_data , int *t , int new_t )
{
  int res = 0;
  int *next_t_floor, *next_t_pos;

  next_t_floor = (int*) getItemLinkedList(r_data->instants_r_floor);
  next_t_pos = (int*) getItemLinkedList(r_data->instants_r_pos);
  
  if ( t_passed(next_t_floor, new_t) ) {
    /* A floor restriction started or ended. */
    res = 1;

    /* Advance the instants of changes for floors. */
    while ( t_passed(next_t_floor, new_t) ) {
      r_data->instants_r_floor = freeHeadLinkedList( r_data->instants_r_floor , free );
      next_t_floor = (int*) getItemLinkedList(r_data->instants_r_floor);
    }

    /* Update floor restrictions list. */
    change_restrict_list( &(r_data->on_hold_r_floor) , &(r_data->active_r_floor) , new_t );
  }

  if ( t_passed(next_t_pos, new_t) ) {
    /* A pos restriction started or ended. */
    res = 2;

    /* Advance the instants of changes for pos. */
    while ( t_passed(next_t_pos, new_t) ) {
      r_data->instants_r_pos = freeHeadLinkedList( r_data->instants_r_pos , free );
      next_t_pos = (int*) getItemLinkedList(r_data->instants_r_pos);
    }

    /* Update pos restrictions lists. */
    change_restrict_list( &(r_data->on_hold_r_pos) , &(r_data->active_r_pos) , new_t );
  }
  
  *t = new_t;
  return res;
}

/* Applies both the active restrictions, floor and position. */
void apply_restrict_complete( restrict_data *r_data , int *info , int size_info , int *floor_flags , int size_ff )
{
  LinkedList *aux;
  restriction *r;
  int i;

  /* Resets all restriction flags. */
  for ( i = 0 ; i < size_info ; i++ ) reset_flag(info[i]);
  for ( i = 0 ; i < size_ff ; i++ ) floor_flags[i] = 0;

  /* Applies floor restrictions. */
  for ( aux = r_data->active_r_floor ; aux != NULL ; aux = getNextNodeLinkedList(aux) ) {
    r = getItemLinkedList(aux);

    floor_flags[r->value] = 1;
  }

  /* Applies pos restrictions. */
  for ( aux = r_data->active_r_pos ; aux != NULL ; aux = getNextNodeLinkedList(aux) ) {
    r = getItemLinkedList(aux);

    set_flag(info[r->value]);
  }
}

/* Applies only the active floor restrictions. */
void apply_restrict_parcial( restrict_data *r_data , int *floor_flags , int size_ff )
{
  LinkedList *aux;
  restriction *r;
  int i;

  /* Resets floor flags. */
  for ( i = 0 ; i < size_ff ; i++ ) floor_flags[i] = 0;

  /* Applies floor restrictions. */
  for ( aux = r_data->active_r_floor ; aux != NULL ; aux = getNextNodeLinkedList(aux) ) {
    r = getItemLinkedList(aux);

    floor_flags[r->value] = 1;
  }
}

/* Destroys all restriction resources. */
void destroy_restrict( restrict_data *r_data )
{
  /* It may happen that there was no restriction file, in whiche case r_data is NULL. */
  if ( r_data == NULL )
    return;

  /* Free all lists. */
  freeLinkedList(r_data->instants_r_floor, free);
  freeLinkedList(r_data->instants_r_pos, free);
  freeLinkedList(r_data->on_hold_r_floor, free);
  freeLinkedList(r_data->on_hold_r_pos, free);
  freeLinkedList(r_data->active_r_floor, free);
  freeLinkedList(r_data->active_r_pos, free);

  /* Can now free r_data. */
  free(r_data);
}

/* Private. Allocates and initializes a new restriction. */
restriction *new_rest( int start , int end , int value )
{
  restriction *new;

  new = malloc(sizeof(restriction));
  mem_check(new);
  
  new->start = start;
  new->end = end;
  new->value = value;
  
  return new;
}

/* Private. Check a restriction. */
/* return value: -1 -> to happen ; 0 -> in effect ; 1 -> has ended */
int is_restricted( restriction *r , int t )
{
  if ( t < r->start ) return -1;

  if ( r->end != 0 && t >= r->end ) return 1;

  return 0;
}

/* Private. Processes and updates both the active and on_hold restriction lists. */
void change_restrict_list( LinkedList **on_hold_r , LinkedList **active_r , int t )
{
  LinkedList *aux, *aux2;
  restriction *r;
  int res;
  
  /* Clear restrictions that ended. */
  aux = *active_r;
  aux2 = NULL;
  while ( aux != NULL ) {
    r = (restriction*) getItemLinkedList(aux);
    
    if ( is_restricted( r , t ) == 1 ) {
      free(r);
      
      if ( aux2 == NULL ) {
	*active_r = aux = removeHeadLinkedList(aux);
      } else {
	removeMiddleLinkedList(aux2);
	aux = getNextNodeLinkedList(aux2);
      }
    } else {
      aux2 = aux;
      aux = getNextNodeLinkedList(aux);
    }
  }

  /* Pass to active restrictions the ones that started and clear the ones that were "jumped". */
  aux = *on_hold_r;
  aux2 = NULL;
  while ( aux != NULL ) {
    r = (restriction*) getItemLinkedList(aux);
    
    if ( (res = is_restricted( r , t )) != -1 ) {
      /* Remove from the list the on_hold list. */
      if ( aux2 == NULL ) {
	*on_hold_r = aux = removeHeadLinkedList(aux);
      } else {
	removeMiddleLinkedList(aux2);
	aux = getNextNodeLinkedList(aux2);
      }
      
      if ( res == 0 ) {
	/* Pass to active. */
	*active_r = insertUnsortedLinkedList( *active_r , (Item) r );
      } else {
	/* "Jumped" -> delete. */
	free(r);
      }
    } else {
      aux2 = aux;
      aux = getNextNodeLinkedList(aux);
    }
  }
}

