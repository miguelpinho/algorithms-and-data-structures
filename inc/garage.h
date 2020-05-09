/*
 *  File name: garage.h
 *
 *  Authors: Miguel Malaca & Miguel Pinho
 *
 *  Description: Implementation of the garage management.
 *
 *  Implementation details:
 *    Defines the structure with all the needed garage elements. Has functions
 *    to read and create the garage, destroy the garage, process the input files
 *    and generate the output file.
 *
 */

#ifndef GARAGE_H_INCLUDED
#define GARAGE_H_INCLUDED

#include "defs.h"
#include "graph.h"
#include "restriction.h"
#include "h_table.h"
#include "queue.h"

typedef struct _garage garage;
typedef struct _access access;
typedef struct _park park;
typedef struct _car car;
typedef struct _p_car p_car;

void read_line(char *row, int n, FILE *fp);
void read_dimensions( FILE* fp , garage* g );
void read_map (garage *g , FILE* fp, int floor );
void read_accesses ( garage* g, FILE* fp, int* );
void fill_identifier( int*, char* row, int j, int i, int floor, int N, int M );
garage* read_entryfile ( FILE* fp );
void read_restrict( garage *g, FILE *fpr );
void no_restrict( garage *g );
void path_garage( garage *g , int(*random_int)(int, int));
void write_direction(garage *g, FILE* fp, car *c, LinkedList* path, int p, int t );
int escreve_saida(FILE *fp, char *vid, int tk, int pX, int pY, int pZ, char tm );
int park_car(car* c,  garage * g, FILE* fp, int );
void process_car_file( garage *g , FILE *car_in , FILE *car_out );
void mark_invalid( garage *g );
void remove_car( garage * g, char * tag, FILE * fp, int t );
void destroy_garage( garage * g );

#endif /* GARAGE_H_INCLUDED */
