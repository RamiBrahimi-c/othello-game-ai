#include "Game.h"



int grid[ROWS][COLS] = {0} ; 
Cell last_played_cell = {-1 , -1} ; 
int turn = 0 ; 
Game game  ;
PossibleCellsStack possible_cells_stack = {.length = 0} ; 


void addNewPossibleCell(Cell origin , Cell poss) {
    possible_cells_stack.cells[possible_cells_stack.length].original.x = origin.x ; 
    possible_cells_stack.cells[possible_cells_stack.length].original.y = origin.y ;

    possible_cells_stack.cells[possible_cells_stack.length].possible.x = poss.x ; 
    possible_cells_stack.cells[possible_cells_stack.length].possible.y = poss.y ;

    possible_cells_stack.cells[possible_cells_stack.length].count= MAX(poss.x-origin.x ,poss.y-origin.y ) -1 ;

    possible_cells_stack.length++ ;

}

void initGrid() {

    for (size_t i = 0; i < ROWS; i++)
        for (size_t j = 0; j < COLS; j++)
            grid[i][j]=-1 ; 
    grid[4][3] = 0 ; 
    grid[3][4] = 0 ; 
    grid[3][3] = 1 ; 
    grid[4][4] = 1 ; 

    
}

CellsColor getCellsRemainingCount() {

    int count = 0 ;
    int count_white = 0 ;
    int count_black = 0 ;

    CellsColor total = {0} ; 

    for (size_t i = 0; i < ROWS; i++)
        for (size_t j = 0; j < COLS; j++)
            if (grid[i][j]<0)
            total.empty++ ; 
            else if (!grid[i][j])
            total.black++ ; 
            else if (grid[i][j]==1)
            total.white++ ;
            else 
            total.possible++ ;

    return total ; 
}


