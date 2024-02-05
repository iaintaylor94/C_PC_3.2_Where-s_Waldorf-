#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <math.h>

// File Initialization
FILE *gInputFile;
void usage (char *);

// Game Definitions
int gNumGames;
int getNumGames(FILE*);

// Word Grid
const int kMaxLineLength = 81;
const int kMaxGridSize = 52;
struct grid {
  char wordGrid [kMaxGridSize][kMaxGridSize];
  int gridWidth;
  int gridHeight;
} gWordGrid;
void fillWordGrid (struct grid *);
void printWordGrid (struct grid);

// Dictionary
const int kMaxNumWords = 20;
struct pairs {
  char word[kMaxLineLength];
  int mCoord;
  int nCoord;
};
struct dictionary {
  struct pairs entries[kMaxNumWords];
  int numWords;
} gDictionary;
void fillDictionary (struct dictionary *);
void printDictionary (struct dictionary);


int main(int argc, char *argv[]) {

  /*-------------------------------FILE INITIALIZATION START-----------------------------*/
  if (argc != 2) usage (argv[0]);

  if (NULL == (gInputFile = fopen(argv[1], "r"))) {
    fprintf (stderr, "inputFile: %s: %s\n", argv[1], strerror(errno));
    exit (EXIT_FAILURE);
  }
  else {
    fprintf (stderr, "%s opened for reading.\n\n" , argv[1]);
  }
  /*-------------------------------FILE INITIALIZATION END--------------------------------*/
  /*--------------------------------MAIN PROGRAM START------------------------------------*/

  gNumGames = getNumGames(gInputFile);
  printf ("Number of Games: %d\n", gNumGames);
  fillWordGrid(&gWordGrid);
  printWordGrid(gWordGrid);

  fillDictionary(&gDictionary);
  printDictionary(gDictionary);



  /*--------------------------------MAIN PROGRAM END--------------------------------------*/
  /*--------------------------------FILE CLEANUP START------------------------------------*/
  fclose (gInputFile);
  /*--------------------------------FILE CLEANUP END--------------------------------------*/

  
  return 0;
}

// File Initialization
void usage (char *cmd) {
  fprintf (stderr, "usage: %s inputFileName\n", cmd);
  exit (EXIT_SUCCESS);
}


// Game Definitions
int getNumGames(FILE *fp) {
  // Get num Games
  char line [kMaxLineLength];
  fgets (line, kMaxLineLength, fp);
  line [strcspn(line, "\n")] = '\0';
  int numGames = atoi(line);

  // Strip blank line
  fgets (line, kMaxLineLength, fp);
  
  // Return num Games
  return numGames;
}

// Word Grid
void fillWordGrid (struct grid *g) {
  // Get Dimensions
  char line [kMaxLineLength];
  fgets (line, kMaxLineLength, gInputFile);
  line [strcspn(line, "\n")] = '\0';
  sscanf (line, "%d %d", &g->gridHeight, &g->gridWidth);
  printf ("Grid height: %d\nGrid width: %d\n", g->gridHeight, g->gridWidth);

  // Fill Word Grid - with input
  for (int i = 1; i < g->gridHeight + 1; i ++) {
    char line [kMaxLineLength];
    fgets (line, kMaxLineLength, gInputFile);
    line [strcspn(line, "\n")] = '\0';
    
    for (int j = 1; j < g->gridWidth + 1; j ++) {
      g->wordGrid[i][j] = line[j - 1];
    }
  }
  
  // Fill Word Grid - with padding
  // Collumns
  for (int i = 0; i < g->gridHeight + 2; i ++) {
    g->wordGrid[i][0] = '\0';
    g->wordGrid[i][g->gridWidth + 2] = '\0';
  }
  // Rows
  for (int i = 0; i < g->gridWidth + 2; i ++) {
    g->wordGrid[0][i] = '\0';
    g->wordGrid[g->gridHeight + 2][i] = '\0';
  }
}
void printWordGrid (struct grid g) {
  printf ("WORD GRID  \n");
  printf ("-----------\n");
  for (int i = 1; i < g.gridHeight + 1; i ++) {
    for (int j = 1; j < g.gridWidth + 1; j ++) {
      printf ("%c", g.wordGrid[i][j]);
    }
    printf ("\n");
  }
  printf ("\n");
}

// Dictionary
void fillDictionary (struct dictionary *d) {
  // Get num words
  char line [kMaxLineLength];
  fgets (line, kMaxLineLength, gInputFile);
  line [strcspn(line, "\n")] = '\0';
  d->numWords = atoi(line);

  // Fill Dictionary
  for (int i = 0; i < d->numWords; i ++) {
    char line [kMaxLineLength];
    fgets (line, kMaxLineLength, gInputFile);
    line [strcspn(line, "\n")] = '\0';
    sscanf (line, "%s", d->entries[i].word);
  }
}
void printDictionary (struct dictionary d) {
  printf ("DICTIONARY  \n");
  printf ("------------\n");
  for (int i = 0; i < d.numWords; i ++) {
    printf ("%s\n", d.entries[i].word);
  }
  printf ("\n");
}