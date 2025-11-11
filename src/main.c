#include "raylib.h"
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "../include/Game.h"
// #include "Game.h"
// #include "GameAi.h"
#include "../include/GameAi.h"
#include <pthread.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 500
#define BUTTONN_MENU_LENGTH 4

#define DEPTH 8

int Depth ;
// ****************************************************************************
// ******** functions *********************************************************
// ****************************************************************************


void renderGrid( int px , int py  , Player player) ;
void printPossibleCellsArray() ; 
void renderMetaData()  ; 




void handleMainMenuMouse(Rectangle buttonRect[BUTTONN_MENU_LENGTH] ,Color buttonColor[BUTTONN_MENU_LENGTH] , int length) {
    if (game.state)
        return;

    int i = 0 ; 
    while (i < length)
    {
        Vector2 mousePoint = GetMousePosition();
        bool mouseOverButton = CheckCollisionPointRec(mousePoint, buttonRect[i]);

        if (mouseOverButton)
        {
            buttonColor[i] = DARKGREEN; // Change color on hover
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // Button click action here
                TraceLog(LOG_INFO, "Button %d" , i);

                 if (!i)
                {
                    game.state = 1 ; 
                } else if (i==1) {
                    strcpy(game.player2->id , "Ai v0.1") ; 
                    game.player2->type = AI ; 
                    game.state = 2 ; 

                } else if (i==2){
                    // game.state = 3 ; 
                    game.state = -1 ; 
                }
                 
            }
        }
        else
        {
            buttonColor[i] = GREEN; // Restore original color
        }
        i++ ; 
    }    
}

void renderMainMenu(Rectangle buttonRect[BUTTONN_MENU_LENGTH] ,Color buttonColor[BUTTONN_MENU_LENGTH]  , const char *buttonText[BUTTONN_MENU_LENGTH] , int length) {
    char main_text[40] ; 

    ClearBackground(CLITERAL(Color) {0x40 , 0x40 , 0x40 , 0xff}) ; 

    sprintf( main_text, "Othello") ;
    
    DrawText(main_text , SCREEN_WIDTH/2-70 , SCREEN_HEIGHT / 6 , 40 , BLACK) ; 

    int i = 0 ; 
    while (i < length)
    {
            // Draw the button rectangle
        DrawRectangleRec(buttonRect[i], buttonColor[i]);

        // Draw the button text, centered on the button
        int textWidth = MeasureText(buttonText[i], 20); // Measure text width for centering
        DrawText(buttonText[i], buttonRect[i].x + buttonRect[i].width / 2 - textWidth / 2,
                 buttonRect[i].y + buttonRect[i].height / 2 - 10, 20, WHITE);
            // buttonRect[i].y += 10 ; 
            i++ ; 
    }


} 

void renderMainMenubetter(Rectangle buttonRect[BUTTONN_MENU_LENGTH], Color buttonColor[BUTTONN_MENU_LENGTH], const char *buttonText[BUTTONN_MENU_LENGTH] , int length) {
    // Clear background
    ClearBackground(CLITERAL(Color){0x40, 0x40, 0x40, 0xFF});

    // Draw title - properly centered
    const char* mainText = "Othello";
    int titleWidth = MeasureText(mainText, 40);
    DrawText(mainText, 
             SCREEN_WIDTH / 2 - titleWidth / 2, 
             SCREEN_HEIGHT / 6, 
             40, BLACK);

    // Draw buttons
    for (int i = 0; i < length; i++) {
        // Draw button background
        DrawRectangleRec(buttonRect[i], buttonColor[i]);
        
        // Calculate centered text position
        int textWidth = MeasureText(buttonText[i], 20);
        int textX = buttonRect[i].x + (buttonRect[i].width - textWidth) / 2;
        int textY = buttonRect[i].y + (buttonRect[i].height - 20) / 2;
        
        // Draw button text
        DrawText(buttonText[i], textX, textY, 20, WHITE);
    }
}




