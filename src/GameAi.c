// #include "GameAi.h"
#include "../include/GameAi.h"


CellMinimax aiMovecalculated = {-1 , -1}; 

Node_n *root_minimax_n ; 


Queue queue = {
    .head = NULL ,
    .tail = NULL ,
    .size = 0
}; 



void generateLegalMoves()  {

} 
void applyMoveToBoard( GameState game_state, Cell move) {
    game_state.board[move.x][move.y] = game_state.turn->color ;  
    flipCellsOptimizedGameState(&game_state , *(game.player2)) ; 
    game_state.turn = game_state.turn->next ; 
    game_state.possible_moves_stack.length = 0 ; 
    game_state.last_played_move.x = move.x ; 
    game_state.last_played_move.y = move.y ; 
}



CellsColor getCellsRemainingCountGame(GameState game_state) {

    int count = 0 ;
    int count_white = 0 ;
    int count_black = 0 ;

    CellsColor total = {0} ; 

    for (size_t i = 0; i < ROWS; i++)
        for (size_t j = 0; j < COLS; j++)
            if (game_state.board[i][j]<0)
            total.empty++ ; 
            else if (game_state.board[i][j]==0)
            total.black++ ; 
            else if (game_state.board[i][j]==1)
            total.white++ ;
            else 
            total.possible++ ;

    return total ; 
}



int evaluatePositionEnhanced(GameState game_state , int maximizingPlayer) {

    int score = 0 ; 
    int matrix_eval[ROWS][COLS] = 
    { 
        {100, -20, 10, 5, 5, 10, -20, 100},
        {-20, -50, -2, -2, -2, -2, -50, -20},
        { 10,  -2, -1, -1, -1, -1,  -2,  10},
        {  5,  -2, -1, -1, -1, -1,  -2,   5},
        {  5,  -2, -1, -1, -1, -1,  -2,   5},
        { 10,  -2, -1, -1, -1, -1,  -2,  10},
        {-20, -50, -2, -2, -2, -2, -50, -20},
        {100, -20, 10, 5, 5, 10, -20, 100}
    } ; 
    int maxColor = maximizingPlayer ? 1 : 0;  // Adjust based on your color mapping
    int minColor = maximizingPlayer ? 0 : 1;

    for (size_t i = 0; i < ROWS; i++)
    {
        for (size_t j = 0; j < COLS; j++)
        {
            if (game_state.board[i][j] == maxColor )
            {
                score += matrix_eval[i][j] ; 
            } else if (game_state.board[i][j]== minColor) {
                score -= matrix_eval[i][j] ; 

            }
            
        }
        
    }
    
    return score ; 
}  

int evaluatePosition(GameState game_state) {

    int player1_pieces_count = getCellsRemainingCountGame(game_state).black ; // human plays in black 
    int ai_pieces_count = getCellsRemainingCountGame(game_state).white ; // ai plays in white

    return (player1_pieces_count - ai_pieces_count) ; 
}  




GameState getGameState() {
    GameState game_state ; 
    game_state.turn = game.current ; 
    for (size_t i = 0; i < ROWS; i++)
        for (size_t j = 0; j < COLS; j++)
            game_state.board[i][j] = grid[i][j] ; 
    
    game_state.possible_moves_stack.length = possible_cells_stack.length ; 
    for (size_t i = 0; i < possible_cells_stack.length; i++)
    {
        game_state.possible_moves_stack.cells[i].count = possible_cells_stack.cells[i].count ; 
        game_state.possible_moves_stack.cells[i].original.x = possible_cells_stack.cells[i].original.x ; 
        game_state.possible_moves_stack.cells[i].original.y = possible_cells_stack.cells[i].original.y ; 
        game_state.possible_moves_stack.cells[i].possible.x = possible_cells_stack.cells[i].possible.x ; 
        game_state.possible_moves_stack.cells[i].possible.y = possible_cells_stack.cells[i].possible.y ; 
    }
    game_state.last_played_move.x = last_played_cell.x; 
    game_state.last_played_move.y = last_played_cell.y; 
    return game_state ; 
}


void clearPossibleChancesGameState(GameState game_state) {
    for (size_t i = 0; i < ROWS; i++)
        for (size_t j = 0; j < COLS; j++)
            if (game_state.board[i][j]==2)
                game_state.board[i][j] = -1 ; 
    
}

