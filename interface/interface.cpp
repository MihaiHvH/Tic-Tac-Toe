#include "../main.hpp"

pInterface::pInterface() {
}

pInterface::~pInterface() {
}

void pInterface::drawSettings() {
    settings = !settings;
    screen.render();
    if (!settings) {
        resetGame();
        if (AI && !AIXor0) handleClick();
    }
}

void pInterface::drawGrid() {
    if (settings) return;

    for (int i = 1; i <= 2; ++i) {
        graphics.drawRect({ i * screen.size.first / 3.f, 0 }, { 5, screen.size.second }, graphics.black);
        graphics.drawRect({ 0, i * screen.size.second / 3.f }, { screen.size.first, 5 }, graphics.black);
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (matrix[j][i] == 1) {
                std::pair<double, double> points[4] = {
                    { (i == 0) ? 5 : 10 + i * screen.size.first / 3.f, (j == 0) ? 5 : 10 + j * screen.size.second / 3.f },
                    { (i == 0) ? 20 : 25 + i * screen.size.first / 3.f, (j == 0) ? 5 : 10 + j * screen.size.second / 3.f },
                    { (i + 1) * screen.size.first / 3.f - 5, (j + 1) * screen.size.second / 3.f - 5 },
                    { (i + 1) * screen.size.first / 3.f - 20, (j + 1) * screen.size.second / 3.f - 5 }
                };
                graphics.draw4PointRect(points, gold[j][i] ? graphics.yellow : graphics.red);
                points[0] = { (i + 1) * screen.size.first / 3.f - 5, (j == 0) ? 5 : 10 + j * screen.size.second / 3.f };
                points[1] = { (i + 1) * screen.size.first / 3.f - 20, (j == 0) ? 5 : 10 + j * screen.size.second / 3.f };
                points[2] = { (i == 0) ? 5 : 10 + i * screen.size.first / 3.f, (j + 1) * screen.size.second / 3.f - 5 };
                points[3] = { (i == 0) ? 20 : 25 + i * screen.size.first / 3.f, (j + 1) * screen.size.second / 3.f - 5 };
                graphics.draw4PointRect(points, gold[j][i] ? graphics.yellow : graphics.red);
            }
            else if (matrix[j][i] == 2)
                graphics.drawOutlinedEllipse({ i * screen.size.first / 3.f + screen.size.first / 6.f + 2.5, j * screen.size.second / 3.f + screen.size.second / 6.f + 2.5 }, { screen.size.first / 6.f - 10, screen.size.second / 6.f - 10 }, 10, gold[j][i] ? graphics.yellow : graphics.blue);
        }
    }
}

std::pair<int, int> pInterface::getCursorPos() {
    return { screen.mousePointer.second / (screen.size.second / 3.f), screen.mousePointer.first / (screen.size.first / 3.f) };
}

void pInterface::resetGame() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            matrix[i][j] = 0, gold[i][j] = 0;
    state = 0;
    turn = AI ? AIXor0 : false;
    firstTurn = true;
    screen.render();
}

void pInterface::handleClick() {
    if (settings) return;

    if (state != 0) {
        resetGame();
        if (!AI) return;
        else if (turn) return;
    }
    std::pair<int, int> pos = getCursorPos();
    if (!AI) {
        if (matrix[pos.first][pos.second] == 0 && state == 0) {
            matrix[pos.first][pos.second] = turn + 1;
            turn = !turn;
            detectState(true);
        }
    }
    else {
        if (!turn && state == 0) { // AI
            if (AIDifficulty == 0) AIEasy();
            else if (AIDifficulty == 1) AIHard();
            else if (AIDifficulty == 2) AIImpossible();
            turn = !turn;
            detectState(true);
        }
        else if (matrix[pos.first][pos.second] == 0) { // HUMAN
            matrix[pos.first][pos.second] = AIXor0 ? 1 : 2;
            turn = !turn;
            detectState(true);
            if (state == 0) handleClick(); // AI
        }
    }
}

void pInterface::AIEasy() {
    if (chance(40)) AIImpossible();
    else generateRandomMove();
}