void setPossibleChances(Player player) {
    Directions directions_possible[8]  ; 
    
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            if (grid[i][j] < 0)
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

            if (grid[i][j]==player.color)
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
                        if (grid[look_x+x][look_y+y]==-1 )
                            if (u > 0) {
                                addNewPossibleCell((Cell) {i , j} ,(Cell) {look_x+x , look_y+y}) ; 
                                grid[look_x+x][look_y+y]=2 ; 
                                break;

                            }
                            else 
                                break;    
                        else if ((grid[look_x+x][look_y+y]==player.color  )) // || grid[look_x+x][look_y+y]== 2 
                        break;
                        else if ((grid[look_x+x][look_y+y]==2  )) 
                        {
                            addNewPossibleCell((Cell) {i , j} ,(Cell) {look_x+x , look_y+y});
                            /* for (size_t q = 0; q < possible_cells_stack.length; q++)
                            {
                                if (possible_cells_stack.cells[q].possible.x == look_x+x && possible_cells_stack.cells[q].possible.y == look_y+y 
                                && possible_cells_stack.cells[q].original.x != i && possible_cells_stack.cells[q].original.y != j)
                                {
                                        addNewPossibleCell((Cell) {i , j} ,(Cell) {look_x+x , look_y+y});
                                    break;
                                    
                                }
                                
                            } */
                            break;
                            
                        }

                        // else if (k < 2 && grid[i][j+y]==((player+1)%2)+1)
                        // y += (y>0) ? 1 : -1 ;     
                        // x += (y>0) ? 1 : -1 ;     
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


void clearPossibleChances() {
    for (size_t i = 0; i < ROWS; i++)
        for (size_t j = 0; j < COLS; j++)
            if (grid[i][j]==2)
                grid[i][j] = -1 ; 
    
}

void flipCells(Player player) {
    Directions directions_possible[8]  ; 
    
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            if (grid[i][j] < 0)
                continue;
            

            for (size_t k = 0; k < 8; k++)
            {
                directions_possible[k].possibility = 1 ; 
                directions_possible[k].direct = k ; 
            }

            if ( i == 0)
                directions_possible[UP].possibility = 0 ; 
            if ( i == 7)
                directions_possible[DOWN].possibility = 0 ; 
            if ( j == 0)
                directions_possible[LEFT].possibility = 0 ; 
            if ( j == 7)
                directions_possible[RIGHT].possibility = 0 ; 

            if (grid[i][j]==player.color ) // && (i == last_played_cell.x) && (j == last_played_cell.y)
            {
                int k = 0 ; 
                while (k < 8)
                {
                    if (!directions_possible[k].possibility)
                    {
                        k++ ; 
                         continue;
                    }   
                    int look_x = i ; 
                    int look_y = j ;
                    int look = (k < 2) ? i : j  ;
                    int x = 0 ; 
                    int y = 0 ; 
                    int u = 0 ; 

                    switch (directions_possible[k].direct )
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

                    int cells = 0 ; 

                    while ( look_y + y < 8 && look_y + y > -1 && look_x + x < 8 && look_x + x > -1)
                    {
                        if ( grid[look_x+x][look_y+y]==-1 )
                                break;    
                        else if ( grid[look_x+x][look_y+y]==((player.color+1)%2) ) // 
                        {
                            cells++ ; 
                        }
                        else if ( ( grid[look_x+x][look_y+y]== player.color  )) // && (look_x+x == last_played_cell.x) && (look_y+y == last_played_cell.y) 
                        {
                            if (!cells) 
                            break;

                            int a = 0 ;
                            int key_x = x ;  
                            int key_y = y ;  
                            int temp_x = look_x + x ;  
                            int temp_y = look_y + y ;  
                            
                            
                            key_x *=  -1 ;     
                            key_y *=  -1 ;     
                            // temp_x += key_x ; 
                            // temp_y += key_y ; 
                            // key_x -= (key_x>0) ? 1 : -1 ;     
                            // key_y -= (key_y>0) ? 1 : -1 ;     
                            printf("original cell : grid[%d][%d] = %d \n" , look_x+x , look_y+y  ,grid[look_x+x][look_y+y] ) ; 
                            printf("cells =  %d \n" ,cells ) ; 
                            
                            while (a < cells)
                            {
                                printf("grid[%d][%d]=%d   player.color = %d \n" ,temp_x+key_x , temp_y+key_y  , grid[temp_x+key_x][temp_y+key_y] , player.color) ; 
                                grid[temp_x+key_x][temp_y+key_y] = (player.color) ; 
                                temp_x += key_x ; 
                                temp_y += key_y ; 

                                a++ ; 
                            }
                            break;
                            
                        }
                        // else if (k < 2 && grid[i][j+y]==((player+1)%2)+1)
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


int abs(int a  ) {
    return (a>0) ? a : -a ; 
}

void clearPossibleCellArray() {
    possible_cells_stack.length = 0 ;     
}

void flipCellsOptimized(Player player ) {
    if ( last_played_cell.x < 0 || last_played_cell.y < 0)
        return ; 

    int p = 0 ; 

    for (size_t i = 0; i < possible_cells_stack.length; i++)
    {
        if (last_played_cell.x == possible_cells_stack.cells[i].possible.x &&
            last_played_cell.y == possible_cells_stack.cells[i].possible.y
        )
        {
            int key = i ; 
            int origin_x = possible_cells_stack.cells[key].original.x   ; 
            int origin_y = possible_cells_stack.cells[key].original.y   ; 
            int delta_x = (possible_cells_stack.cells[key].possible.x - possible_cells_stack.cells[key].original.x)  ; 
            int delta_y = (possible_cells_stack.cells[key].possible.y - possible_cells_stack.cells[key].original.y)  ; 
            int c = 1 ; 
            // printf("delta_x=%d delta_y=%d\n" , delta_x , delta_y) ; 
            while (c < abs(delta_x) && c < abs(delta_y))
            {
                if (delta_x >= 0 && delta_y >= 0)
                grid[origin_x + c][origin_y+c] = player.color ;
                else if (delta_x >= 0)
                grid[origin_x + c][origin_y-c] = player.color ;
                else if (delta_y >= 0)
                grid[origin_x - c][origin_y+c] = player.color ;
                else 
                grid[origin_x - c][origin_y-c] = player.color ;

                c++ ; 
            }
            while (c < abs(delta_x) )
            {
                if (delta_x>=0)
                grid[origin_x + c][origin_y] = player.color ;
                else
                grid[origin_x - c][origin_y] = player.color ;

                c++ ; 
            }
            
            while (c < abs(delta_y) )
            {
                if (delta_y>=0)
                grid[origin_x ][origin_y+c] = player.color ;
                else
                grid[origin_x ][origin_y-c] = player.color ;

                c++ ; 
            }
            
            p++ ;
        }
        
    }
    

}



