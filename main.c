#include "raylib.h"
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 500

#define CELL_WIDTH 45
#define CELL_HEIGHT 45
#define CIRCLE_RADIUS 15
#define ROWS 8
#define COLS 8
#define PX 120
#define PY 80


typedef struct Player 
{
    char id[20] ; 
    char color ;
    struct Player *next ; 
}Player;



typedef struct 
{
    char mode ; 
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

typedef struct 
{
    PossibleCells cells[MAX_POSSIBLE_CELLS] ;
    int length ; 
}PossibleCellsStack;


PossibleCellsStack possible_cells_stack = {.length = 0} ; 

#define MAX(X,Y) (X>Y) ? X : Y


/*
    1  , 2 , 3
    1  , 2 , 3
    1  , 2 , 3
*/
void addNewPossibleCell(Cell origin , Cell poss) {
    possible_cells_stack.cells[possible_cells_stack.length].original.x = origin.x ; 
    possible_cells_stack.cells[possible_cells_stack.length].original.y = origin.y ;

    possible_cells_stack.cells[possible_cells_stack.length].possible.x = poss.x ; 
    possible_cells_stack.cells[possible_cells_stack.length].possible.y = poss.y ;

    possible_cells_stack.cells[possible_cells_stack.length].count= MAX(poss.x-origin.x ,poss.y-origin.y ) -1 ;

    possible_cells_stack.length++ ;

}


int grid[ROWS][COLS] = {0} ; 
Cell last_played_cell = {0} ; 





void renderGrid( int px , int py  , Player player) {


    for (size_t i = 0; i < ROWS; i++)
    {
        for (size_t j = 0; j < ROWS; j++)
        {
            int x = i * CELL_WIDTH + px ; 
            int y =  j * CELL_HEIGHT + py ; 
            DrawRectangleLines(x, y  , CELL_WIDTH , CELL_HEIGHT , CLITERAL(Color) {0xff , 0xff , 0xff , 0xff}) ; 
            int x2 = (i+1) * CELL_WIDTH + px ; 
            int y2 = (j+1) * CELL_HEIGHT + py ; 
            if (grid[i][j]==0)
                DrawCircle((x + x2) /2 , (y + y2) /2 , CIRCLE_RADIUS , BLACK) ; 
            else if(grid[i][j]==1)
            DrawCircle((x + x2) /2 , (y + y2) /2 , CIRCLE_RADIUS , WHITE) ; 
            else if(grid[i][j]==2)
            {
                DrawCircleLines((x + x2) /2 , (y + y2) /2 , CIRCLE_RADIUS , BLACK ) ;
                // printf("hey\n") ; 
            }
                
             
        }
        
    }
    
}


void initGrid() {

    for (size_t i = 0; i < ROWS; i++)
        for (size_t j = 0; j < COLS; j++)
            grid[i][j]=-1 ; 

    
}

Vector3 placesRemaining() {

    int count = 0 ;
    int count_white = 0 ;
    int count_black = 0 ;

    for (size_t i = 0; i < ROWS; i++)
        for (size_t j = 0; j < COLS; j++)
            if (grid[i][j]<0)
            count++ ; 
            else if (!grid[i][j])
            count_black++ ; 
            else if (grid[i][j]==1)
            count_white++ ; 

    Vector3 v = {.x=  count, .y = count_black  , .z = count_white } ; 
    return v ; 
}


void printPossibleChances(Player player) {
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
                    int u = 0 ;          
                    
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

                     while ( look_y + y < 8 && look_y + y > -1 && look_x + x < 8 && look_x + x > -1)
                    {
                        if (grid[look_x+x][look_y+y]==-1 )
                            if (u > 0) {
                                grid[look_x+x][look_y+y]=2 ; 
                                break;

                            }
                            else 
                                break;    
                        else if ((grid[look_x+x][look_y+y]==player.color || grid[look_x+x][look_y+y]== 2 )) // 
                        break;

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



int main() {
    // Initialize the window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "orth ella");
    
    
    initGrid(grid) ;

    grid[4][3] = 0 ; 
    grid[3][4] = 0 ; 
    grid[3][3] = 1 ; 
    grid[4][4] = 1 ; 
    // grid[5][5] = 2 ; 
    // grid[5][2] = 1 ; 


    Player player1 = {.id = '1' , .color = 0 } ; 
    Player player2 = {.id = '2' , .color = 1 } ; 

    player1.next = &player2 ; 
    player2.next = &player1 ; 

    Game game  ;
    game.mode = 1 ;
    game.player1 = &player1 ; 
    game.player2 = &player2 ; 
    game.current = &player1 ; // player1 


    SetTargetFPS(60); // Set FPS for consistent timing

    while (!WindowShouldClose()) { // Main game loop
        
        float i , j ; 
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse =  GetMousePosition() ;
            
            if (mouse.x >= PX && mouse.x <= PX + ROWS * CELL_WIDTH)
            {
                 i = ((roundf(mouse.x) - PX) / CELL_WIDTH) ; 
            }
            if (mouse.y >= PY && mouse.y <= PY + COLS * CELL_HEIGHT)
            {
                j = ((roundf(mouse.y) - PY) / CELL_HEIGHT) ;
            }
            if (grid[(int) i][(int) j]==2)
            {
                grid[(int) i][(int) j]= game.current->color  ; 
                flipCells(*game.current) ; 
                game.current = game.current->next ; 
                clearPossibleChances() ; 
                /* code */
                last_played_cell.x= (int) i ; 
                last_played_cell.y= (int) j ; 

                printf("last-played-cell = {.x = %d ,.y = %d} \n " ,last_played_cell.x , last_played_cell.y) ;
                printf("row = %d column = %d \n " ,(int) i ,(int) j) ;
                printf("cells remaining = %.1f \n " , placesRemaining().x ) ;
            }
            



        }
            
        char cellsRemainingText[40] ; 
        sprintf(cellsRemainingText , "black cells : %.0f white cells : %.0f " , placesRemaining().y , placesRemaining().z) ;

        // Draw the scene
        BeginDrawing();
        ClearBackground(CLITERAL(Color) {0x0 , 0x55 , 0x0 , 0xff}) ; 

        printPossibleChances(*game.current) ; 
        renderGrid(PX , PY , *game.current) ; 
        
        DrawText(cellsRemainingText , SCREEN_WIDTH /2 - 200, SCREEN_HEIGHT - 30 , 24 , BLACK ) ; 

        // DrawRectangle(10 , 10 , 20 , 20 , CLITERAL(Color) {0xff , 0xff , 0xff , 0xff}) ; 
        // DrawRectangleLines(10 , 10 , 20 , 20 , CLITERAL(Color) {0xff , 0xff , 0xff , 0xff}) ; 

        EndDrawing();
    }

    CloseWindow(); // Close the window and free resources
    return 0;
}
