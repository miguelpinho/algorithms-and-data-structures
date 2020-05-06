/*
 *  File name: garage.c
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "garage.h"

#define NUM_IDS 8
#define MAX_ID_SIZE 128

static const int ids[] = { EMPTY , INVALID , PARK_PLACE_E , PARK_PLACE_O ,
			   INVALID , INVALID , UP , DOWN };
static const char symbols[] = { ' ' , '@' , '.' , 'x' , 'e' , 'a' , 'u' , 'd' };

struct _garage {
  graph *garage_graph;
  int M, N, P, A, E;
  access *entries;
  LinkedList *exits[5];
  int* info;
  restrict_data *r_data;
  int valid_exit[5];
  int *valid_entry;
  h_table *parked_cars;
  int empty_parks;
  LinkedList **park_spaces;
  int* valid_floors;
};

struct _p_car{
  char* tag;
  int index_park ;
};

struct _car{
  char* tag;
  int index_entry, exit_code, t;
};

struct _access{
  char* name;
  int index;
  int* spt;
  int* wt;
};

int car_function(Item c1, int size);
int compare_car(Item c1, Item c2);
void destroy_access( Item a );
void destroy_p_car( Item p );
void destroy_car( Item c );

void read_line(char *row, int n, FILE *fp) {

  if ( fgets(row, n, fp) == NULL ) {
    /* Error reading file. */
    fprintf(stderr, "Error reading *.cfg file. \n");
    exit(0);
  } 

  return;
}

int convert_type (char type)
{
  if(type == 'C')
    return 0;
  else if(type == 'H')
    return 1;
  else if(type == 'E')
    return 2;
  else if(type == 'L')
    return 3;
  else
    return 4;
}

void read_dimensions( FILE* fp , garage* g )
{
  char aux[LINE_SIZE];
  int size, i;

  if ( fgets(aux, LINE_SIZE, fp) == NULL ) {
    /* The compiler, with the flags -O3 and -ansi, had a warning for not evaluating the fgets return value. */

    /* Error reading file. */
    fprintf(stderr, "Error reading *.cfg file. \n");
    exit(0);
  } 
  sscanf( aux, "%d %d %d %d %d" , &g->N, &g->M, &g->P, &g->E , &g->A );

  size = g->N * g->M * g->P;
  g->garage_graph = new_graph( size );
  g->entries = (access*) malloc( g->E * sizeof(access));
  g->info = (int*) malloc( size * sizeof(int));
  g->park_spaces = (LinkedList**) malloc(g->P * sizeof(LinkedList*));
  g->valid_floors = (int*) malloc(g->P * sizeof(int));
  g->valid_entry = (int*) malloc( g->E * sizeof(int));
  g->empty_parks = 0;
  for(i = 0; i < 5; i++){
    g->exits[i] = initLinkedList();
    g->valid_exit[i] = 0;
  }
  for(i = 0; i < g->E; i++){
    g->valid_entry[i] = 0;
    g->entries[i].spt = NULL;
    g->entries[i].wt = NULL;
  }
  for(i = 0; i < g->P; i++){
    g->park_spaces[i] = initLinkedList();
    g->valid_floors[i] = 0;
  }
}

int get_identifier( char symbol )
{
  int i;

  /* Compare the symbol with the array of known ones.
     Return its code if there is a match. */
  for ( i = 0 ; i < NUM_IDS ; i++ ) {
    if ( symbol == symbols[i] )
      return ids[i];
  }

  return INVALID;  /* If the symbol is not known, considers it a wall. */
}

/*Function responsible for reading and linking the vertexes of the graph
Based on the idea that teachers used on the Invididual Evaluation: 
read one line and save the line before, then comparing the adjacencies*/

