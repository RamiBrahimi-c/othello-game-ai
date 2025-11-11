#include "../include/Game.h"
// #include "Game.h"
#include <math.h>

#define true 1
#define false 0

// #define INFINITY 0xfffffff 


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



typedef struct 
{
    GameState game_state;
    int depth;
    int maximizingPlayer ; 
}ArgsMinimax;



typedef struct Node
{
    CellMinimax node ; 
    struct Node *next ;     
}Node;


#define MAX_CHILDREN 1000

typedef struct Node_n
{
    GameState data ; 
    int n ;
    int depth ;
    int maximizing_player ;
    struct Node_n *child[MAX_CHILDREN] ;     
}Node_n;




typedef struct Queue
{
    Node *head ;  
    Node *tail ;
    int size ;   
}Queue;



extern Queue queue ; 

extern Node_n *root_minimax_n ; 
extern  CellMinimax aiMovecalculated ; 


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

void* minimaxThread(void *arg) ;

CellMinimax minimaxalpha(GameState game_state, int depth , int alpha , int beta, int maximizingPlayer) ;

int evaluatePositionEnhanced(GameState , int ) ; 




void initQueue() ;

void addToQueue(CellMinimax data) ;


void printQueue() ;


void cleanQueue() ;


GameState copyGameState(GameState ) ; 

void renderTree_n( Node_n *root) ;
void traverseTree_n(Node_n *root) ;
Node_n *createNode_n(int data) ;

CellMinimax minimax_n(GameState game_state, int depth, int maximizingPlayer , Node_n *root) ;

Node_n *createNode_n(int data) ;