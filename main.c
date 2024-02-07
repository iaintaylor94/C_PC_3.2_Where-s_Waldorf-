#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <math.h>
#include <string.h>

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
void getWord (int, char *, struct dictionary);
void updateDictionary (int, int, int, struct dictionary *);

// Word Search
void reverseString (char *);
void findWords (struct grid, struct dictionary);

void getLineHorizontal (int, char*);
void searchLineHorizontal (char*, int, bool);
void searchHorizontal (void);

void getLineVertical (int, char*);
void searchLineVertical (char*, int, bool);
void searchVertical (void);

void getLineDiagonalLD (int, char *);
void searchLineDiagonalLD (int, char *);
void findLD (int);

void getLineDiagonalLU (int, char *);
void searchLineDiagonalLU (int, char *);
void findLU (int);

void getLineDiagonalRD (int, char *);
void searchLineDiagonalRD (int, char *);
void findRD (int);

void getLineDiagonalRU (int, char *);
void searchLineDiagonalRU (int, char *);
void findRU (int);

void getLineDiagonalTR (int, char *);
void searchLineDiagonalTR (int, char *);
void findTR (int);

void getLineDiagonalTL (int, char *);
void searchLineDiagonalTL (int, char *);
void findTL (int);

void getLineDiagonalBR (int, char *);
void searchLineDiagonalBR (int, char *);
void findBR (int);

void getLineDiagonalBL (int, char *);
void searchLineDiagonalBL (int, char *);
void findBL (int);



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

  findWords (gWordGrid, gDictionary);
  printDictionary (gDictionary);










  


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
      g->wordGrid[i][j] = tolower(line[j - 1]);
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

    for (int i = 0; i < strlen(line); i ++) {
      line[i] = tolower(line[i]);
    }
    
    sscanf (line, "%s", d->entries[i].word);
    d->entries[i].mCoord = -1;
    d->entries[i].nCoord = -1;
  }
}
void printDictionary (struct dictionary d) {
  printf ("DICTIONARY  \n");
  printf ("Name                  mC  nC\n");
  printf ("--------------------  --  --\n");
  for (int i = 0; i < d.numWords; i ++) {
    printf ("%-20s  %2d  %2d\n", d.entries[i].word, d.entries[i].mCoord, d.entries[i].nCoord);
  }
  printf ("\n");
}
void getWord (int i, char *line, struct dictionary d) {
  strcpy (line, d.entries[i].word);
}
void updateDictionary (int i, int m, int n, struct dictionary *d) {
  d->entries[i].mCoord = m;
  d->entries[i].nCoord = n;
}


// Word Search
void reverseString (char *line) {
  for (int i = 0, j = strlen(line); i < j; i++, j--) {
    char temp = line[i];
    line[i] = line[j];
    line[j] = temp;
  }
}

void getLineHorizontal (int m, char *line) {
  strcpy (line, &gWordGrid.wordGrid[m][1]);
}
void searchLineHorizontal (char *line, int m, bool forwards) {
  for (int i = 0; i < gDictionary.numWords; i++) {
    char *position = strstr(line, gDictionary.entries[i].word);
    if (position != NULL) {
      int offset = position - line;
      if (forwards) {
        updateDictionary(i, m, offset + 1, &gDictionary);
      }
      else {
        updateDictionary(i, m, strlen(line) - offset + 1, &gDictionary);
      }
    }
  }
}
void searchHorizontal () {
  for (int m = 0; m < gWordGrid.gridHeight; m++) {
    char line [kMaxLineLength];
    getLineHorizontal (m, line);
    searchLineHorizontal (line, m, true);
    reverseString (line);
    searchLineHorizontal (line, m, false);
  }
}


void getLineVertical (int n, char *line) {
  for (int i = 1; i < gWordGrid.gridHeight + 2; i ++) {
    line[i - 1] = gWordGrid.wordGrid[i][n];
  }
}
void searchLineVertical (char *line, int n, bool forwards) {
  for (int i = 0; i < gDictionary.numWords; i++) {
    char *position = strstr(line, gDictionary.entries[i].word);
    if (position != NULL) {
      int offset = position - line;
      if (forwards) {
        updateDictionary(i, offset + 1, n, &gDictionary);
      }
      else {
        updateDictionary(i, strlen(line) - offset + 1, n, &gDictionary);
      }
    }
  }
}
void searchVertical () {
  for (int n = 0; n < gWordGrid.gridWidth; n++) {
    char line [kMaxLineLength];
    getLineVertical (n, line);
    searchLineVertical (line, n, true);
    reverseString (line);
    searchLineVertical (line, n, false);
  }
}


void getLineDiagonalLD (int m, char *line) {
  for (int n = 1; gWordGrid.wordGrid[m][n] != '\0'; m--, n++) {
    line [n - 1] = gWordGrid.wordGrid[m][n];
    line [n] = '\0';
  }
}
void searchLineDiagonalLD (int m, char *line) {
  for (int i = 0; i < gDictionary.numWords; i++) {
    char *position = strstr(line, gDictionary.entries[i].word);
    int offset = position - line;
    if (position != NULL) {
      updateDictionary(i, m - offset, 1 + offset, &gDictionary);
    }
  }
}
void findLD (int m) {
  char line [kMaxLineLength];
  getLineDiagonalLD (m, line);
  searchLineDiagonalLD (m, line);
}

void getLineDiagonalLU (int m, char *line) {
  for (int n = 1; gWordGrid.wordGrid[m][n] != '\0'; m++, n++) {
    line [n - 1] = gWordGrid.wordGrid[m][n];
    line [n] = '\0';
  }
}
void searchLineDiagonalLU (int m, char *line) {
  for (int i = 0; i < gDictionary.numWords; i++) {
    char *position = strstr(line, gDictionary.entries[i].word);
    int offset = position - line;
    if (position != NULL) {
      updateDictionary(i, m + offset, 1 + offset, &gDictionary);
    }
  }
}
void findLU (int m) {
  char line [kMaxLineLength];
  getLineDiagonalLU (m, line);
  searchLineDiagonalLU (m, line);
}

