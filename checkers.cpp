#include "Dependencies/SFML/include/SFML/Graphics.hpp"
#include <iostream>
#include <cstdlib>

class Board {
public:
    int size = 8;

    void Draw(sf::RenderWindow& window) {
        sf::RectangleShape tile;
        tile.setSize(sf::Vector2f(75.f, 75.f));
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                tile.setPosition(sf::Vector2f(75 * i, 75 * j));
                if ((i + j) % 2 == 0) {
                    tile.setFillColor(sf::Color::White);
                }
                else {
                    tile.setFillColor(sf::Color::Black);
                }
                window.draw(tile);
            }
        }
    }

    void Highlight(sf::RenderWindow& window, int x, int y) {
        sf::RectangleShape tile;
        tile.setSize(sf::Vector2f(75.f, 75.f));
        tile.setFillColor(sf::Color::Green);
        tile.setPosition(sf::Vector2f(75 * x, 75 * y));
        window.draw(tile);
    }
};

class Piece {
public:
    float radius = 30;
    int x;
    int y;
    bool isAlive;
    bool isKing = false;
    sf::Color color;

    void Draw(sf::RenderWindow& window) {
        if (isAlive) {
            sf::CircleShape shape(radius);
            shape.setFillColor(color);
            if (isKing) {
                shape.setOutlineThickness(3.f);
                shape.setOutlineColor(sf::Color::Yellow);
            }
            shape.setPosition(sf::Vector2f(x * 75 + (75 - radius * 2) / 2, y * 75 + (75 - 2 * radius) / 2));
            window.draw(shape);
        }
    }
};

void Setup(sf::RenderWindow& window, Piece* RedPieces, Piece* WhitePieces) {
    int m = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = (int)!(i % 2 & 1); j < 8; j += 2) {
            WhitePieces[m].isAlive = true;
            WhitePieces[m].x = j;
            WhitePieces[m].y = i;
            WhitePieces[m].isKing = false;
            m++;
        }
    }
    m = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = (int)(i % 2 & 1); j < 8; j += 2) {
            RedPieces[m].isAlive = true;
            RedPieces[m].x = j;
            RedPieces[m].y = 7 - i;
            RedPieces[m].isKing = false;
            m++;
        }
    }
}

Piece* FindPiece(int x, int y, Piece* RedPieces, Piece* WhitePieces) {
    for (int i = 0; i < 12; i++) {
        if (RedPieces[i].x == x && RedPieces[i].y == y) {
            if (RedPieces[i].isAlive) {
                return &RedPieces[i];
            }
        }
        if (WhitePieces[i].x == x && WhitePieces[i].y == y) {
            if (WhitePieces[i].isAlive) {
                return &WhitePieces[i];
            }
        }
    }
    return NULL;
}

void KillPiece(int x, int y, Piece* RedPieces, Piece* WhitePieces, int* turn) {
    FindPiece(x, y, RedPieces, WhitePieces)->isAlive = false;
    *turn = ((*turn == 1) ? 2 : 1);
    return;
}