void pInterface::AIHard() {
    if (chance(90)) AIImpossible();
    else generateRandomMove();
}

void pInterface::AIImpossible() {
    if (firstTurn) {
        firstTurn = !firstTurn;
        if (matrix[1][1] == 0) {
            matrix[1][1] = AIXor0 + 1;
            screen.render();
        }
        else AIImpossible();
        return;
    }

    int bestScore = INT_MIN;
    std::pair<int, int> bestMove;
    
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (matrix[i][j] == 0) {
                matrix[i][j] = AIXor0 + 1;
                int score = minimax(matrix, 0, false);
                if (bestScore < score) {
                    bestScore = score;
                    bestMove = { i, j };
                }
                matrix[i][j] = 0;
            }
    matrix[bestMove.first][bestMove.second] = AIXor0 + 1;
}

int pInterface::chance(int percent) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(1, 100);
    return dist(mt) <= percent;
}

int pInterface::minimax(int mat[3][3], int d, bool player) {
    int localState = detectLocalState(mat);
    if (localState != 0) {
        if (localState == 1) return AIXor0 ? d - 10 : 10 - d;
        if (localState == 2) return AIXor0 ? 10 - d : d - 10;
        return 0;
    }
    int bestScore = player ? INT_MIN : INT_MAX;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        if (mat[i][j] == 0) {
            mat[i][j] = player ? (AIXor0 + 1) : (AIXor0 ? 1 : 2);
            int score = minimax(mat, d + 1, !player);
            if (player) bestScore = std::max(score, bestScore);
            else bestScore = std::min(score, bestScore);
            mat[i][j] = 0;
        }
    return bestScore;
}

bool pInterface::eq3(int a, int b, int c) {
    return a == b && b == c && a != 0;
}

int pInterface::detectLocalState(int mat[3][3]) {
    int localState = 0;
    for (int i = 0; i < 3 && !localState; ++i) {
        if (eq3(mat[i][0], mat[i][1], mat[i][2]))
            localState = mat[i][0];
        if(eq3(mat[0][i], mat[1][i], mat[2][i]))
            localState = mat[0][i];
    }
    if (!localState && (eq3(mat[0][0], mat[1][1], mat[2][2])))
        localState = mat[1][1];
    if (!localState && (eq3(mat[0][2], mat[1][1], mat[2][0])))
        localState = mat[1][1];
    if (!localState) {
        bool ext = 0;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (mat[i][j] == 0) {
                    ext = 1;
                    break;
                }
        if (!ext) localState = 3;
    }
    return localState;
}

void pInterface::detectState(bool showOutput) {
    for (int i = 0; i < 3 && !state; ++i) {
        if (eq3(matrix[i][0], matrix[i][1], matrix[i][2])) {
            state = matrix[i][0];
            if (showOutput) gold[i][0] = gold[i][1] = gold[i][2] = 1;
        }
        if(eq3(matrix[0][i], matrix[1][i], matrix[2][i])) {
            state = matrix[0][i];
            if (showOutput) gold[0][i] = gold[1][i] = gold[2][i] = 1;
        }
    }
    if (!state && (eq3(matrix[0][0], matrix[1][1], matrix[2][2]))) {
        state = matrix[1][1];
        if (showOutput) gold[0][0] = gold[1][1] = gold[2][2] = 1;
    }
    if (!state && (eq3(matrix[0][2], matrix[1][1], matrix[2][0]))) {
        state = matrix[1][1];
        if (showOutput) gold[0][2] = gold[1][1] = gold[2][0] = 1;
    }
    if (!state) {
        bool ext = 0;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (matrix[i][j] == 0) {
                    ext = 1;
                    break;
                }
        if (!ext) state = 3;
    }
    if (showOutput) {
        screen.render();
        if (state == 1)
            printf("X Wins!\n");
        else if (state == 2)
            printf("0 Wins!\n");
        else if (state == 3)
            printf("Tie!\n");
    }
}

void pInterface::generateRandomMove() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 2);
    int i, j;
    do { i = dist(mt); j = dist(mt); } while (matrix[i][j] != 0);
    matrix[i][j] = AIXor0 + 1;
}