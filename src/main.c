#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "garage.h"

int main( int argc , char **argv )
{
  FILE *fpin, *fpin2, *fpin3, *fout2;
  garage* g;
  char extOut[] = ".pts";
  char *nomeFicheiroIn, *nomeFicheiroOut, *totake;
  
  if (argc < 3) {
    fprintf(stderr, "Too few arguments. Use: ./autopark file.cfg file.inp [file.res] \n");
    exit(0);
  }
  
  nomeFicheiroIn = argv[1];
  nomeFicheiroOut = (char *)malloc((strlen(nomeFicheiroIn)+strlen(extOut)+1)*sizeof(char));
  
  strcpy(nomeFicheiroOut,nomeFicheiroIn);
  totake = strstr( nomeFicheiroOut , ".cfg");
  strcpy(totake, extOut);
  
  fpin = fopen(argv[1], "r");
  file_check(fpin, argv[1]);
  
  g = read_entryfile(fpin);
  
  fclose(fpin);
  
  if (argc > 3){
    fpin3 = fopen(argv[3], "r");
    file_check(fpin3, argv[3]);
    
    read_restrict( g, fpin3 );
    fclose(fpin3);
  } else {
    no_restrict(g);
  }

  fpin2 = fopen(argv[2], "r");
  file_check(fpin2, argv[2]);
  fout2 = fopen(nomeFicheiroOut, "w");
  file_check(fout2, nomeFicheiroOut);
  
  process_car_file(g, fpin2, fout2);

  fclose(fout2);
  fclose(fpin2);
	
  free(nomeFicheiroOut);
  destroy_garage(g);

  exit(0);
}

