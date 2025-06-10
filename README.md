# Checkers Game

A simple implementation of the classic Checkers game in C++ using the [SFML](https://www.sfml-dev.org/) library.

## Screenshot

## Features

- Graphical interface with SFML
- Two-player mode on the same computer
- Classic checkers rules: moves, captures, king promotion
- Win detection and endgame message
- Option to restart or quit after the game ends

## How to Run

1. **Install SFML**  
   Download and install SFML for your system: [https://www.sfml-dev.org/download.php](https://www.sfml-dev.org/download.php)

2. **Build the Project**  
   Compile `checkers.cpp` with SFML linked. For example, using MinGW (Windows):

   ```
   g++ checkers.cpp -o checkers -IC:/path/to/SFML/include -LC:/path/to/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system
   ```

   Or use CMake/Visual Studio as appropriate.

3. **Run the Game**  
   Make sure the font file `arial.ttf` is in the same folder as the executable (or change the font in the code).
   Then run:

   ```
   ./checkers
   ```

## Controls

- **Left mouse button** — select a piece and make a move
- **R** — restart the game after it ends
- **Q** — quit the game after it ends

## Code Structure

- `Board` — draws the game board
- `Piece` — represents a checker piece (position, color, status, king)
- Game logic: piece selection, moves, captures, win checking, king promotion

## Requirements

- C++17 or newer
- SFML 2.5 or newer

**Note:**  
This project was created as part of an educational practice to improve C++ and SFML skills.