void displayWinner(Player *winner) {
    const Color BACKGROUND_COLOR = {0x2D, 0x2D, 0x2D, 0xFF};
    const Color TEXT_COLOR = WHITE;
    const Color HIGHLIGHT_COLOR = {0xFF, 0xD7, 0x00, 0xFF};
    const Color BOX_COLOR = {0x40, 0x40, 0x40, 0xFF};
    const Color BLACK_COLOR = {0x30, 0x30, 0x30, 0xFF};
    const Color WHITE_COLOR = {0xE0, 0xE0, 0xE0, 0xFF};
    const Color TIE_COLOR = {0x88, 0xC0, 0xFF, 0xFF}; // Light blue for ties
    
    // Get game statistics
    CellsColor stats = getCellsRemainingCount();
    
    // Determine if it's a draw (both players have equal pieces)
    bool isDraw = (stats.black == stats.white);
    
    // Clear background
    // ClearBackground(BACKGROUND_COLOR);
    
    // Draw main winner announcement box
    Rectangle winnerBox = {
        SCREEN_WIDTH / 2 - 250,
        SCREEN_HEIGHT / 2 - 150,
        500,
        300
    };
    
    // Draw box with border
    DrawRectangleRec(winnerBox, BOX_COLOR);
    DrawRectangleLinesEx(winnerBox, 3, isDraw ? TIE_COLOR : HIGHLIGHT_COLOR);
    
    // Draw title
    const char* titleText = "Game Over!";
    int titleWidth = MeasureText(titleText, 36);
    DrawText(titleText, 
             SCREEN_WIDTH / 2 - titleWidth / 2,
             winnerBox.y + 25,
             36, isDraw ? TIE_COLOR : HIGHLIGHT_COLOR);
    
    if (winner != NULL && !isDraw) {
        // Draw winner name (only if not a draw)
        char winnerText[50];
        sprintf(winnerText, "Winner: %s", winner->id);
        
        int winnerWidth = MeasureText(winnerText, 28);
        DrawText(winnerText,
                 SCREEN_WIDTH / 2 - winnerWidth / 2,
                 winnerBox.y + 80,
                 28, TEXT_COLOR);
        
        // Draw player color indicator
        char colorText[20];
        Color playerColor = (winner->color == 'B') ? BLACK_COLOR : WHITE_COLOR;
        sprintf(colorText, "Color: %s", !winner->color  ? "Black" : "White");
        
        int colorWidth = MeasureText(colorText, 24);
        DrawText(colorText,
                 SCREEN_WIDTH / 2 - colorWidth / 2,
                 winnerBox.y + 120,
                 24, playerColor);
        
    } else {
        // Draw tie game message (when scores are equal OR winner is NULL)
        const char* tieText = "It's a Tie!";
        int tieWidth = MeasureText(tieText, 32);
        DrawText(tieText,
                 SCREEN_WIDTH / 2 - tieWidth / 2,
                 winnerBox.y + 90,
                 32, TIE_COLOR);
        
        // Additional tie explanation
        if (stats.black == stats.white) {
            char tieReason[40];
            sprintf(tieReason, "Both players have %d pieces", stats.black);
            DrawText(tieReason,
                     SCREEN_WIDTH / 2 - MeasureText(tieReason, 18) / 2,
                     winnerBox.y + 130,
                     18, LIGHTGRAY);
        }
    }
    
    // Draw statistics section
    const char* statsText = "Final Score:";
    DrawText(statsText,
             SCREEN_WIDTH / 2 - MeasureText(statsText, 22) / 2,
             winnerBox.y + 160,
             22, isDraw ? TIE_COLOR : HIGHLIGHT_COLOR);
    
    // Draw black pieces count
    char blackText[20];
    sprintf(blackText, "Black: %d", stats.black);
    DrawText(blackText,
             SCREEN_WIDTH / 2 - 100,
             winnerBox.y + 190,
             20, LIGHTGRAY);
    
    // Draw a small black circle next to the count
    DrawCircle(SCREEN_WIDTH / 2 - 130, winnerBox.y + 200, 8, BLACK);
    
    // Draw white pieces count
    char whiteText[20];
    sprintf(whiteText, "White: %d", stats.white);
    DrawText(whiteText,
             SCREEN_WIDTH / 2 + 40,
             winnerBox.y + 190,
             20, LIGHTGRAY);
    
    // Draw a small white circle next to the count
    DrawCircle(SCREEN_WIDTH / 2 + 15, winnerBox.y + 200, 8, WHITE);
    
    // Highlight the winning color or show equal sign for draws
    // if (isDraw) {
    //     // Draw equal sign between scores
    //     DrawText("=",
    //              SCREEN_WIDTH / 2 - 10,
    //              winnerBox.y + 190,
    //              20, TIE_COLOR);
    // } else if (winner != NULL) {
    //     // Highlight the winning color with a star
    //     const char* winIndicator = "★";
    //     if (!winner->color) {
    //         DrawText(winIndicator,
    //                  SCREEN_WIDTH / 2 - 140,
    //                  winnerBox.y + 185,
    //                  20, HIGHLIGHT_COLOR);
    //     } else {
    //         DrawText(winIndicator,
    //                  SCREEN_WIDTH / 2 + 70,
    //                  winnerBox.y + 185,
    //                  20, HIGHLIGHT_COLOR);
    //     }
    // }
    
    // Draw empty cells count
    // char emptyText[30];
    // sprintf(emptyText, "Empty Cells: %d", stats.empty);
    // DrawText(emptyText,
    //          SCREEN_WIDTH / 2 - MeasureText(emptyText, 16) / 2,
    //          winnerBox.y + 220,
    //          16, GRAY);
    
    // Draw continue prompt
    const char* continueText = "Press any key to continue...";
    int continueWidth = MeasureText(continueText, 18);
    DrawText(continueText,
             SCREEN_WIDTH / 2 - continueWidth / 2,
             SCREEN_HEIGHT - 50,
             18, TEXT_COLOR);
}