void addNewPossibleCellGameState(GameState *game_state,Cell origin , Cell poss) {
    game_state->possible_moves_stack.cells[game_state->possible_moves_stack.length].original.x = origin.x ; 
    game_state->possible_moves_stack.cells[game_state->possible_moves_stack.length].original.y = origin.y ;

    game_state->possible_moves_stack.cells[game_state->possible_moves_stack.length].possible.x = poss.x ; 
    game_state->possible_moves_stack.cells[game_state->possible_moves_stack.length].possible.y = poss.y ;

    game_state->possible_moves_stack.cells[game_state->possible_moves_stack.length].count= MAX(poss.x-origin.x ,poss.y-origin.y ) -1 ;

    game_state->possible_moves_stack.length++ ;

}

void setPossibleChancesGameState(GameState game_state ,Player player) {
    Directions directions_possible[8]  ; 
    
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            if (game_state.board[i][j] < 0)
                continue;
            

            for (size_t k = 0; k < 8; k++)
            {
                directions_possible[k].possibility = 1 ; 
                directions_possible[k].direct = k ; 
            }

            if ( i == 0) {
                directions_possible[UP].possibility = 0 ; 
                directions_possible[UP_RIGHT].possibility = 0 ; 
                directions_possible[UP_LEFT].possibility = 0 ; 

            }
            if ( i == 7) {
                directions_possible[DOWN].possibility = 0 ; 
                directions_possible[DOWN_LEFT].possibility = 0 ; 
                directions_possible[DOWN_RIGHT].possibility = 0 ; 

            }
            if ( j == 0) {
                directions_possible[LEFT].possibility = 0 ; 
                directions_possible[UP_LEFT].possibility = 0 ; 
                directions_possible[DOWN_LEFT].possibility = 0 ; 
                
            }
            if ( j == 7) {
                directions_possible[RIGHT].possibility = 0 ; 
                directions_possible[UP_RIGHT].possibility = 0 ; 
                directions_possible[DOWN_RIGHT].possibility = 0 ; 

            }

            if (game_state.board[i][j]==player.color)
            {
                int k = 0 ; 
                while (k < 8)
                {
                    if (!directions_possible[k].possibility)
                    {
                        k++ ; 
                         continue;
                    }   
                    
                    int look_x =  i  ;
                    int look_y =  j  ;
                    int x = 0 ; 
                    int y = 0 ; 
                    switch (directions_possible[k].direct)
                    {
                    case UP:
                        x = -1 ; 
                        y = 0 ; 
                        break;
                    case DOWN:
                        x = 1 ; 
                        y = 0 ; 
                        /* code */
                        break;
                    case RIGHT:
                        x = 0 ; 
                        y = 1 ; 
                        /* code */
                        break;
                    case LEFT:
                        x = 0 ; 
                        y = -1 ; 
                        /* code */
                        break;
                    case UP_RIGHT:
                        x = -1 ; 
                        y = 1 ; 
                        break;
                    case UP_LEFT:
                        x = -1 ; 
                        y = -1 ; 
                        /* code */
                        break;
                    case DOWN_RIGHT:
                        x = 1 ; 
                        y = 1 ; 
                        /* code */
                        break;
                    case DOWN_LEFT:
                        x = 1 ; 
                        y = -1 ; 
                        break;
                    
                    default:
                        break;
                    }


                    int u = 0 ;          

                     while ( look_y + y < 8 && look_y + y > -1 && look_x + x < 8 && look_x + x > -1)
                    {
                        if (game_state.board[look_x+x][look_y+y]==-1 )
                            if (u > 0) {
                                addNewPossibleCellGameState(&game_state ,(Cell) {i , j} ,(Cell) {look_x+x , look_y+y}) ; 
                                game_state.board[look_x+x][look_y+y]=2 ; 
                                break;

                            }
                            else 
                                break;    
                        else if ((game_state.board[look_x+x][look_y+y]==player.color  )) // || game_state.board[look_x+x][look_y+y]== 2 
                        break;
                        else if ((game_state.board[look_x+x][look_y+y]==2  )) 
                        {
                            addNewPossibleCellGameState(&game_state,(Cell) {i , j} ,(Cell) {look_x+x , look_y+y});
                            break;
                            
                        }
                        look_x += x ; 
                        look_y += y ; 
                        u++ ; 
                    } 


                    ++k ; 
                     

                }
                
            }
            
        }
        
    }
    
}