void read_map (garage *g , FILE* fp, int floor )
{

  char* row, *last_row, *tmp;
  int i, j, index;
  row = (char*) malloc( (g->N +2) * sizeof(char));
  last_row = (char*) malloc( (g->N + 2) * sizeof(char));

  for ( i = g->M-1  ; i >= 0 ; i-- ){
    read_line( row , g->N+2 , fp );
    for( j = 0 ; j < g->N ; j++ ){
      index = get_index(j, i, floor, g->M, g->N);

      g->info[index] = get_identifier( row[j] );

      /* Don't link two vertex if one of them is a wall */
      if( row[j] != '@' ) {
	
	/* Don't look after the first line of the file */ 
	if( i != g->M-1 && last_row[j] != '@'){
	  
	  /* Don't link two vertex if they're a combination of accesses, entries or parks */
	  if(!((last_row[j] == '.' || last_row[j] == 'x' || last_row[j] == 'a' ||last_row[j] == 'e' ) && (row[j] == '.' || row[j] == 'x' || row[j] == 'a' || row[j] == 'e')))
	    insertE_graph( g->garage_graph , index , get_index(j, i+1, floor, g->M, g->N) , 1);
	}
	
	/* Don't look after the first column of the file */
	if( j != 0 && row[j-1] != '@'  ){
	  if(!((row[j-1] == '.' || row[j-1] == 'x' || row[j-1] == 'a' || row[j-1] == 'e') && (row[j] == '.' || row[j] == 'x' || row[j] == 'a' || row[j] == 'e')))
	    insertE_graph( g->garage_graph , index , get_index(j-1, i, floor, g->M, g->N) , 1);
	}
	
	/* Link ups to downs */
	if (row[j] == 'u'){
	  insertE_graph( g->garage_graph , index , get_index(j, i, floor+1, g->M, g->N) , 2);
	}
	if (row[j] == '.'){
	  g->empty_parks++;
	}
	/* Save a park's list per floor */
	if (row[j] == '.' || row[j] == 'x'){
	  g->park_spaces[floor] = insertUnsortedLinkedList(g->park_spaces[floor], new_int(index));
	}
      }
      
    }
    
    tmp = last_row;
    last_row = row;
    row = tmp;
  }
  free(row);
  free(last_row);
}

/* Read the positions, names and types of the accesses of the map: entries and exits*/
void read_accesses ( garage* g, FILE* fp, int* ec)
{
  int  x, y, z;
  char aux[LINE_SIZE];
  char type;
  char tag[LINE_SIZE];

  while( fgets(aux, LINE_SIZE, fp) != NULL && aux[0] != '+' ) {
    sscanf( aux, "%s %d %d %d %c" , tag, &x, &y , &z , &type);
      if (aux[0] == 'E') {
	g->entries[*ec].wt = (int*) malloc( (g->M * g->N * g->P) * sizeof(int));
	g->entries[*ec].spt = (int*) malloc( (g->M * g->N * g->P) * sizeof(int));
	g->entries[*ec].index = get_index(x, y, z, g->M, g->N);
	g->entries[*ec].name = (char*) malloc((strlen(tag) + 1) * sizeof(char));
	strcpy(g->entries[*ec].name, tag);
	*ec+=1;
      } else if(aux[0] == 'A') {
	access* a = (access*) malloc(sizeof(access));
	a->wt = (int*) malloc( (g->M * g->N * g->P) * sizeof(int));
	a->spt = (int*) malloc( (g->M * g->N * g->P) * sizeof(int));
	a->name = (char*) malloc((strlen(tag) + 1) * sizeof(char));
	strcpy(a->name, tag);
	a->index = get_index(x, y, z, g->M, g->N);
	g->exits[convert_type(type)] = insertUnsortedLinkedList(g->exits[convert_type(type)], (Item) a);
      }
  }
}

/* Iterface function: process the configure file and create garage. */
garage* read_entryfile( FILE* fp )
{

  int floor = 0, ecounter = 0;

  garage* g = (garage*) malloc (sizeof(garage));
  mem_check(g);

  read_dimensions( fp , g );

  for( floor = 0 ; floor < g->P ; floor ++){

    read_map( g , fp , floor );

    read_accesses( g , fp, &ecounter);

  }

  return g;
}

/* Interface function: generate restrictions from its file. */
void read_restrict( garage *g, FILE *fpr )
{
  g->r_data = init_restrict(fpr, g->M, g->N);
}

/* Interface function: set no restriction. */
void no_restrict( garage *g)
{
    g->r_data = NULL;
}

void update_spt_entries(garage* g, int i)
{

  generate_spt(g->garage_graph, g->info, g->M, g->N, g->entries[i].spt, g->entries[i].wt, g->entries[i].index);
  g->valid_entry[i] = 1;
}