int getPossibleMoveValue(Cell poss) {
    
    int k = 0 ; 
    for (size_t i = 0; i < possible_cells_stack.length; i++)
    {
        if (possible_cells_stack.cells[i].possible.x == poss.x && 
            possible_cells_stack.cells[i].possible.y == poss.y){
                k += possible_cells_stack.cells[i].count ; 
        }
    }
    return k ; 
}


Cell getBestMoveAt1Position() {
    
    Cell max = { possible_cells_stack.cells[0].possible.x , 
            possible_cells_stack.cells[0].possible.y
            } ; 
    for (size_t i = 1; i < possible_cells_stack.length; i++)
    {
        if (getPossibleMoveValue(max) < getPossibleMoveValue(possible_cells_stack.cells[i].possible))
        {
            max.x = possible_cells_stack.cells[i].possible.x;
            max.y = possible_cells_stack.cells[i].possible.y;
        }
            
    }    
    return max ; 
}



void renderBox(Rectangle rect , int value) {
    DrawRectangle(rect.x , rect.y , rect.width , rect.height , RED) ;
    char buffer[64] ; 
    sprintf(buffer , "%d" , value) ; 
    DrawText(buffer , rect.x + rect.width/4 ,rect.y + rect.height/4 , 13 , BLACK ) ;  
}



