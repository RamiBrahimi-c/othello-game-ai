#include "Game.h"


#define true 1
#define false 0

#define INFINITY 0xfffffff 


typedef struct 
{
    int board[ROWS][COLS] ; 
    Player *turn ; 
    PossibleCellsStack possible_moves_stack ; 
    Cell last_played_move ; 
}GameState;

typedef struct 
{
    Cell move ;
    int weight ; 
}CellMinimax;


void generateLegalMoves()  ;

void applyMoveToBoard( GameState , Cell );

CellsColor getCellsRemainingCountGame(GameState );



int evaluatePosition(GameState );

GameState getGameState() ;

void clearPossibleChancesGameState(GameState ) ;

void addNewPossibleCellGameState(GameState *,Cell  , Cell ) ;

void setPossibleChancesGameState(GameState ,Player ) ;

void flipCellsOptimizedGameState(GameState *game_state, Player player ) ;

int isGameOver(GameState ) ;


CellMinimax minimax(GameState  ,int depth ,int maximizingPlayer) ;