void update_spt_exits(garage* g, int i)
{

  access *e;
  LinkedList *list1 = g->exits[i];

  while(list1 != NULL) {
    e = (access*) getItemLinkedList(list1);
    generate_spt(g->garage_graph, g->info, g->M, g->N, e->spt, e->wt, e->index);

    list1 = getNextNodeLinkedList(list1);
  }
  g->valid_exit[i] = 1;

}


/* Function responsible for choosing the right times to write in output: 
these times are when the car changes direction, parks, reaches the exit, didn't change direction
but made some move, on foot or by car . The direction changes are seen by the difference between 
the present position and the position before and between the next position and the present position */ 
void write_direction(garage *g, FILE* fp, car *c, LinkedList* path, int p, int te)
{
  LinkedList *aux1, *aux2;
  int *s2, *s3;
	/* Flag responsible for the type of movement to write and for knowing if we have to write a movement when there aren't any direction changes*/
  int ap = 0;
	int dif1 = 0, dif2 = 0, first, t_p = 0, t_e, t_a, t;
  s2 = getItemLinkedList(path);
	/* Dertermining the time when the car appears on an entry */
  t_e = c->t;
	/* Dertermining the time when the car starts to move */
  t = te;
  aux1 = getNextNodeLinkedList(path);

	/* Run through the list of the position of the way until reach the position of the exit*/
  while(getNextNodeLinkedList(aux1) != NULL) {
    t++;

    first = *s2;
    s2 = getItemLinkedList(aux1);
    aux2 = getNextNodeLinkedList(aux1);
    s3 = getItemLinkedList(aux2);
    dif1 = *s3 - *s2;
    dif2 = *s2 - first;
		/*When teh car goes up or down the time that the movement lasts is 2*/
    if (abs(dif2) == g->M * g->N) t++;
		/* Write the parking movement when the present position is the park where the car parks*/
    if(*s2 == p){
			/* Dertermining the time when the car starts parks */
      t_p = t;
		
			/*If the car never changes direction in the movement until reaching the park, write a 'm' movent on the position before*/
			if(ap == 0) escreve_saida(fp, c->tag, t-1, get_x(first, g->N), get_y(first, g->M, g->N), get_z(first, g->N, g->M), 'm');
			
			/* Change of the flag value in order to start writing on foot movements */      
			ap = 1;
      escreve_saida(fp, c->tag, t, get_x(*s2, g->N), get_y(*s2, g->M, g->N), get_z(*s2, g->N, g->M), 'e');
    }else if(dif1 != dif2 && ap <= 0){
      escreve_saida(fp, c->tag, t, get_x(*s2, g->N), get_y(*s2, g->M, g->N), get_z(*s2, g->N, g->M), 'm');
			ap--;
    }else if(ap > 0 && dif1 != dif2){
        escreve_saida(fp, c->tag, t, get_x(*s2, g->N),get_y(*s2, g->M, g->N), get_z(*s2, g->N, g->M), 'p');
				ap++;
    }

    aux1 = getNextNodeLinkedList(aux1);
  }

	/*If the car never changes direction in the movement until reaching the exit, write a 'p' movent on the position before*/
	if( ap == 1 )
	{
		escreve_saida(fp, c->tag, t, get_x(*s2, g->N),get_y(*s2, g->M, g->N), get_z(*s2, g->N, g->M), 'p');
	}

  s2 = getItemLinkedList(aux1);
  t++;
	/* Dertermining the time when the person reaches the exit */
  t_a = t;
	
  escreve_saida(fp, c->tag, t, get_x(*s2, g->N), get_y(*s2, g->M, g->N), get_z(*s2, g->N, g->M), 'a');

  escreve_saida(fp, c->tag, t_e, t_p, t_a, (t_p - t_e) + COST_MULT*(t_a - t_p), 'x');

  freeLinkedList(path, free);

}