void renderTree_n_minimax( Node_n *root) {
    if (root == NULL) return;

    // queue + parallel arrays for storing parent positions for each queued node
    Node_n *queue[1000];
    int parentX[1000], parentY[1000];
    int front = 0, rear = 0;

    // enqueue root; parent position for root is itself (unused for drawing)
    queue[rear] = root;
    parentX[rear] = SCREEN_WIDTH / 2;
    parentY[rear] = 50;
    rear++;

    int level = 0;
    while (front < rear) {
        int levelCount = rear - front;           // number of nodes on this level
        int spacing = SCREEN_WIDTH / (levelCount + 1);

        // For each node on this level compute its x,y and draw it.
        for (int i = 0; i < levelCount; ++i) {
            Node_n *cur = queue[front + i];
            int x = spacing * (i + 1);
            int y = 100 + level * 120;

            // Draw line from stored parent position to this node (skip for root)
            if (level > 0) {
                // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white lines
                // SDL_RenderDrawLine(renderer, parentX[front + i], parentY[front + i], x, y);
                DrawLine(parentX[front + i], parentY[front + i], x , y , WHITE) ; 
            }

            // Draw the node circle
            // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green nodes
            DrawCircle( x, y, 25 , GREEN);
            char number[10] ; 
            sprintf(number , "%d" ,evaluatePositionEnhanced(copyGameState(cur->data) , cur->maximizing_player) ) ;
            // printf("game state(cooked) : %s   %s  \n" , number , (!cur->data.turn->type ? "human" : "ai") ) ;  
            DrawText(number , x-10 ,y-10 , 20 , RED) ; 

            // Enqueue children and store this node's position as their parent position.
            for (int c = 0; c < cur->n; ++c) {
                if (cur->child[c] == NULL) continue; // be safe
                queue[rear] = cur->child[c];
                // queue[rear]->n = cur->child[c]->n ; 
                parentX[rear] = x;         // when that child is processed, we'll draw line from these coords
                parentY[rear] = y;
                rear++;
                if (rear >= 1000) {        // guard against overflow
                    // optional: handle overflow properly in your real program
                    rear = 999;
                }
            }
        }

        // advance to next level
        front += levelCount;
        level++;
    }
}








// ************************************************************************************************
// ************************************************************************************************
// ************************************************************************************************
// ************************************************************************************************
// ************************************************************************************************
// ************************************************************************************************
// ************************************************************************************************
// ************************************************************************************************





