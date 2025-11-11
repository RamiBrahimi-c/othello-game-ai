# Othello AI

A C implementation of the classic board game **Othello** (also known as Reversi) with an AI opponent powered by the **Minimax algorithm**. The game features a graphical user interface built with **Raylib**.

## Overview

This project implements Othello with the following features:
- **Human vs Human** gameplay
- **Human vs AI** gameplay with configurable difficulty (search depth)
- **AI Decision Making** using the Minimax algorithm with position evaluation
- **Graphical User Interface** using Raylib
- **Optimized Move Calculation** with efficient board state management

## Game Rules

Othello is a two-player strategy game played on an 8×8 board:
- Players alternate placing pieces (black and white)
- When a piece is placed, any opponent pieces in a continuous line between the new piece and another of the player's pieces are flipped
- Players must make a legal move (a move that flips at least one opponent piece)
- If a player cannot move, their turn is skipped
- The game ends when neither player can move
- The player with the most pieces wins

## Project Structure

```
othello-ai/
├── src/
│   ├── main.c          # Main game loop and UI rendering
│   ├── Game.c          # Core game logic and board management
│   └── GameAi.c        # AI implementation using Minimax algorithm
├── include/
│   ├── Game.h          # Game data structures and function declarations
│   ├── GameAi.h        # AI data structures and function declarations
│   └── Queue.h         # Queue data structure for AI
├── bin/                # Compiled executable
├── obj/                # Object files
├── Makefile            # Build configuration
└── README.md           # This file
```

## Data Structures

### Core Game Structures

**Cell**
- Represents a position on the board (x, y coordinates)

**Player**
- Contains player information (ID, color, type [HUMAN/AI], pointer to next player)

**GameState**
- Snapshot of game state for AI evaluation
- Includes board, current turn, possible moves, and last played move

**PossibleCells**
- Represents a legal move with origin cell, target cell, and number of pieces that would be flipped

**CellsColor**
- Counts of pieces by color and board status (empty, black, white, possible)

### AI Structures

**CellMinimax**
- Represents a move with its evaluation weight from the Minimax algorithm

**Node_n**
- Tree node for game tree exploration
- Contains game state, depth, maximizing player flag, and child nodes

## Algorithm Details

### Minimax Algorithm

The AI uses a **Minimax algorithm** to determine the best move:

```
minimax(gameState, depth, isMaximizingPlayer)
    if depth == 0 or game is over:
        return position evaluation score
    
    if isMaximizingPlayer (AI's turn):
        maxEval = -∞
        for each legal move:
            eval = minimax(applyMove(gameState), depth-1, false)
            maxEval = max(maxEval, eval)
        return maxEval
    else (Human's turn):
        minEval = +∞
        for each legal move:
            eval = minimax(applyMove(gameState), depth-1, true)
            minEval = min(minEval, eval)
        return minEval
```

### Position Evaluation

The AI evaluates board positions using a **weighted matrix**:
- Corner positions (100): Highest value - most strategic
- Near edges (-50 to -2): Lower value - risky positions
- Center positions (-1 to 5): Medium value

The evaluation function:
```c
evaluatePositionEnhanced(gameState, maximizingPlayer)
    Applies position matrix weights to all pieces
    AI pieces (white) are positive
    Human pieces (black) are negative
    Returns total weighted score
```

## Building the Project

### Prerequisites

- GCC or compatible C compiler
- Raylib library installed
- Make

### Compilation

```bash
make              # Build the project
make clean        # Remove build artifacts
make run          # Build and run
make debug        # Build with debug symbols
```

The Makefile is configured with:
- **CFLAGS**: `-Wall -Wextra -pedantic -Iinclude` (warnings and include paths)
- **LDFLAGS**: Raylib dependencies and math library

## Usage

1. **Run the Program**:
   ```bash
   ./bin/main
   ```

2. **Menu Options**:
   - **Play vs Human**: Two players on the same machine
   - **Play vs AI**: Play against the AI opponent
   - **Exit**: Quit the game

3. **During Gameplay**:
   - Click on highlighted cells (shown as semi-transparent circles) to make a move
   - The AI will think and automatically make its move (up to 3 seconds)
   - The current score (black and white pieces) is displayed at the bottom

4. **Controls**:
   - `S` - Skip current player's turn
   - `C` - Clear the AI move queue
   - `BACKSPACE` - Return to main menu
   - Mouse click - Place piece on board

## Configuration

### AI Difficulty

Modify the search depth in `main.c`:

```c
Depth = 2;  // Default search depth (number of moves to look ahead)
```

Higher values (up to 8-10) increase AI strength but also increase thinking time.

## Key Functions

### Game Logic (Game.c)
- `initGrid()` - Initialize the board
- `setPossibleChances()` - Calculate legal moves for current player
- `flipCellsOptimized()` - Flip opponent pieces after a move
- `getCellsRemainingCount()` - Count pieces on the board

### AI Logic (GameAi.c)
- `minimax()` - Core Minimax algorithm implementation
- `evaluatePositionEnhanced()` - Position evaluation using weighted matrix
- `isGameOver()` - Check if game has ended
- `applyMoveToBoard()` - Apply move to game state copy
- `copyGameState()` - Create independent copy of game state

### UI Rendering (main.c)
- `renderGrid()` - Draw the board and pieces
- `renderMainMenu()` - Display menu
- `displayWinner()` - Show game results
- `renderMetaData()` - Display current score and turn

## Performance Considerations

- **Game State Copying**: Each evaluation requires copying the game state for safe exploration
- **Search Depth**: Default depth of 2-4 provides reasonable balance between AI strength and response time
- **Move Ordering**: Moves could be ordered by heuristic for better pruning (future optimization)
- **Alpha-Beta Pruning**: The code includes a stub for `minimaxalpha()` for potential optimization

## Future Enhancements

1. **Alpha-Beta Pruning**: Implement for faster search
2. **Transposition Tables**: Cache previously evaluated positions
3. **Opening Book**: Pre-computed optimal opening moves
4. **Endgame Solver**: Perfect play for final moves
5. **Network Play**: Multiplayer over network
6. **Difficulty Levels**: Easy, Medium, Hard with appropriate depths
7. **Move History**: Undo/Redo functionality
8. **Timer**: Real-time gameplay constraints

## Technical Notes

- Board representation: 8×8 2D array with values:
  - `-1`: Empty cell
  - `0`: Black piece
  - `1`: White piece
  - `2`: Possible move (highlighted)

- Game states are managed as circular linked lists for player turns
- All coordinates are 0-indexed (0-7)

## License

This project is provided as-is for educational purposes.

## Author

Developed as a C programming project demonstrating game AI implementation, data structures, and graphics programming with Raylib.
