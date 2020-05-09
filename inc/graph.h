/*
 *  File name: graph.h
 *
 *  Authors: Miguel Malaca & Miguel Pinho
 *
 *  Description: C-Implementation of a weighted (integer), undirected graph,
 *               with specialized search functions embedded.
 *
 *  Implementation details:
 *    The graph structure is represented by a adjencency list. Each vertex,
 *    represent by an integer, corresponds to an index in a array of lists.
 *    The corresponding list stores the vertices to which it is connected,
 *    along with the edges weights.
 *
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "defs.h"
#include "LinkedList.h"
#include "p_queue.h"
#include "restriction.h"

typedef struct _graph graph;

/* graph: */
graph* new_graph( int V );
void destroy_graph( graph *g );
int V_graph( graph *g );
void insertE_graph( graph *g , int a , int b , int weight );

/* search: */
void generate_spt( graph *g , int *info , int M , int N , int *spt , int *wt , int source );
LinkedList* get_path( int vB , int *AtoB , int *BtoC );

#endif