int main() {





    // depth
    Depth = 2 ; 


    // Initialize the window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "orthello");
    // SetWindowState(FLAG_WINDOW_RESIZABLE);                    // Set window configuration state using flags
    
    
    // set every cell initially to -1
    initGrid(grid) ;


    Player player1 = {.id = "player 1" , .color = 0 , .type= HUMAN } ;  // black player
    Player player2 = {.id = "player 2" , .color = 1 , .type= AI} ;  // white player 

    player1.next = &player2 ; 
    player2.next = &player1 ; 



    game.mode = 1 ;
    game.player1 = &player1 ; 
    game.player2 = &player2 ; 
    game.current = &player1 ; // player1 
    game.ongoing = 1 ; 
    game.state = 0 ; 
    game.winner = NULL ; 
    
    setPossibleChances(*game.current) ; 

    Rectangle buttonRect[BUTTONN_MENU_LENGTH] = { {(float)SCREEN_WIDTH / 2 - 80, (float)SCREEN_HEIGHT / 2 - 20, 170, 40} , 
    {(float)SCREEN_WIDTH / 2 - 80, (float)SCREEN_HEIGHT / 2 - 20 + 50, 170, 40} , 
    {(float)SCREEN_WIDTH / 2 - 80, (float)SCREEN_HEIGHT / 2 - 20 + 100, 170, 40}

    };
    Color buttonColor[BUTTONN_MENU_LENGTH] = {GREEN , GREEN , GREEN};
    const char *buttonText[BUTTONN_MENU_LENGTH] = {"Play vs Human" , 
        "Play vs AI"  ,

        "Exit"
    };
    int buttons_length = 3 ; 

    int magic = 0 ; 
    int played = 0 ; 


    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0.0f, 0.0f };
    camera.offset = (Vector2){ 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

        int visual_minimax = 0 ; 

            Node_n *root_test = malloc(sizeof(Node_n)) ; 


    SetTargetFPS(60); // Set FPS for consistent timing

    while (!WindowShouldClose()) { // Main game loop
        
        int i , j ; 
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

            if (grid[(int) i][(int) j]==2 && game.state==1)
            {
                grid[(int) i][(int) j]= game.current->color  ; 
                last_played_cell.x= (int) i ; 
                last_played_cell.y= (int) j ; 

                flipCellsOptimized(*game.current ) ; //  
                game.current = game.current->next ; 

                clearPossibleChances() ; 
                clearPossibleCellArray() ;

                setPossibleChances(*game.current);
                played = 1 ; 

            }
            if (grid[(int) i][(int) j]==2 && (game.state==2 || game.state==3) && game.current->type==HUMAN)
            {
                grid[(int) i][(int) j]= game.current->color  ; 
                last_played_cell.x= (int) i ; 
                last_played_cell.y= (int) j ; 

                flipCellsOptimized(*game.current ) ; //  
                game.current = game.current->next ; 

                clearPossibleChances() ; 
                clearPossibleCellArray() ;

                setPossibleChances(*game.current);
                played = 1 ; 

            }             



        }

        if (IsKeyPressed(KEY_S))
        {
            game.current = game.current->next ; 

                clearPossibleChances() ; 
                clearPossibleCellArray() ;

                setPossibleChances(*game.current);

                turn = (turn+1)%2 ; 
             
        }

        if(IsKeyPressed(KEY_C)) {
            cleanQueue() ; 
        }


        if(IsKeyDown(KEY_BACKSPACE)) {
            game.state = 0 ; 
        }

        if(game.state==-1) {
            break;
        }
        handleMainMenuMouse(buttonRect , buttonColor  , BUTTONN_MENU_LENGTH) ; 
            

        
        // Draw the scene
        BeginDrawing();
        ClearBackground(CLITERAL(Color) {0x0 , 0x55 , 0x0 , 0xff}) ; 
        
        char fps[20]  ; 
        sprintf(fps , " FPS : %d" , GetFPS()) ; 
        DrawText(fps , 10 , 10 , 18 , YELLOW) ; 


        switch (game.state)
        {
        case 0:

            renderMainMenu(buttonRect , buttonColor , buttonText , BUTTONN_MENU_LENGTH); 
            break;
        
        case 1:
            int gameover = isGameOver(getGameState()) ; 
            if (gameover)
            {
                game.ongoing = 0 ; 
                CellsColor total = getCellsRemainingCount() ; 
                game.winner = (total.black > total.white) ? &player1 : &player2 ;                 


            } else {
                if (getCellsRemainingCount().possible < 1 )
                {
                    TraceLog(LOG_INFO , "%s skipped his turn " , game.current->id) ; 
                    game.current = game.current->next ; 

                    clearPossibleChances() ; 
                    clearPossibleCellArray() ;

                    setPossibleChances(*game.current);
                    played = 1 ; 

                    // turn = (turn+1)%2 ; 
                    
                }
            }



            renderMetaData() ; 
            
            // BeginMode2D(camera) ; 
            
            renderGrid(PX , PY , *game.current) ; 
            // EndMode2D() ; 
            
            if(gameover)
            displayWinner(game.winner) ; 


            break;

        case 2 :
        /*
            vs AI    
        */
            gameover = isGameOver(getGameState()) ; 
            if (gameover)
            {
                game.ongoing = 0 ; 
                CellsColor total = getCellsRemainingCount() ; 
                game.winner = (total.black > total.white) ? &player1 : &player2 ;                 


            } else {
                if (getCellsRemainingCount().possible < 1 )
                {
                    TraceLog(LOG_INFO , "%s skipped his turn " , game.current->id) ; 
                    game.current = game.current->next ; 

                    clearPossibleChances() ; 
                    clearPossibleCellArray() ;

                    setPossibleChances(*game.current);
                    played = 1 ; 

                    break;
                    
                }

                if (game.current->type == AI ) {
                    static double aiMoveTime = 0;
                    static bool aiThinking = false;
                    static CellMinimax aiMove;
                    double before = 0 ;     
                    double after = 0 ;     
                    
                    pthread_t thread1 ;
                    CellMinimax *temp ;
                    
                    if (!aiThinking) {
                        // Start AI thinking
                        aiThinking = true;
                        aiMoveTime = GetTime();
                        before = GetTime() ; 





                        CellMinimax temp = minimax(getGameState(), Depth, true);
                        printf("\n");
                        // CellMinimax temp = minimaxalpha(getGameState(), Depth ,(int)-INFINITY ,(int) INFINITY , true);
                        after = GetTime() ;

                        aiMove.move.x = temp.move.x; 
                        aiMove.move.y = temp.move.y;
                        aiMove.weight = temp.weight;
                        
                        TraceLog(LOG_INFO, "AI calculated move: (%d, %d) with weight: %d", 
                                aiMove.move.x, aiMove.move.y, aiMove.weight);
                    }
                    
                    // Show "AI is thinking..." message while waiting
                    if (GetTime() - aiMoveTime <= 3.0f) {
                        // Calculate progress for visual feedback (0.0 to 1.0)
                        float progress = (GetTime() - aiMoveTime) / 3.0f;
                        
                        // Draw thinking message with pulsing effect
                        const char* thinkingText = "AI is thinking";
                        int dotCount = ((int)(GetTime() * 2) % 4); // Pulsing dots
                        char dynamicText[20];
                        snprintf(dynamicText, sizeof(dynamicText), "AI is thinking%.*s", dotCount, "...");
                        
                        // Draw semi-transparent background
                        DrawRectangle(10, 10, 200, 60, Fade(BLACK, 0.7f));
                        
                        // Draw text
                        DrawText(dynamicText, 20, 20, 20, YELLOW);
                        
                        // Draw progress bar
                        DrawRectangle(20, 50, 180, 15, DARKGRAY);
                        DrawRectangle(20, 50, (int)(180 * progress), 15, GREEN);
                        
                    } else {
                        // Time's up - execute the AI move
                        int i = aiMove.move.x;
                        int j = aiMove.move.y;

                        TraceLog(LOG_WARNING, "AI plays move {.x = %d, .y = %d} took %fs to think", i, j , after - before); 
                        
                        if(i > -1 && i < ROWS && j > -1 && j < COLS) {
                            grid[i][j] = game.current->color; 
                            last_played_cell.x = i; 
                            last_played_cell.y = j; 
                            flipCellsOptimized(*game.current);
                            game.current = game.current->next; 
                            clearPossibleChances();
                            clearPossibleCellArray();
                            setPossibleChances(*game.current);
                                played = 1 ;

                        } else {
                            TraceLog(LOG_ERROR, "AI wants to play invalid move (%d, %d)", i, j);
                        }
                        
                        aiThinking = false;
                    }
                }

            }
            



            char temp_string_depth[20]  ;
            sprintf( temp_string_depth, "depth : %d" , Depth) ;  
            DrawText(temp_string_depth , SCREEN_WIDTH/4+SCREEN_WIDTH/2 , 15 , 22 , DARKBLUE) ; 

            renderMetaData() ; 


            
            renderGrid(PX , PY , *game.current) ; 
                        
            
            if(gameover)
            displayWinner(game.winner) ; 

            



                
                break;
            default:
                break;
        } 

        if (played)
        {
            turn = (turn+1)%2 ; 
            played = 0 ; 
        }
        

        EndDrawing();
    }

    exit_game:
    CloseWindow(); // Close the window and free resources
    return 0;
}



