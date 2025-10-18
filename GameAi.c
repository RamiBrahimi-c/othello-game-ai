#include "GameAi.h"




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

/* 
void applyMoveToBoard( GameState *game_state, Cell move) {
    game_state->board[move.x][move.y] = game_state->turn->color ;  
    game_state->turn = game_state->turn->next ; 
    game_state->possible_moves_stack.length = 0 ; 
    game_state->last_played_move.x = move.x ; 
    game_state->last_played_move.y = move.y ; 
} */


CellsColor getCellsRemainingCountGame(GameState game_state) {

    int count = 0 ;
    int count_white = 0 ;
    int count_black = 0 ;

    CellsColor total = {0} ; 

    for (size_t i = 0; i < ROWS; i++)
        for (size_t j = 0; j < COLS; j++)
            if (game_state.board[i][j]<0)
            total.empty++ ; 
            else if (!game_state.board[i][j])
            total.black++ ; 
            else if (game_state.board[i][j]==1)
            total.white++ ;
            else 
            total.possible++ ;

    return total ; 
}



int evaluatePosition(GameState game_state) {

    int player1_pieces_count = getCellsRemainingCountGame(game_state).black ; // human plays in black 
    int ai_pieces_count = getCellsRemainingCountGame(game_state).white ; // ai plays in white

    return -(player1_pieces_count - ai_pieces_count) ; 
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

// Define these constants at the top of your file
#define MAX_SCORE 1000000
#define MIN_SCORE -1000000

CellMinimax minimax(GameState game_state, int depth, int maximizingPlayer) {
    // Base case
    if (depth == 0 || isGameOver(game_state)) {
        int score = evaluatePosition(game_state);
        return (CellMinimax) {
            .weight = score, 
            .move.x = game_state.last_played_move.x,
            .move.y = game_state.last_played_move.y
        }; 
    }

    if (maximizingPlayer) {
        CellMinimax maxeval = {.weight = MIN_SCORE, .move.x = -1, .move.y = -1};
        
        // Check if no moves available
        if (game_state.possible_moves_stack.length == 0) {
            // Pass turn
            GameState nextState = game_state; // Make a proper copy if needed
            // Switch player logic here if needed
            CellMinimax temp = minimax(nextState, depth - 1, false);
            return temp;
        }
        
        for (size_t i = 0; i < game_state.possible_moves_stack.length; i++) {
            // CRITICAL: Create a copy of the game state
            GameState newState = copyGameState(game_state);
            
            // Apply move to the copy, not the original
            applyMoveToBoard(newState, newState.possible_moves_stack.cells[i].possible);
            
            CellMinimax evalTemp = minimax(newState, depth - 1, false);
            
            // Store the actual move that led to this evaluation
            evalTemp.move.x = game_state.possible_moves_stack.cells[i].possible.x;
            evalTemp.move.y = game_state.possible_moves_stack.cells[i].possible.y;

            if (evalTemp.weight > maxeval.weight) {
                maxeval.weight = evalTemp.weight;
                maxeval.move.x = evalTemp.move.x;
                maxeval.move.y = evalTemp.move.y;
            }
            
            // Clean up if your copyGameState allocates memory
            // freeGameStateCopy(newState);
        }
        return maxeval;
        
    } else {
        CellMinimax mineval = {.weight = MAX_SCORE, .move.x = -1, .move.y = -1};
        
        // Check if no moves available
        if (game_state.possible_moves_stack.length == 0) {
            // Pass turn
            GameState nextState = game_state; // Make a proper copy if needed
            CellMinimax temp = minimax(nextState, depth - 1, true);
            return temp;
        }
        
        for (size_t i = 0; i < game_state.possible_moves_stack.length; i++) {
            // CRITICAL: Create a copy of the game state
            GameState newState = copyGameState(game_state);
            
            // Apply move to the copy
            applyMoveToBoard(newState, newState.possible_moves_stack.cells[i].possible);
            
            CellMinimax evalTemp = minimax(newState, depth - 1, true);
            
            // Store the actual move
            evalTemp.move.x = game_state.possible_moves_stack.cells[i].possible.x;
            evalTemp.move.y = game_state.possible_moves_stack.cells[i].possible.y;

            // FIXED: Look for minimum weight (not maximum)
            if (evalTemp.weight < mineval.weight) {
                mineval.weight = evalTemp.weight;
                mineval.move.x = evalTemp.move.x;
                mineval.move.y = evalTemp.move.y;
            }
        }
        return mineval;
    }
}





/* 
CellMinimax minimax(GameState game_state ,int depth ,int maximizingPlayer) {
    if (!depth || isGameOver(game_state))
    return  (CellMinimax) {.weight =  evaluatePosition(game_state) , .move.x = game_state.last_played_move.x ,.move.y = game_state.last_played_move.y  }; 




    if (maximizingPlayer)
    {
        CellMinimax maxeval = {.weight =  -500 ,.move.x = -1 , .move.y = -1 } ; 
        for (size_t i = 0; i < game_state.possible_moves_stack.length; i++)
        {
            applyMoveToBoard(game_state , game_state.possible_moves_stack.cells[i].possible) ; 
            CellMinimax evalTemp = minimax(game_state , depth-1 , false) ; 
            // evalTemp.move.x = game_state.possible_moves_stack.cells[i].possible.x ;
            // evalTemp.move.y = game_state.possible_moves_stack.cells[i].possible.y ;

            if (evalTemp.weight > maxeval.weight)
            {
                maxeval.move.x = evalTemp.move.x ; 
                maxeval.move.y = evalTemp.move.y ;
                maxeval.weight = evalTemp.weight ;  
            }
            
        }
        return maxeval ; 
        
    } else {
        CellMinimax mineval = {.weight =  500 ,.move.x = -1 , .move.y = -1 } ; 
        for (size_t i = 0; i < game_state.possible_moves_stack.length; i++)
        {
            applyMoveToBoard(game_state , game_state.possible_moves_stack.cells[i].possible) ; 
            CellMinimax evalTemp = minimax(game_state , depth-1 , true) ; 
            // evalTemp.move.x = game_state.possible_moves_stack.cells[i].possible.x ;
            // evalTemp.move.y = game_state.possible_moves_stack.cells[i].possible.y ;

            if (evalTemp.weight > mineval.weight)
            {
                mineval.move.x = evalTemp.move.x ; 
                mineval.move.y = evalTemp.move.y ;
                mineval.weight = evalTemp.weight ;  
            }
            
        }
        return mineval ; 

    }
    
}
 */