void flipCellsOptimizedGameState(GameState *game_state, Player player ) {
    if ( game_state->last_played_move.x < 0 || game_state->last_played_move.y < 0)
        return ; 

    int p = 0 ; 

    for (size_t i = 0; i < game_state->possible_moves_stack.length; i++)
    {
        if (game_state->last_played_move.x == game_state->possible_moves_stack.cells[i].possible.x &&
            game_state->last_played_move.y == game_state->possible_moves_stack.cells[i].possible.y
        )
        {
            int key = i ; 
            int origin_x = game_state->possible_moves_stack.cells[key].original.x   ; 
            int origin_y = game_state->possible_moves_stack.cells[key].original.y   ; 
            int delta_x = (game_state->possible_moves_stack.cells[key].possible.x - game_state->possible_moves_stack.cells[key].original.x)  ; 
            int delta_y = (game_state->possible_moves_stack.cells[key].possible.y - game_state->possible_moves_stack.cells[key].original.y)  ; 
            int c = 1 ; 
            // printf("delta_x=%d delta_y=%d\n" , delta_x , delta_y) ; 
            while (c < abs(delta_x) && c < abs(delta_y))
            {
                if (delta_x >= 0 && delta_y >= 0)
                game_state->board[origin_x + c][origin_y+c] = player.color ;
                else if (delta_x >= 0)
                game_state->board[origin_x + c][origin_y-c] = player.color ;
                else if (delta_y >= 0)
                game_state->board[origin_x - c][origin_y+c] = player.color ;
                else 
                game_state->board[origin_x - c][origin_y-c] = player.color ;

                c++ ; 
            }
            while (c < abs(delta_x) )
            {
                if (delta_x>=0)
                game_state->board[origin_x + c][origin_y] = player.color ;
                else
                game_state->board[origin_x - c][origin_y] = player.color ;

                c++ ; 
            }
            
            while (c < abs(delta_y) )
            {
                if (delta_y>=0)
                game_state->board[origin_x ][origin_y+c] = player.color ;
                else
                game_state->board[origin_x ][origin_y-c] = player.color ;

                c++ ; 
            }
            
            p++ ;
        }
        
    }
    

}



int isGameOver(GameState game_state) {
    // GameState game_state = getGameState(); 

    int poss = getCellsRemainingCountGame(game_state).possible ; 

    if (poss > 0)
        return false ; 

    clearPossibleChancesGameState(game_state) ; 
    
    game_state.possible_moves_stack.length = 0 ; 
    game_state.turn = game_state.turn->next ; 
    
    setPossibleChancesGameState(game_state , *game_state.turn);

    poss = getCellsRemainingCountGame(game_state).possible ; 

    if (poss > 0)
        return false ; 
    
    return true ; 

}



// Add this function to create a copy of the game state
GameState copyGameState(GameState original) {
    GameState copy;
    
    // Copy basic fields
    copy.turn = original.turn;
    copy.last_played_move.x = original.last_played_move.x;
    copy.last_played_move.y = original.last_played_move.y;
    
    // Copy the grid
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            copy.board[i][j] = original.board[i][j];
        }
    }
    
    // Copy the possible moves stack (you'll need to implement this based on your stack structure)
    copy.possible_moves_stack.length = original.possible_moves_stack.length;
    for (size_t i = 0; i < original.possible_moves_stack.length; i++) {
        copy.possible_moves_stack.cells[i] = original.possible_moves_stack.cells[i];
    }
    
    return copy;
}



void initQueue() {
    
}

void addToQueue(CellMinimax data) {
    Node *temp = malloc(sizeof(Node)) ; 

    temp->node.move.y = data.move.x ; 
    temp->node.move.y = data.move.y ; 
    temp->next = NULL ; 


    if (!queue.size)
    {
        queue.head = temp ; 
        queue.tail = temp ;
        queue.size++ ;  
    } else {
        queue.tail->next = temp ; 
        queue.tail = temp ; 
        queue.size++ ; 
    }
    
}