void renderTree_n( Node_n *root) {
    if (root == NULL) return;

    // queue + parallel arrays for storing parent positions for each queued node
    Node_n *queue[1000];
    int parentX[1000], parentY[1000];

    int front = 0, rear = 0;

    // enqueue root; parent position for root is itself (unused for drawing)
    queue[rear] = root;
    parentX[rear] = SCREEN_WIDTH / 2;
    parentY[rear] = 50;
    rear++;

    int level = 0;
    while (front < rear) {
        int levelCount = rear - front;           // number of nodes on this level
        int spacing = SCREEN_WIDTH / (levelCount + 1);

        // For each node on this level compute its x,y and draw it.
        for (int i = 0; i < levelCount; ++i) {
            Node_n *cur = queue[front + i];
            int x = spacing * (i + 1);
            int y = 100 + level * 120;

            // Draw line from stored parent position to this node (skip for root)
            if (level > 0) {
                // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white lines
                // SDL_RenderDrawLine(renderer, parentX[front + i], parentY[front + i], x, y);
                DrawLine(parentX[front + i], parentY[front + i], x , y , WHITE) ; 
            }

            // Draw the node circle
            // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green nodes
            DrawCircle( x, y, 25 , GREEN);
            char number[10] ; 
            sprintf(number , "%d" ,evaluatePositionEnhanced(cur->data , cur->maximizing_player) ) ;
            // printf("%s     %d\n" , number , cur->data) ;  
            DrawText(number , x-10 ,y-10 , 30 , RED) ; 

            // Enqueue children and store this node's position as their parent position.
            for (int c = 0; c < cur->n; ++c) {
                if (cur->child[c] == NULL) continue; // be safe
                queue[rear] = cur->child[c];
                parentX[rear] = x;         // when that child is processed, we'll draw line from these coords
                parentY[rear] = y;
                rear++;
                if (rear >= 1000) {        // guard against overflow
                    // optional: handle overflow properly in your real program
                    rear = 999;
                }
            }
        }

        // advance to next level
        front += levelCount;
        level++;
    }
}






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
                Color c1 = {0x0 , 0x0 , 0x0 , 0x44} ;
                Color c2 = {0xff , 0xff , 0xff , 0x44} ;
                DrawCircle((x + x2) /2 , (y + y2) /2 , CIRCLE_RADIUS , (!player.color)?c1 : c2 ) ;
                // DrawCircle((x + x2) /2 , (y + y2) /2 , CIRCLE_RADIUS , CLITERAL(Color) {0xff , 0xff , 0xff , 0x22} ) ;
                // DrawCircle((x + x2) /2 , (y + y2) /2 , CIRCLE_RADIUS , CLITERAL(Color) {0xb4 , 0xb4 , 0xb4 , 0x44} ) ;
                // printf("hey\n") ; 
            }
                
             
        }
        
    }
    
}



