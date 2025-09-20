#pragma once

#include "../structs.hpp"
#include "../graphics/graphics.hpp"

class pInterface {
    public:
    pInterface();
    ~pInterface();

    int matrix[3][3]{}; // 0 - gol; 1 - x; 2 - 0
    bool turn = 0; // 0 - x; 1 - y
    bool firstTurn = 1;
    int state = 0; // 0 - continue; 1 - win X; 2 - win 0; 3 - tie

    bool AI = true;
    bool AIRandomX = true;

    bool settings = false;

    void drawSettings();
    void draw();
    void drawLine(int i, int j, bool diag = false);
    
    std::pair<int, int> getCursorPos();

    void resetGame();
    void handleClick();
    void detectState(bool showOutput = false);

    bool eq3(int a, int b, int c);

    int minimax(int mat[3][3], int d, bool player);
    void generateRandomStartX();

    pGraphics graphics;
};