void printQueue() {
    printf("Moves by weights : { ") ; 
    Node *temp = queue.head ; 
    for (size_t i = 0; i < queue.size; i++)
    {
        printf(" %d " , temp->node.weight) ; 
        temp = temp->next ; 
    }printf("}\n");
    
}

void cleanQueue() {
    Node *temp = queue.head ; 
    for (size_t i = 0; i < queue.size; i++)
    {
        Node *deleted = temp;
        temp = temp->next ; 
        free(temp) ; 
    }printf("}\n");
    queue.head = NULL ; 
    queue.tail = NULL ; 
    queue.size = 0 ; 
        
}




CellMinimax minimax(GameState game_state, int depth, int maximizingPlayer) {


    
    
    // Base case
    if (depth == 0 || isGameOver(game_state)) {
        // int score = evaluatePositionEnhanced(game_state);
        int score = evaluatePositionEnhanced(game_state , maximizingPlayer);
        

        printf(" %d " , evaluatePositionEnhanced(game_state , maximizingPlayer)) ; 
        
        return (CellMinimax) {
            .weight = score, 
            .move.x = game_state.last_played_move.x,
            .move.y = game_state.last_played_move.y
        }; 
    }

    if (maximizingPlayer) {
        CellMinimax maxeval = {.weight =(int) -INFINITY, .move.x = -1, .move.y = -1};
        
        if (game_state.possible_moves_stack.length == 0) {
            GameState nextState = game_state; // Make a proper copy if needed
            CellMinimax temp = minimax(nextState, depth - 1, false);
            return temp;
        }
        
        for (size_t i = 0; i < game_state.possible_moves_stack.length; i++) {
            GameState newState = copyGameState(game_state);
            
            // applyMoveToBoard(newState, newState.possible_moves_stack.cells[i].possible);
            newState.board[newState.possible_moves_stack.cells[i].possible.x][newState.possible_moves_stack.cells[i].possible.y] = newState.turn->color ;  
            flipCellsOptimizedGameState(&newState , *(game.player2)) ; 
            newState.turn = newState.turn->next ; 
            newState.possible_moves_stack.length = 0 ; 
            newState.last_played_move.x = newState.possible_moves_stack.cells[i].possible.x ; 
            newState.last_played_move.y = newState.possible_moves_stack.cells[i].possible.y ;             
            CellMinimax evalTemp = minimax(newState, depth - 1, false);

            evalTemp.move.x = game_state.possible_moves_stack.cells[i].possible.x;
            evalTemp.move.y = game_state.possible_moves_stack.cells[i].possible.y;

            if (evalTemp.weight > maxeval.weight) {
                maxeval.weight = evalTemp.weight;
                maxeval.move.x = evalTemp.move.x;
                maxeval.move.y = evalTemp.move.y;
            }
            
        }
        // addToQueue(maxeval) ; 
        return maxeval;
        
    } else {
        CellMinimax mineval = {.weight =(int) INFINITY, .move.x = -1, .move.y = -1};
        
        if (game_state.possible_moves_stack.length == 0) {
            GameState nextState = game_state; // Make a proper copy if needed
            CellMinimax temp = minimax(nextState, depth - 1, true);
            return temp;
        }
        
        for (size_t i = 0; i < game_state.possible_moves_stack.length; i++) {
            GameState newState = copyGameState(game_state);
            
            // applyMoveToBoard(newState, newState.possible_moves_stack.cells[i].possible);
            newState.board[newState.possible_moves_stack.cells[i].possible.x][newState.possible_moves_stack.cells[i].possible.y] = newState.turn->color ;  
            flipCellsOptimizedGameState(&newState , *(game.player2)) ; 
            newState.turn = newState.turn->next ; 
            newState.possible_moves_stack.length = 0 ; 
            newState.last_played_move.x = newState.possible_moves_stack.cells[i].possible.x ; 
            newState.last_played_move.y = newState.possible_moves_stack.cells[i].possible.y ;             
            
            CellMinimax evalTemp = minimax(newState, depth - 1, true);
            
            evalTemp.move.x = game_state.possible_moves_stack.cells[i].possible.x;
            evalTemp.move.y = game_state.possible_moves_stack.cells[i].possible.y;

            if (evalTemp.weight < mineval.weight) {
                mineval.weight = evalTemp.weight;
                mineval.move.x = evalTemp.move.x;
                mineval.move.y = evalTemp.move.y;
            }
        }
        // addToQueue(mineval) ; 

        return mineval;
    }
}