void printPossibleCellsArray() {
    printf("********************\n");
    printf("length = %d \n" , possible_cells_stack.length) ;
    for (size_t i = 0; i < possible_cells_stack.length; i++)
    {
        printf("origin{%d,%d}\tpossible={%d,%d}\n" , possible_cells_stack.cells[i].original.x ,possible_cells_stack.cells[i].original.y , 
            possible_cells_stack.cells[i].possible.x , possible_cells_stack.cells[i].possible.y 
        ) ;
    }
    
    printf("********************\n");
}


void renderMetaData() {
    char cellsRemainingText[40] ; 
    sprintf(cellsRemainingText , " : %d  " , getCellsRemainingCount().black ) ;

        
    DrawText("Turn : " , SCREEN_WIDTH/2-50, 20 , 20 , BLACK ) ; 
    DrawCircle( SCREEN_WIDTH/2+40,30 , CIRCLE_RADIUS , (!turn)? BLACK : WHITE ) ; 
    // black player
    DrawText(cellsRemainingText , SCREEN_WIDTH /2 - 150, SCREEN_HEIGHT - 30 , 24 , BLACK ) ; 
    DrawCircle( SCREEN_WIDTH /2 - 180, SCREEN_HEIGHT - 20 , CIRCLE_RADIUS , BLACK ) ; 

    // white player
    sprintf(cellsRemainingText , "%d : " , getCellsRemainingCount().white ) ;
    DrawText(cellsRemainingText , (SCREEN_WIDTH /2 +   100) , SCREEN_HEIGHT - 30 , 24 , WHITE ) ; 
    DrawCircle( (SCREEN_WIDTH /2 ) + SCREEN_WIDTH/4 +10, SCREEN_HEIGHT - 20 , CIRCLE_RADIUS , WHITE ) ; 

}