int MovePiece(int x, int y, Piece* s_Piece, Piece* RedPieces, Piece* WhitePieces, int* turn) {
    // Check if the destination is within board boundaries
    if (x < 0 || x >= 8 || y < 0 || y >= 8) {
        return 0;
    }

    // Regular moves and captures for red pieces (including backward moves/captures for kings)
    if (s_Piece->color == sf::Color::Red || (s_Piece->color == sf::Color::White && s_Piece->isKing)) {
        // Regular moves (one square diagonally forward)
        if (!s_Piece->isKing) {
            if ((x == s_Piece->x - 1 && y == s_Piece->y - 1) || (x == s_Piece->x + 1 && y == s_Piece->y - 1)) {
                if (!FindPiece(x, y, RedPieces, WhitePieces)) {
                    *turn = (*turn == 1) ? 2 : 1;
                    s_Piece->x = x;
                    s_Piece->y = y;
                    return 1;
                }
            }
        }
        // Forward captures
        if (x == s_Piece->x - 2 && y == s_Piece->y - 2) {
            if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x + 1, y + 1, RedPieces, WhitePieces) != NULL &&
                FindPiece(x + 1, y + 1, RedPieces, WhitePieces)->color != s_Piece->color) {
                *turn = (*turn == 1) ? 2 : 1;
                KillPiece(x + 1, y + 1, RedPieces, WhitePieces, turn);
                s_Piece->x = x;
                s_Piece->y = y;
                return 1;
            }
        }
        if (x == s_Piece->x + 2 && y == s_Piece->y - 2) {
            if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x - 1, y + 1, RedPieces, WhitePieces) != NULL &&
                FindPiece(x - 1, y + 1, RedPieces, WhitePieces)->color != s_Piece->color) {
                *turn = (*turn == 1) ? 2 : 1;
                KillPiece(x - 1, y + 1, RedPieces, WhitePieces, turn);
                s_Piece->x = x;
                s_Piece->y = y;
                return 1;
            }
        }
        // Backward captures
        if (x == s_Piece->x - 2 && y == s_Piece->y + 2) {
            if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x + 1, y - 1, RedPieces, WhitePieces) != NULL &&
                FindPiece(x + 1, y - 1, RedPieces, WhitePieces)->color != s_Piece->color) {
                *turn = (*turn == 1) ? 2 : 1;
                KillPiece(x + 1, y - 1, RedPieces, WhitePieces, turn);
                s_Piece->x = x;
                s_Piece->y = y;
                return 1;
            }
        }
        if (x == s_Piece->x + 2 && y == s_Piece->y + 2) {
            if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x - 1, y - 1, RedPieces, WhitePieces) != NULL &&
                FindPiece(x - 1, y - 1, RedPieces, WhitePieces)->color != s_Piece->color) {
                *turn = (*turn == 1) ? 2 : 1;
                KillPiece(x - 1, y - 1, RedPieces, WhitePieces, turn);
                s_Piece->x = x;
                s_Piece->y = y;
                return 1;
            }
        }
        // Long-range diagonal moves for kings
        if (s_Piece->isKing) {
            int dx = x - s_Piece->x;
            int dy = y - s_Piece->y;
            // Check if the move is along a diagonal
            if (abs(dx) == abs(dy)) {
                // Ensure the destination is empty
                if (!FindPiece(x, y, RedPieces, WhitePieces)) {
                    // Check that all intermediate squares are empty
                    int steps = abs(dx);
                    int step_x = dx / steps;
                    int step_y = dy / steps;
                    bool valid = true;
                    for (int i = 1; i < steps; i++) {
                        int check_x = s_Piece->x + i * step_x;
                        int check_y = s_Piece->y + i * step_y;
                        if (FindPiece(check_x, check_y, RedPieces, WhitePieces)) {
                            valid = false;
                            break;
                        }
                    }
                    if (valid) {
                        *turn = (*turn == 1) ? 2 : 1;
                        s_Piece->x = x;
                        s_Piece->y = y;
                        return 1;
                    }
                }
            }
        }
    }

    // Regular moves and captures for white pieces (including backward moves/captures for kings)
    if (s_Piece->color == sf::Color::White || (s_Piece->color == sf::Color::Red && s_Piece->isKing)) {
        // Regular moves (one square diagonally forward)
        if (!s_Piece->isKing) {
            if ((x == s_Piece->x - 1 && y == s_Piece->y + 1) || (x == s_Piece->x + 1 && y == s_Piece->y + 1)) {
                if (!FindPiece(x, y, RedPieces, WhitePieces)) {
                    *turn = (*turn == 1) ? 2 : 1;
                    s_Piece->x = x;
                    s_Piece->y = y;
                    return 1;
                }
            }
        }
        // Forward captures
        if (x == s_Piece->x - 2 && y == s_Piece->y + 2) {
            if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x + 1, y - 1, RedPieces, WhitePieces) != NULL &&
                FindPiece(x + 1, y - 1, RedPieces, WhitePieces)->color != s_Piece->color) {
                *turn = (*turn == 1) ? 2 : 1;
                KillPiece(x + 1, y - 1, RedPieces, WhitePieces, turn);
                s_Piece->x = x;
                s_Piece->y = y;
                return 1;
            }
        }
        if (x == s_Piece->x + 2 && y == s_Piece->y + 2) {
            if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x - 1, y - 1, RedPieces, WhitePieces) != NULL &&
                FindPiece(x - 1, y - 1, RedPieces, WhitePieces)->color != s_Piece->color) {
                *turn = (*turn == 1) ? 2 : 1;
                KillPiece(x - 1, y - 1, RedPieces, WhitePieces, turn);
                s_Piece->x = x;
                s_Piece->y = y;
                return 1;
            }
        }
        // Backward captures
        if (x == s_Piece->x - 2 && y == s_Piece->y - 2) {
            if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x + 1, y + 1, RedPieces, WhitePieces) != NULL &&
                FindPiece(x + 1, y + 1, RedPieces, WhitePieces)->color != s_Piece->color) {
                *turn = (*turn == 1) ? 2 : 1;
                KillPiece(x + 1, y + 1, RedPieces, WhitePieces, turn);
                s_Piece->x = x;
                s_Piece->y = y;
                return 1;
            }
        }
        if (x == s_Piece->x + 2 && y == s_Piece->y - 2) {
            if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x - 1, y + 1, RedPieces, WhitePieces) != NULL &&
                FindPiece(x - 1, y + 1, RedPieces, WhitePieces)->color != s_Piece->color) {
                *turn = (*turn == 1) ? 2 : 1;
                KillPiece(x - 1, y + 1, RedPieces, WhitePieces, turn);
                s_Piece->x = x;
                s_Piece->y = y;
                return 1;
            }
        }
        // Long-range diagonal moves for kings
        if (s_Piece->isKing) {
            int dx = x - s_Piece->x;
            int dy = y - s_Piece->y;
            // Check if the move is along a diagonal
            if (abs(dx) == abs(dy)) {
                // Ensure the destination is empty
                if (!FindPiece(x, y, RedPieces, WhitePieces)) {
                    // Check that all intermediate squares are empty
                    int steps = abs(dx);
                    int step_x = dx / steps;
                    int step_y = dy / steps;
                    bool valid = true;
                    for (int i = 1; i < steps; i++) {
                        int check_x = s_Piece->x + i * step_x;
                        int check_y = s_Piece->y + i * step_y;
                        if (FindPiece(check_x, check_y, RedPieces, WhitePieces)) {
                            valid = false;
                            break;
                        }
                    }
                    if (valid) {
                        *turn = (*turn == 1) ? 2 : 1;
                        s_Piece->x = x;
                        s_Piece->y = y;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

// Check if a player has valid moves
bool HasValidMoves(Piece* pieces, Piece* RedPieces, Piece* WhitePieces, int turn) {
    for (int i = 0; i < 12; i++) {
        if (pieces[i].isAlive) {
            // Check regular moves and captures
            int x = pieces[i].x;
            int y = pieces[i].y;
            if (pieces[i].color == sf::Color::Red || (pieces[i].color == sf::Color::White && pieces[i].isKing)) {
                // Regular moves for red (or white kings)
                if (!pieces[i].isKing) {
                    if (x - 1 >= 0 && y - 1 >= 0 && !FindPiece(x - 1, y - 1, RedPieces, WhitePieces)) return true;
                    if (x + 1 < 8 && y - 1 >= 0 && !FindPiece(x + 1, y - 1, RedPieces, WhitePieces)) return true;
                }
                // Captures
                if (x - 2 >= 0 && y - 2 >= 0 && !FindPiece(x - 2, y - 2, RedPieces, WhitePieces) &&
                    FindPiece(x - 1, y - 1, RedPieces, WhitePieces) && FindPiece(x - 1, y - 1, RedPieces, WhitePieces)->color != pieces[i].color) return true;
                if (x + 2 < 8 && y - 2 >= 0 && !FindPiece(x + 2, y - 2, RedPieces, WhitePieces) &&
                    FindPiece(x + 1, y - 1, RedPieces, WhitePieces) && FindPiece(x + 1, y - 1, RedPieces, WhitePieces)->color != pieces[i].color) return true;
                if (x - 2 >= 0 && y + 2 < 8 && !FindPiece(x - 2, y + 2, RedPieces, WhitePieces) &&
                    FindPiece(x - 1, y + 1, RedPieces, WhitePieces) && FindPiece(x - 1, y + 1, RedPieces, WhitePieces)->color != pieces[i].color) return true;
                if (x + 2 < 8 && y + 2 < 8 && !FindPiece(x + 2, y + 2, RedPieces, WhitePieces) &&
                    FindPiece(x + 1, y + 1, RedPieces, WhitePieces) && FindPiece(x + 1, y + 1, RedPieces, WhitePieces)->color != pieces[i].color) return true;
                // King long-range moves
                if (pieces[i].isKing) {
                    for (int dx = -7; dx <= 7; dx++) {
                        int dy = dx;
                        if (x + dx >= 0 && x + dx < 8 && y + dy >= 0 && y + dy < 8 && abs(dx) == abs(dy) && !FindPiece(x + dx, y + dy, RedPieces, WhitePieces)) {
                            bool valid = true;
                            int steps = abs(dx);
                            int step_x = dx / steps;
                            int step_y = dy / steps;
                            for (int j = 1; j < steps; j++) {
                                if (FindPiece(x + j * step_x, y + j * step_y, RedPieces, WhitePieces)) {
                                    valid = false;
                                    break;
                                }
                            }
                            if (valid) return true;
                        }
                        dy = -dx;
                        if (x + dx >= 0 && x + dx < 8 && y + dy >= 0 && y + dy < 8 && abs(dx) == abs(dy) && !FindPiece(x + dx, y + dy, RedPieces, WhitePieces)) {
                            bool valid = true;
                            int steps = abs(dx);
                            int step_x = dx / steps;
                            int step_y = dy / steps;
                            for (int j = 1; j < steps; j++) {
                                if (FindPiece(x + j * step_x, y + j * step_y, RedPieces, WhitePieces)) {
                                    valid = false;
                                    break;
                                }
                            }
                            if (valid) return true;
                        }
                    }
                }
            }
            if (pieces[i].color == sf::Color::White || (pieces[i].color == sf::Color::Red && pieces[i].isKing)) {
                // Regular moves for white (or red kings)
                if (!pieces[i].isKing) {
                    if (x - 1 >= 0 && y + 1 < 8 && !FindPiece(x - 1, y + 1, RedPieces, WhitePieces)) return true;
                    if (x + 1 < 8 && y + 1 < 8 && !FindPiece(x + 1, y + 1, RedPieces, WhitePieces)) return true;
                }
                // Captures
                if (x - 2 >= 0 && y + 2 < 8 && !FindPiece(x - 2, y + 2, RedPieces, WhitePieces) &&
                    FindPiece(x - 1, y + 1, RedPieces, WhitePieces) && FindPiece(x - 1, y + 1, RedPieces, WhitePieces)->color != pieces[i].color) return true;
                if (x + 2 < 8 && y + 2 < 8 && !FindPiece(x + 2, y + 2, RedPieces, WhitePieces) &&
                    FindPiece(x + 1, y + 1, RedPieces, WhitePieces) && FindPiece(x + 1, y + 1, RedPieces, WhitePieces)->color != pieces[i].color) return true;
                if (x - 2 >= 0 && y - 2 >= 0 && !FindPiece(x - 2, y - 2, RedPieces, WhitePieces) &&
                    FindPiece(x - 1, y - 1, RedPieces, WhitePieces) && FindPiece(x - 1, y - 1, RedPieces, WhitePieces)->color != pieces[i].color) return true;
                if (x + 2 < 8 && y - 2 >= 0 && !FindPiece(x + 2, y - 2, RedPieces, WhitePieces) &&
                    FindPiece(x + 1, y - 1, RedPieces, WhitePieces) && FindPiece(x + 1, y - 1, RedPieces, WhitePieces)->color != pieces[i].color) return true;
                // King long-range moves
                if (pieces[i].isKing) {
                    for (int dx = -7; dx <= 7; dx++) {
                        int dy = dx;
                        if (x + dx >= 0 && x + dx < 8 && y + dy >= 0 && y + dy < 8 && abs(dx) == abs(dy) && !FindPiece(x + dx, y + dy, RedPieces, WhitePieces)) {
                            bool valid = true;
                            int steps = abs(dx);
                            int step_x = dx / steps;
                            int step_y = dy / steps;
                            for (int j = 1; j < steps; j++) {
                                if (FindPiece(x + j * step_x, y + j * step_y, RedPieces, WhitePieces)) {
                                    valid = false;
                                    break;
                                }
                            }
                            if (valid) return true;
                        }
                        dy = -dx;
                        if (x + dx >= 0 && x + dx < 8 && y + dy >= 0 && y + dy < 8 && abs(dx) == abs(dy) && !FindPiece(x + dx, y + dy, RedPieces, WhitePieces)) {
                            bool valid = true;
                            int steps = abs(dx);
                            int step_x = dx / steps;
                            int step_y = dy / steps;
                            for (int j = 1; j < steps; j++) {
                                if (FindPiece(x + j * step_x, y + j * step_y, RedPieces, WhitePieces)) {
                                    valid = false;
                                    break;
                                }
                            }
                            if (valid) return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

// Check win condition and return winner (1 for Red, 2 for White, 0 for no winner)
int CheckWin(Piece* RedPieces, Piece* WhitePieces, int turn) {
    // Count alive pieces
    int redCount = 0, whiteCount = 0;
    for (int i = 0; i < 12; i++) {
        if (RedPieces[i].isAlive) redCount++;
        if (WhitePieces[i].isAlive) whiteCount++;
    }
    // Check if one side has no pieces
    if (redCount == 0) return 2; // White wins
    if (whiteCount == 0) return 1; // Red wins
    // Check if the current player has no valid moves
    if (turn == 1 && !HasValidMoves(RedPieces, RedPieces, WhitePieces, turn)) return 2; // White wins
    if (turn == 2 && !HasValidMoves(WhitePieces, RedPieces, WhitePieces, turn)) return 1; // Red wins
    return 0; // No winner yet
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16.0;
    sf::RenderWindow window(sf::VideoMode(600, 600), "Checkers", sf::Style::Default, settings);
    sf::Event event;
    Board board;
    int grid[8][8];
    Piece RedPieces[12];
    Piece WhitePieces[12];
    bool selected = false;
    Piece* SelectedPiece = NULL;
    int turn = 1;
    int winner = 0;
    bool fontLoaded = false;

    // Load font for win message
    sf::Font font;
    sf::Text winText;
    if (font.loadFromFile("arial.ttf")) {
        fontLoaded = true;
        winText.setFont(font);
        winText.setCharacterSize(24);
        winText.setFillColor(sf::Color::Yellow);
        winText.setPosition(50, 250);
    }
    else {
        std::cout << "Error loading font 'arial.ttf'. Win messages will be displayed in the console.\n";
    }

    for (int i = 0; i < 12; i++) {
        WhitePieces[i].color = sf::Color::White;
        RedPieces[i].color = sf::Color::Red;
    }

    Setup(window, RedPieces, WhitePieces);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (winner != 0) {
                // Handle restart or quit
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::R) {
                        // Restart game
                        Setup(window, RedPieces, WhitePieces);
                        selected = false;
                        SelectedPiece = NULL;
                        turn = 1;
                        winner = 0;
                    }
                    else if (event.key.code == sf::Keyboard::Q) {
                        window.close();
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    selected = !selected;
                }
            }
        }

        window.clear();

        board.Draw(window);

        if (SelectedPiece != NULL) {
            board.Highlight(window, SelectedPiece->x, SelectedPiece->y);
        }

        for (int i = 0; i < 12; i++) {
            WhitePieces[i].Draw(window);
            RedPieces[i].Draw(window);
        }

        if (selected && winner == 0) {
            int x = sf::Mouse::getPosition(window).x / 75;
            int y = sf::Mouse::getPosition(window).y / 75;
            if (FindPiece(x, y, RedPieces, WhitePieces) &&
                (FindPiece(x, y, RedPieces, WhitePieces)->color == sf::Color::Red && turn == 1 ||
                    FindPiece(x, y, RedPieces, WhitePieces)->color == sf::Color::White && turn == 2)) {
                if (FindPiece(x, y, RedPieces, WhitePieces) == SelectedPiece) {
                    SelectedPiece = NULL;
                }
                else {
                    SelectedPiece = FindPiece(x, y, RedPieces, WhitePieces);
                }
                selected = false;
            }
            else if (SelectedPiece != NULL && MovePiece(x, y, SelectedPiece, RedPieces, WhitePieces, &turn)) {
                selected = false;
                SelectedPiece = NULL;
            }
            selected = false;
        }

        // Check for king promotion
        for (int i = 0; i < 12; i++) {
            if (RedPieces[i].y == 0 && RedPieces[i].isAlive) {
                RedPieces[i].isKing = true;
            }
            if (WhitePieces[i].y == 7 && WhitePieces[i].isAlive) {
                WhitePieces[i].isKing = true;
            }
        }

        // Check for win condition
        winner = CheckWin(RedPieces, WhitePieces, turn);
        if (winner != 0) {
            if (fontLoaded) {
                if (winner == 1) {
                    winText.setString("Red wins! Press R to restart or Q to quit");
                }
                else {
                    winText.setString("White wins! Press R to restart or Q to quit");
                }
                window.draw(winText);
            }
            else {
                // Fallback to console output
                if (winner == 1) {
                    std::cout << "Red wins! Press R to restart or Q to quit\n";
                }
                else {
                    std::cout << "White wins! Press R to restart or Q to quit\n";
                }
            }
        }

        window.display();
    }
    return 0;
}