CellMinimax minimaxalpha(GameState game_state, int depth , int alpha , int beta, int maximizingPlayer) {
    // Base case
    if (depth == 0 || isGameOver(game_state)) {
        // int score = evaluatePositionEnhanced(game_state , maximizingPlayer);
        int score = evaluatePosition(game_state);
        return (CellMinimax) {
            .weight = score, 
            .move.x = game_state.last_played_move.x,
            .move.y = game_state.last_played_move.y
        }; 
    }

    if (maximizingPlayer) {
        CellMinimax maxeval = {.weight =(int) -INFINITY, .move.x = -1, .move.y = -1};
        
        if (game_state.possible_moves_stack.length == 0) {
            GameState nextState = game_state; 
            CellMinimax temp = minimaxalpha(nextState, depth  - 1 , alpha , beta, false);
            return temp;
        }
        
        for (size_t i = 0; i < game_state.possible_moves_stack.length; i++) {
            GameState newState = copyGameState(game_state);
            
            applyMoveToBoard(newState, newState.possible_moves_stack.cells[i].possible);
            
            CellMinimax evalTemp = minimaxalpha(newState, depth  - 1 , alpha , beta, false);
            
            evalTemp.move.x = game_state.possible_moves_stack.cells[i].possible.x;
            evalTemp.move.y = game_state.possible_moves_stack.cells[i].possible.y;

            if (evalTemp.weight > maxeval.weight) {
                maxeval.weight = evalTemp.weight;
                maxeval.move.x = evalTemp.move.x;
                maxeval.move.y = evalTemp.move.y;
            }
            if (maxeval.weight >= beta)
            {
                break;
            }
            
            alpha = MAX(alpha , maxeval.weight) ; 
            
            
        }
        return maxeval;
        
    } else {
        CellMinimax mineval = {.weight =(int) INFINITY, .move.x = -1, .move.y = -1};
        
        if (game_state.possible_moves_stack.length == 0) {
            GameState nextState = game_state; // Make a proper copy if needed
            CellMinimax temp = minimaxalpha(nextState, depth  - 1 , alpha , beta, false);
            return temp;
        }
        
        for (size_t i = 0; i < game_state.possible_moves_stack.length; i++) {
            GameState newState = copyGameState(game_state);
            
            applyMoveToBoard(newState, newState.possible_moves_stack.cells[i].possible);
            
            CellMinimax evalTemp = minimaxalpha(newState, depth  - 1 , alpha , beta, false);
            
            evalTemp.move.x = game_state.possible_moves_stack.cells[i].possible.x;
            evalTemp.move.y = game_state.possible_moves_stack.cells[i].possible.y;

            if (evalTemp.weight < mineval.weight) {
                mineval.weight = evalTemp.weight;
                mineval.move.x = evalTemp.move.x;
                mineval.move.y = evalTemp.move.y;
            }
            if (mineval.weight <= alpha )
            {
                break;
            }
            
            beta = MIN(beta , mineval.weight) ; 
            
            
        }
        return mineval;
        }

}



void traverseTree_n(Node_n *root) {
    if (root == NULL)
        return;

    // Simple array-based queue
    Node_n *queue[100];  // fixed size for simplicity
    int front = 0, rear = 0;

    queue[rear++] = root;

    while (front < rear) {
        Node_n *current = queue[front++];

        printf("%d  depth : %d   childs : %d    %s player \n", evaluatePositionEnhanced(current->data , current->maximizing_player)  , current->depth, current->n    , (current->maximizing_player ? "max" : "min"));
        for (size_t i = 0; i < current->n; i++)
        {
            
            queue[rear++] = current->child[i];
            
            /* code */
        }

    }
}
Node_n *createNode_n(int data) {
    Node_n*temp = malloc(sizeof(Node_n)) ; 

    
    temp->n = 0 ;
    
    // temp->data = data ; 

    return temp ;

}



// Initialize a node properly
void initializeNode(Node_n *node) {
    node->n = 0;
    node->depth = 0;
    for (int i = 0; i < MAX_CHILDREN; i++) {
        node->child[i] = NULL;
    }
}
