#pragma once

#include "../structs.hpp"
#include "../graphics/graphics.hpp"

class pInterface {
    public:
    pInterface();
    ~pInterface();

    int matrix[3][3]{}; // 0 - empty; 1 - X; 2 - 0
    int gold[3][3]{};
    bool turn = false; // 0 - X; 1 - 0
    bool firstTurn = true;
    int state = 0; // 0 - continue; 1 - win X; 2 - win 0; 3 - tie

    bool AI = true;
    bool AIXor0 = false;
    int AIDifficulty = 0; //0 - easy; 1 - hard; 2 - impossible

    bool settings = false;

    void drawSettings();
    void drawGrid();
    
    std::pair<int, int> getCursorPos();

    void resetGame();
    void handleClick();
    void detectState(bool showOutput = false);
    int detectLocalState(int mat[3][3]);

    bool eq3(int a, int b, int c);

    int minimax(int mat[3][3], int d, bool player);
    void generateRandomMove();
    int chance(int percent);
    void AIEasy();
    void AIHard();
    void AIImpossible();

    pGraphics graphics;
};