/* Function responsible for parking a car, updating the spt's and wt's when needed */
int park_car(car* c, garage * g, FILE* fp, int t_e)
{
  int ie = 0, i, min_v = -1, min_dist = 0, result;
  int *p;
  LinkedList *list_p, *list_a, *path;
  access *min_exit, *a;
  p_car *pc;

  while( g->entries[ie].index != c->index_entry ){
    ie++;
  }
  /* If the entries and exits spts and wts are invalid at the time update them*/
  if( g->valid_entry[ie] != 1 )
    update_spt_entries(g, ie);

  if( g->valid_exit[c->exit_code] != 1 )
    update_spt_exits(g, c->exit_code);

  /* Determine the way with the minimum cost */ 
  for( i = 0; i < g->P; i++){
    if(g->valid_floors[i] == 0){
      list_p = g->park_spaces[i];
      while( list_p != NULL){
	p = getItemLinkedList(list_p);
	if(g->info[*p] == PARK_PLACE_E && g->entries[ie].wt[*p] != -1){
	  list_a = g->exits[c->exit_code];
	  while(list_a != NULL){
	    a = getItemLinkedList(list_a);
	    if(a->wt[*p] != -1){
	      result = g->entries[ie].wt[*p] + COST_MULT * a->wt[*p];
	      if(min_v == -1 || result < min_dist){

		min_v = *p;
		min_exit = a;
		min_dist = result;

	      }
	    }
	    list_a = getNextNodeLinkedList(list_a);
	  }
	}
	list_p = getNextNodeLinkedList(list_p);
      }
    }
  }

  /* Check if car parking failed (maybe due to restrictions). */
  if (min_v == -1) return 0;
  
	
  g->empty_parks--;

	/*Occupy the park chosen*/
  change_id( g->info[min_v], PARK_PLACE_O );

  pc = (p_car*) malloc(sizeof(p_car));
  pc->tag = (char*) malloc((strlen(c->tag) + 2)*sizeof(char));
  strcpy(pc->tag,c->tag);
  pc->index_park = min_v;

  put_h_table(g->parked_cars, pc, pc->tag);

  path = get_path( min_v , g->entries[ie].spt, min_exit->spt );

  write_direction(g, fp, c, path, min_v, t_e);

  return 1;

}

/******************************************************************************
 * escreve_saida ()
 *
 * Arguments: fp - descriptor do ficheiro de saida
 *            vid - identificador da viatura
 *            tk - instante de tempo em que ocorre o movimento
 *            pX, pY, pZ - coordenadas (X,Y,Z) da viatura em movimento
 *            tm - tipo de movimento
 * Returns: 0 - se nao houver qualquer erro, ou seja se as coordenadas
 *              corresponderem a uma posicao valida
 *          -1 - se houver algum erro
 * Side-Effects: nenhum
 *
 * Description: escreve no ficheiro de saida um tuplo de valores do tipo
 *                          Vid T X Y Z M
 *              sendo Vid o identificador da viatura, T o instante de tempo,
 *              X, Y e Z  indicam a posicao da viatura no parque de
 *              estacionamento e M e' o tipo de movimento podendo apenas ter
 *              os caracteres 'i', 'm', 'e', 'p', 'a', 'x' ou 's'.
 *
 *****************************************************************************/

