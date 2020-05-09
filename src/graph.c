/*
 *  File name: graph.c
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

#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

/*
 *  Data Type: graph (semi-private)
 *  Description: Structure with:
 *      1) Number of vertices.
 *      2) Pointer to the list array.
 *
 *      3) Pointer to the search priority queue.
 */
struct _graph {
  int V;
  LinkedList **adj;

  p_queue *fringe;
};

/*
 *  Data Type: link (private)
 *  Description: Structure with:
 *      1) Number of the vertex connected to.
 *      2) Edge's weight.
 *  Uses: graph connections in adj list
 *        fringe item in pfs
 */
typedef struct _link {
  int to;
  int weight;
} link;

link* new_link( int , int );
int less_P( Item , Item );

/* Allocates and inicializes a graph with the size given. */
graph* new_graph( int V )
{
  graph *new;
  int i;

  new = (graph*) malloc( sizeof(graph) );
  mem_check(new);

  new->V = V;

  new->adj = (LinkedList**) malloc( V * sizeof(LinkedList*) );
  mem_check(new->adj);

  for ( i = 0 ; i < V ; i++ ) {
    new->adj[i] = initLinkedList();
  }

  new->fringe = new_p_queue( V , less_P , free );

  return new;
}

/* Dealocates the graph resources. */
void destroy_graph( graph *g )
{
  int i;

  for( i = 0 ; i < g->V ; i++ ) {
    freeLinkedList( g->adj[i] , free );
  }
  free( g->adj );

  destroy_p_queue( g->fringe );

  free( g );
}

/* Returns the number of vertexes. */
int V_graph( graph *g )
{
  return g->V;
}

/* Inserts a new bidireccional edge, between vertexes a and b, with the weight given. */
void insertE_graph( graph *g , int a , int b , int weight )
{
  /* Connects vertice a to b. */
  g->adj[a] = insertUnsortedLinkedList( g->adj[a] , (Item) new_link(b, weight) );

  /* Coonects vertice b to a. */
  g->adj[b] = insertUnsortedLinkedList( g->adj[b] , (Item) new_link(a, weight) );
}

/* Generates the shortest path tree from a source vertex. */
void generate_spt( graph *g , int *info , int M , int N , int *spt , int *wt , int source )
{
  LinkedList *aux;
  link *cur, *adj; /* Completely different use: cur -> fringe item, adj -> graph edge */
  int i = 0;

  /* Initializes search vectors and the fringe. */
  for ( i = 0 ; i < g->V ; i++ ) { spt[i] = -1; wt[i] = -1; }
  clear_p_queue( g->fringe );

  /* Check if source is a valid vertex. */
  if ( get_flag(info[source]) == 0 ) {
    /* It is. Start the search in it, marking it and adding it to the fringe. */
    spt[source] = source;
    wt[source] = 0;
    insert_p_queue( g->fringe , (Item) new_link(source, 0) );
  }

  /* While there are vertices to explore. */
  while( !isEmpty_p_queue( g->fringe ) ) {
    cur = (link*) remove_p_queue( g->fringe );

    for ( aux = g->adj[cur->to] ; aux != NULL ; aux = getNextNodeLinkedList(aux) ) {
      adj = (link*) getItemLinkedList( aux );

      /* As soon as a vertex is reached by priority first search we now it is the best path, 
	 due to the type of graph (weights are only 1 and 2 - ver relatório). */
      if ( spt[adj->to] == -1 ) {
	if ( get_flag(info[adj->to]) != 0 || get_id(info[adj->to]) == INVALID ) {
	  /* Mark this vertex as invalid. It will not be explored again. */
	  spt[adj->to] = -2;

	  /* Does not continue search from it. */
	} else {
	  /* Is unexplored and valid (EMPTY (normal, UP ou DOWN) or B_OBJECTIVE). */

	  /* If moved to a UP from a pos in the same floor has to go DOWN. */
	  if ( get_id(info[cur->to]) == UP && get_id(info[adj->to] != DOWN ) ) {
	    /* Check if previous pos was in the same floor. */
	    if ( get_z(spt[cur->to], M, N) == get_z(cur->to, M, N) )
	      continue;
	  }

	  /* If moved to a DOWN from a pos in the same floor has to go UP. */
	  if ( get_id(info[cur->to]) == DOWN && get_id(info[adj->to] != UP ) ) {
	    /* Check if previous pos was in the same floor. */
	    if ( get_z(spt[cur->to], M, N) == get_z(cur->to, M, N) )
	      continue;
	  }

	  /* Mark it with the vertex used to reach it. */
	  spt[adj->to] = cur->to;

	  /* Calculate total weight to reach it. */
	  wt[adj->to] = wt[cur->to] + adj->weight;

	  if ( get_id(info[adj->to]) != PARK_PLACE_E && get_id(info[adj->to]) != PARK_PLACE_O ) {
	    /* It is a valid move vertex (cannot move through a park_place). */
	    /* Add to queue. */
	    insert_p_queue( g->fringe , (Item) new_link(adj->to, wt[adj->to]) );
	  }

	  /* If it was a B objective the sub-path ends in it. */
	}
      }
    }

    free(cur);
  }

  /* Fringe should have been emptied, but just in case. */
  clear_p_queue( g->fringe );

  /* Vertexes that could not be reached are left with wt[v] == -1. */
}

/* Prints to a LinkedList the path A->B->C, using the 2 short path trees given. */
LinkedList* get_path( int vB , int *AtoB , int *BtoC )
{
  LinkedList *list, *aux;
  int v;

  /* Initializes list with vB vertex. */
  list = initLinkedList();
  aux = list = insertUnsortedLinkedList( list , (Item) new_int(vB) );

  /* Adds vertexes to reach C after vB in the list. */
  v = BtoC[vB];
  while ( v != BtoC[v] ) {
    insertAfterLinkedList( aux , (Item) new_int(v) );
    aux = getNextNodeLinkedList( aux );

    v = BtoC[v];
  }
  insertAfterLinkedList( aux , (Item) new_int(v) );

  /* Adds vertexes from A before vB in the list. */
  v = AtoB[vB];
  while ( v != AtoB[v] ) {
    list = insertUnsortedLinkedList( list , (Item) new_int(v) );

    v = AtoB[v];
  }
  list = insertUnsortedLinkedList( list , (Item) new_int(v) );

  return list;
}

link* new_link( int to , int weight )
{
  link *new;

  new = (link*) malloc( sizeof(link) );
  mem_check(new);

  new->to = to;
  new->weight = weight;

  return new;
}

int less_P( Item i1 , Item i2 )
{
  if ( ((link*) i1)->weight > ((link*) i2)->weight )
    return 1;

  return 0;
}

