/*
 *  File name: defs.h
 *
 *  Author: 2013 Fall Semester AED Team
 *
 *  Modified and used by: Miguel Malaca & Miguel Pinho
 *
 *  Release date: 2013/10/04
 *
 *  Description: Header file for an abstract type.
 *
 *  Data type list:
 *    Item
 *
 *  Function list:
 *    None
 *
 *  Dependencies:
 *    None
 *
 *  Version: 1.0
 *
 *  Change log: added mem_check macro
 *
 */


/* Prevent multiple inclusions                                      */
#ifndef defsHeader
#define defsHeader

#define LINE_SIZE 200
#define COST_MULT 3
#define H_TABLE_SIZE 3001

#define EMPTY 0
#define UP 1
#define DOWN 2
#define PARK_PLACE_E 3
#define PARK_PLACE_O 4
#define INVALID 5

#define mem_check(x) if ((x) == NULL) { fprintf(stderr, "Memory Error.\n"); exit(0); }
#define file_check(x, FILE) if ((x) == NULL) { fprintf(stderr, "Error opening file %s.\n", FILE ); exit(0); }
#define get_index(x, y, z, M, N) ((x) + (N) * (y) + (z) * (M) * (N))
#define get_x(I, N) ((I) % (N))
#define get_z(I, N, M) ((I) / ((N) * (M)))
#define get_y(I, M, N) (((I)-(get_z(I, N, M)*(N)*(M))) / (N))


/* Bit flag -> used to mark in the id which positions have restrictions.
   0 -> no restrictions  ;  1 -> has restrictions
 */
#define ID_FLAG 0x80000000

/* Mask used to recover each identificaton. */
#define ID_MASK 0x7FFFFFFF

#define get_flag(x) ((x) & ID_FLAG)
#define get_id(x) ((x) & ID_MASK)
#define set_flag(x) ((x) = ((x) | ID_FLAG))
#define reset_flag(x) ((x) = ((x) & ID_MASK))
#define change_id(x, y) ((x) = (get_flag(x) | (y)))

/*
 *  Data type: Item
 *
 *  Description: Pointer to void.
 */
typedef void * Item;

int *new_int( int i );
int cmp_int( Item a , Item b );
void print_int( Item a , FILE *fp );

/* End of: Protect multiple inclusions                              */
#endif