int escreve_saida(FILE *fp, char *vid, int tk, int pX, int pY, int pZ, char tm)
{
  static char pvid[MAX_ID_SIZE];
  static int ptk = -1, ppx, ppy, ppz;

  int retval = 0;    /* valor retornado quando não há erro;
                      * se houver erro retval = -1
                      */
  char *bogus = "??";

  if(fp == NULL) {
    fprintf(stderr, "Ficheiro invalido.\n");
    exit(1);
  }

  /* check for valid range of values and valid type of move */
  if (vid == NULL) {
    vid = bogus;
    fprintf(stderr, "Argumentos invalidos: Identificador de viatura nulo!\n");
    fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
            vid, tk, pX, pY, pZ, tm);
    retval = -1;
  } else if (tk < 0) {
    fprintf(stderr, "Argumentos invalidos: tempo negativo!\n");
    fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
            vid, tk, pX, pY, pZ, tm);
    retval = -1;
  } else if (pX <0 || pY <0 || pZ <0) {
    fprintf(stderr, "Argumentos invalidos: coordenadas erradas!\n");
    fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
            vid, tk, pX, pY, pZ, tm);
    retval = -1;
  } else if (tm != 'i' && tm != 'x' && tm != 'e' && tm != 'm' && tm != 'p'
             && tm != 's' && tm != 'a') {
    fprintf(stderr, "Argumentos invalidos: tipo de movimento!\n");
    fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
            vid, tk, pX, pY, pZ, tm);
    retval = -1;
  } else {
    if (ptk != -1) { /* 2nd execution */
      /* Note: this is abusive; this assumes that identifiers for different
       * are in different memory locations.
       * If this is not the case, comment out this test!
       */
      if (!strcmp(vid, pvid)) {
        /* same vehicle as in last call; exclude summary and exit lines */
        if ((tm == 'i') || (tm == 'm') || (tm == 'e') ||
            (tm == 'p') || (tm == 'a')) {
          if (tk <= ptk) {
            /* invalid tk */
            fprintf(stderr, "tk deve ser maior que %d.\n", ptk);
            fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
                    vid, tk, pX, pY, pZ, tm);
            retval = -1;
          } else  if (pZ == ppz && pX == ppx && pY == ppy) {
            /* it did not move at all */
            fprintf(stderr, "Viatura %s não se moveu.\n", vid);
            fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
                    vid, tk, pX, pY, pZ, tm);
            retval = -1;
          } else if (pZ == ppz && pX != ppx && pY != ppy) {
              /* diagonal move on same floor*/
              fprintf(stderr, "Movimento invalido: ");
              fprintf(stderr,
                      "linha e coluna nao podem mudar em simultaneo.\n");
              fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
                      vid, tk, pX, pY, pZ, tm);
              retval = -1;
            } else if (pZ != ppz && (pX != ppx || pY != ppy)) {
              /* level changes with some lateral move*/
              fprintf(stderr, "Movimento invalido: ");
              fprintf(stderr,
                      "linha e coluna devem ser iguais ao nivel anterior.\n");
              fprintf(stderr, "Chamada erronea:\t\t\t%s %d %d %d %d %c\n",
                      vid, tk, pX, pY, pZ, tm);
              retval = -1;
            }
        }
      }
    }
  }
  if (retval == 0) {
    /* keep info from this run through */
    strncpy(pvid, vid, MAX_ID_SIZE);
    ptk = tk;
    ppx = pX; ppy = pY; ppz = pZ;

    /* generate output */
    fprintf(fp, "%s %d %d %d %d %c\n", vid, tk, pX, pY, pZ, tm);
  }

  return(retval);
}

/* Interface function: */
/* Fuction responsible for proccessing the entire car file: park, remove, insert in the queue */
void process_car_file( garage *g , FILE *car_in , FILE *car_out )
{
  queue *car_queue;
  char buffer[LINE_SIZE], tag[LINE_SIZE], *taux;
  int t = -1, t_new, res, size = g->M * g->P * g->N, x, y, z, ver;
  char type;

  /* Hash table for car id -> car place convertion. */
  g->parked_cars = new_h_table(H_TABLE_SIZE, car_function, compare_car, destroy_p_car);

  /* Queue of cars waiting for a park place. */
  car_queue = new_queue();

  while ( fgets( buffer , LINE_SIZE , car_in ) != NULL ) {
    /* Check if the line read has valid info. If it hasn't ignore it. */
    if ( sscanf( buffer , "%*s %d" , &t_new) != 1 ) continue;

    /* Processes restrictions, if they exist (r_data != NULL). */
    if ( g->r_data != NULL && t < t_new ) {
      /* Check if there were any changes until the new instant. */
      res = update_restrict( g->r_data , &t , t_new );

      if ( res != 0 ) {
	if ( res == 1 ) {
	  /* Only floor restrictions changed. */
	  apply_restrict_parcial(g->r_data, g->valid_floors, g->P);
	} else {
	  /* Pos, and maybe floor, restrictions changed. */
	  apply_restrict_complete(g->r_data, g->info, size, g->valid_floors, g->P);
	  mark_invalid(g);
	}
      }
    }
    
    t = t_new;

    /* Process the line read. */
    ver = sscanf( buffer , "%s %*d %c %d %d %d" , tag, &type, &x, &y, &z );
    if(type != 'S') {
      /* A car entered the garage. */
      
      car *c = (car*) malloc(sizeof(car));
      c->tag = (char*) malloc((strlen(tag)+2) * sizeof(char));
      c->t = t;
      c->index_entry = get_index(x, y, z, g->M, g->N);
      c->exit_code = convert_type(type);
      strcpy(c->tag, tag);
      escreve_saida(car_out, c->tag, c->t, get_x(c->index_entry, g->N), get_y(c->index_entry, g->M, g->N), get_z(c->index_entry, g->N, g->M), 'i');
      
      if ( g->empty_parks > 0 && park_car(c, g, car_out, c->t) == 1 ) {
	/* Car can be parked and the parking was successful. */
	destroy_car(c);
      } else {
	insert_queue( car_queue , c);
      }

    } else {
      /* A car exited the garage. */
      
      taux = (char*) malloc((strlen(tag) + 2) * sizeof(char));
      strcpy(taux, tag);
      if(ver > 3) {
	change_id( g->info[get_index(x, y, z, g->M, g->N)], PARK_PLACE_E);
	escreve_saida(car_out, taux, t, x, y, z, 's');
	
      }else{
	remove_car(g, taux, car_out, t);
      }
      free(taux);

      g->empty_parks++;

      /* Checks if the place emptied can be used to park the first queue car. */
      if ( g->empty_parks > 0 && !isEmpty_queue(car_queue) ){
	car *ent_c = peek_queue(car_queue);
	
	if ( park_car(ent_c, g, car_out, t) == 1 ) {
	  destroy_car(ent_c);
	  remove_queue(car_queue);
	  ent_c = NULL;
	}
      }
    }
  }

  /* Libertação da fila, inclusive dos carros que não conseguiram ser estacionados. */
  destroy_queue( car_queue , destroy_car );
}

