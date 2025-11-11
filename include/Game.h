#pragma once

#include <stdio.h>
#include <stdlib.h>

#define CELL_WIDTH 45
#define CELL_HEIGHT 45
#define CIRCLE_RADIUS 15
#define ROWS 8
#define COLS 8
#define PX 120
#define PY 80



typedef enum {
    HUMAN , 
    AI
}PlayerType;



typedef struct Player 
{
    char id[20] ; 
    char color ;
    PlayerType type ; 
    struct Player *next ; 
}Player;



typedef struct 
{
    char mode ; 
    char ongoing ; 
    char state; 
    Player *winner ; 
    Player *player1 ; 
    Player *player2 ;
    Player *current ;  
}Game;




typedef enum {
    UP ,
    DOWN ,
    RIGHT ,
    LEFT , 
    UP_RIGHT , 
    UP_LEFT , 
    DOWN_RIGHT , 
    DOWN_LEFT , 
}DIRECTIONS;


typedef struct 
{
    DIRECTIONS direct ; 
    int possibility ; 
}Directions;


typedef struct 
{
    int x ;
    int y ;
}Cell;


typedef struct 
{
    Cell original ; // first cell
    Cell possible ; // possible cell to be owned by current player
    int count ; // number of cells between 'original' and 'possible'
}PossibleCells;

#define MAX_POSSIBLE_CELLS 100


#define MAX(X,Y) (X>Y) ? X : Y
#define MIN(X,Y) (X>Y) ? Y : X


typedef struct 
{
    PossibleCells cells[MAX_POSSIBLE_CELLS] ;
    int length ; 
}PossibleCellsStack;

typedef struct 
{
    int black ;
    int white ;
    int possible ;
    int empty ;
}CellsColor;


extern int grid[ROWS][COLS]  ; 
extern Cell last_played_cell  ; 
extern int turn  ; 
extern Game game  ;
extern PossibleCellsStack possible_cells_stack  ; 





void addNewPossibleCell(Cell  , Cell ) ;
void initGrid() ;
CellsColor getCellsRemainingCount() ;
void setPossibleChances(Player ) ;
void clearPossibleChances() ;
void flipCells(Player ) ; 
int abs(int   ) ;
void clearPossibleCellArray() ;
void flipCellsOptimized(Player  ) ; 
void printPossibleCellsArray() ; 