void getLineDiagonalRD (int m, char *line) {
  int k = 0;
  for (int n = gWordGrid.gridWidth; gWordGrid.wordGrid[m][n] != '\0'; m--, n--) {
    line [k] = gWordGrid.wordGrid[m][n];
    line [k + 1] = '\0';
    k++;
  }
}
void searchLineDiagonalRD (int m, char *line) {
  for (int i = 0; i < gDictionary.numWords; i++) {
    char *position = strstr(line, gDictionary.entries[i].word);
    int offset = position - line;
    if (position != NULL) {
      updateDictionary(i, m - offset, gWordGrid.gridWidth - offset, &gDictionary);
    }
  }
}
void findRD (int m) {
  char line [kMaxLineLength];
  getLineDiagonalRD (m, line);
  searchLineDiagonalRD (m, line);
}


void getLineDiagonalRU (int m, char *line) {
  int k = 0;
  for (int n = gWordGrid.gridWidth; gWordGrid.wordGrid[m][n] != '\0'; m++, n--) {
    line [k] = gWordGrid.wordGrid[m][n];
    line [k + 1] = '\0';
    k++;
  }
}
void searchLineDiagonalRU (int m, char *line) {
  for (int i = 0; i < gDictionary.numWords; i++) {
    char *position = strstr(line, gDictionary.entries[i].word);
    int offset = position - line;
    if (position != NULL) {
      updateDictionary(i, m + offset, gWordGrid.gridWidth - offset, &gDictionary);
    }
  }
}
void findRU (int m) {
  char line [kMaxLineLength];
  getLineDiagonalRU (m, line);
  searchLineDiagonalRU (m, line);
}

void getLineDiagonalTR (int n, char *line) {
  int k = 0;
  for (int m = 1; gWordGrid.wordGrid[m][n] != '\0'; m++, n++) {
    line [k] = gWordGrid.wordGrid[m][n];
    line [k + 1] = '\0';
    k++;
  }
}
void searchLineDiagonalTR (int n, char *line) {
  for (int i = 0; i < gDictionary.numWords; i++) {
    char *position = strstr(line, gDictionary.entries[i].word);
    int offset = position - line;
    if (position != NULL) {
      updateDictionary(i, offset, n + offset, &gDictionary);
    }
  }
}
void findTR (int n) {
  char line [kMaxLineLength];
  getLineDiagonalTR (n, line);
  searchLineDiagonalTR (n, line);
}

void getLineDiagonalTL (int n, char *line) {
  int k = 0;
  for (int m = 1; gWordGrid.wordGrid[m][n] != '\0'; m++, n--) {
    line [k] = gWordGrid.wordGrid[m][n];
    line [k + 1] = '\0';
    k++;
  }
}
void searchLineDiagonalTL (int n, char *line) {
  for (int i = 0; i < gDictionary.numWords; i++) {
    char *position = strstr(line, gDictionary.entries[i].word);
    int offset = position - line;
    if (position != NULL) {
      updateDictionary(i, 1 + offset, n - offset, &gDictionary);
    }
  }
}
void findTL (int n) {
  char line [kMaxLineLength];
  getLineDiagonalTL (n, line);
  searchLineDiagonalTL (n, line);
}

void getLineDiagonalBR (int n, char *line) {
  int k = 0;
  for (int m = gWordGrid.gridHeight; gWordGrid.wordGrid[m][n] != '\0'; m--, n++) {
    line [k] = gWordGrid.wordGrid[m][n];
    line [k + 1] = '\0';
    k++;
  }
}
void searchLineDiagonalBR (int n, char *line) {
  for (int i = 0; i < gDictionary.numWords; i++) {
    char *position = strstr(line, gDictionary.entries[i].word);
    int offset = position - line;
    if (position != NULL) {
      updateDictionary(i, gWordGrid.gridHeight - offset, n + offset, &gDictionary);
    }
  }
}
void findBR (int n) {
  char line [kMaxLineLength];
  getLineDiagonalBR (n, line);
  searchLineDiagonalBR (n, line);
}

void getLineDiagonalBL (int n, char *line) {
  int k = 0;
  for (int m = gWordGrid.gridHeight; gWordGrid.wordGrid[m][n] != '\0'; m--, n--) {
    line [k] = gWordGrid.wordGrid[m][n];
    line [k + 1] = '\0';
    k++;
  }
}
void searchLineDiagonalBL (int n, char *line) {
  for (int i = 0; i < gDictionary.numWords; i++) {
    char *position = strstr(line, gDictionary.entries[i].word);
    int offset = position - line;
    if (position != NULL) {
      updateDictionary(i, gWordGrid.gridHeight - offset, n - offset, &gDictionary);
    }
  }
}
void findBL (int n) {
  char line [kMaxLineLength];
  getLineDiagonalBL (n, line);
  searchLineDiagonalBL (n, line);
}

void findWords (struct grid g, struct dictionary d) {
  searchHorizontal ();
  searchVertical ();
  
  for (int i = 0;  i < g.gridHeight; i++) { // LEFT / RIGHT
    //LEFT
    findLD (i);
    findLU (i);

    //RIGHT
    findRD (i);
    findRU (i);
  }
  for (int i = 0; i < g.gridWidth; i++) { // TOP / BOTTOM
    // TOP
    findTR (i);
    findTL (i);
    
    //BOTTOM
    findBR (i);
    findBL (i);
  }
}