void remove_car( garage* g , char* tag , FILE* fp , int t )
{
    p_car* cl;

    cl = remove_h_table(g->parked_cars, tag);
    change_id( g->info[cl->index_park], PARK_PLACE_E );

    escreve_saida(fp, cl->tag, t, get_x(cl->index_park, g->N), get_y(cl->index_park, g->M, g->N), get_z(cl->index_park, g->M, g->N), 's');
    destroy_p_car(cl);
}

void mark_invalid( garage *g )
{
  int i;
  for(i = 0; i < 5; i++) {
    g->valid_exit[i] = 0;
  }
  for(i = 0; i < g->E; i++) {
    g->valid_entry[i] = 0;
  }
}

/* Hash function sorting the cars based on the fisrt three characters (excepting V) of the name of the car*/
int car_function( Item c1 , int size )
{
  int result;
  char* c;
  c = (char*) c1;
  if(strlen(c) >= 4){
    result = (c[1]*(32)^2) + (c[2]*(32)^1) + (c[3]*(32)^0);
  }else if(strlen(c) == 3){
    result = (c[1]*(32)^2) + (c[2]*(32)^1) + (' '*(32)^0);
  }else{
    result = (c[1]*(32)^2) + (' '*(32)^1) + (' '*(32)^0);
  }

  return result %  size;
}

int compare_car(Item c1, Item c2)
{
  if(strcmp((char*) c1, (char*) c2) == 0) return 1;
  return 0;
}

/* Interface fuction: frees all resources used by the garage. */
void destroy_garage( garage * g )
{
  int i;

  destroy_graph(g->garage_graph);

  for(i=0; i<g->E; i++){
    free(g->entries[i].name);
    free(g->entries[i].wt);
    free(g->entries[i].spt);
  }
  free(g->entries);

  for(i = 0; i < 5; i++){
    freeLinkedList(g->exits[i], destroy_access);
  }

  free(g->info);

  free(g->valid_entry);

  destroy_h_table(g->parked_cars);

  for(i = 0; i < g->P ; i++){
    freeLinkedList(g->park_spaces[i], free);
  }
  free(g->park_spaces);

  free(g->valid_floors);

  destroy_restrict(g->r_data);

  free(g);
}

void destroy_access( Item a )
{
  free(((access*) a)->name);
  free(((access*) a)->spt);
  free(((access*) a)->wt);
  free((access*) a);
}

void destroy_p_car( Item p )
{
  free(((p_car*) p)->tag);
  free((p_car*) p);
}

void destroy_car( Item c )
{
  free(((car*) c)->tag);
  free((car*) c);
}

