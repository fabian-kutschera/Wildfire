#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void printHeader() {
  printf( " ------------------\n" );
  printf( "  PROJECT WILDFIRE\n" );
  printf( " ------------------\n" );
}

void printInstructions() {
  printHeader();
  printf( "\n To simulate a wildfire please specify:\n\n" );
  printf( "  --size=N ..... to use a forest of N-by-N cells\n" );
  printf( "  --alpha=v .... with v being likelyhood that a tree grows\n" );
  printf( "  --gamma=v .... with v being likelyhood that lightning strikes\n");
  printf( "  --steps=N .... to perform N steps of the simulation\n\n");
}

void printParams( int size, double alpha, double gamma, int steps,
                  char* filename ) {
  printHeader();
  printf( "\n Running a wildfire simulation using\n\n" );
  printf( "  * size = %d\n", size );
  printf( "  * alpha = %f\n", alpha );
  printf( "  * gamma = %f\n", gamma );
  printf( "  * steps = %d\n", steps );
  printf( "\n Writing output to '%s'.\n\n", filename );
}

// Enumeration type describing the value of a cell
typedef enum{ ASHES = 0, TREE = 1, FIRE = 2 } cell_t;

// Datatype for a 2D simulation domain
typedef struct {
  cell_t** cells;
  int size;
} forest_t;

// Roll the dice and let chance decide what happens
bool rollDice( double luckyValue ) {
  double randVal = (double) rand() / (double)RAND_MAX;
  return randVal < luckyValue;
}

// Initialise the 2D simulation domain by setting all cells to ASHES
void initForest( forest_t* forest ){
  //forest->cells[forest->size][forest->size] = {ASHES}; //Why can't I use sth like this?
  for( int k = 0; k < forest->size; k++)
    for(int m = 0; m < forest->size; m++)
      forest->cells[k][m] = ASHES;
  /* YOUR CODE HERE - DONE? */
}

// Export the state of the simulation at given step into the given file
void exportForest( FILE* outfile, forest_t* forest, int step ) {
  fprintf( outfile, "# Forest at step %d\n", step );
  for( int k = 0; k < forest->size; k++ ) {
    for( int m = 0; m < forest->size - 1; m++ ) {
      fprintf( outfile, "%d ", forest->cells[k][m] );
    }
    fprintf( outfile, "%d\n", forest->cells[k][forest->size-1] );
  }
  fprintf( outfile, "\n" );
}

// Compute the new state from the previous one given probabilities for
// alpha (tree growth) and gamma (lighting strike)
void updateForest( forest_t* forestOld, forest_t* forestNew, double alpha,
                  double gamma ) {
  // I think we need if statements
  // I think we need rolldice()
  /* YOUR CODE HERE - it's probably really inefficient but it works*/
  for( int k = 1; k < forestOld->size-1; k++) {
    for(int m = 1; m < forestOld->size-1; m++) {
      if (forestOld->cells[k][m] == ASHES) {
	if (rollDice(alpha)) {
	  forestNew->cells[k][m] = TREE;
	}
	//else forestNew->cells[k][m] = ASHES;
      }
      else if (forestOld->cells[k][m] == TREE) {
	//if ((k>0 && m>0) && (k<forestOld->size && m<forestOld->size)) 
	if (forestOld->cells[k+1][m]==FIRE || forestOld->cells[k-1][m]==FIRE || forestOld->cells[k][m+1]==FIRE || forestOld->cells[k][m-1]==FIRE) {
	  forestNew->cells[k][m] = FIRE;
	}
	else if (rollDice(gamma)) {
	  forestNew->cells[k][m] = FIRE;
	}
	//else forestNew->cells[k][m] = TREE;
      }
      else //if (forestOld->cells[k][m] == FIRE)
	{forestNew->cells[k][m] = ASHES;}
    }
  }
}


// Dynamically allocate memory for the cells of the 2D simulation domain
cell_t** allocCells( int size ) {

  cell_t** buffer = (cell_t**) malloc( size * sizeof(cell_t*) );
  if ( buffer == NULL ) {
    fprintf( stderr, "Memory allocation #1 failed in allocCells()!\n" );
    exit( EXIT_FAILURE );
  }

  buffer[0] = (cell_t*) malloc( size*size*sizeof(cell_t) );
  if ( buffer[0] == NULL ) {
    fprintf( stderr, "Memory allocation #2 failed in allocCells()!\n" );
    exit( EXIT_FAILURE );
  }

  for ( int k = 1; k < size; k++ ) {
    buffer[k] = buffer[0] + k * size;
  }

  return buffer;
}

// Free dynamically allocate memory for the cells of the 2D simulation domain
// and re-set forest data structure to empty
void freeCells( forest_t* forest ) {
  free(forest);
  /* I think the freeing is not done properly - execution returns "munmap_chunk(): invalid pointer",
     but running the python script works fine */
  /* YOUR CODE HERE - DONE?*/
}

// Driver routine
int main( int argc, char* argv[] ) {

  int size = 0;
  int numSteps = 0;
  double alpha = 0.0;
  double gamma = 0.0;

  // set name of output file
  char filename[] = "wildfire.data";

  // process command-line arguments
  if( argc != 5 ) {
    printInstructions();
  }
  else {
    sscanf( argv[1], "--size=%d", &size );
    sscanf( argv[2], "--alpha=%lf", &alpha );
    sscanf( argv[3], "--gamma=%lf", &gamma );
    sscanf( argv[4], "--steps=%d", &numSteps );

    printParams( size, alpha, gamma, numSteps, filename );
  }

  // initialise one forest to hold the current simulation state
  forest_t forest = {NULL, 0};
  forest.size = size;
  forest.cells = allocCells( size );
  initForest( &forest );

  // initialise another auxilliary forest for updating the simulation
  // state
  forest_t aux = {NULL, 0};
  aux.size = size;
  aux.cells = allocCells( size );
  initForest( &aux );

  // open datafile for writing and export initial state
  FILE *of = fopen( filename, "w" );
  if( of == NULL ) {
    fprintf( stderr, "Failed to open datafile '%s'\n", filename );
    exit( EXIT_FAILURE );
  }
  exportForest( of, &forest, 0 );

  // initialise, i.e. set seed, for random number generator
  srand( time( NULL ) );

  // perform the simulation steps
  for( int k = 1; k <= numSteps; k++ ) {

    // compute next state
    updateForest( &forest, &aux, alpha, gamma );

    // update cell states in forest from that in aux
    /* YOUR CODE HERE */
    //forest = aux;
    memcpy(&forest, &aux, sizeof(forest));
    
    // export the new state
    exportForest( of, &forest, k );
  }

  // close output file
  if( fclose( of ) != 0  ) {
    fprintf( stderr, "Problems closing datafile '%s'\n", filename );
    exit( EXIT_FAILURE );
  }

  // deallocate memory
  freeCells( &forest );
  freeCells( &aux );

  if( forest.cells != NULL ) {
    fprintf( stderr, " NOTE: You forgot to implement something!\n" );
    exit( EXIT_FAILURE );
  }
  
}
