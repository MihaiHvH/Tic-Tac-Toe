#include "../main.hpp"

#include <random>

pInterface::pInterface() {
    //matrix[1][0] = 2;
    //matrix[2][0] = 1;
}

pInterface::~pInterface() {

}

void pInterface::draw() {
    for (int i = 1; i <= 2; ++i) {
        graphics.drawRect(std::make_pair(i * screen.size.first / 3.f, 0), std::make_pair(5, screen.size.second), graphics.black);
        graphics.drawRect(std::make_pair(0, i * screen.size.second / 3.f), std::make_pair(screen.size.first, 5), graphics.black);
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (matrix[j][i] == 1) {
                std::pair<double, double> points[4] = {
                    std::make_pair((i == 0) ? 5 : 10 + i * screen.size.first / 3.f, (j == 0) ? 5 : 10 + j * screen.size.second / 3.f),
                    std::make_pair((i == 0) ? 20 : 25 + i * screen.size.first / 3.f, (j == 0) ? 5 : 10 + j * screen.size.second / 3.f),
                    std::make_pair((i + 1) * screen.size.first / 3.f - 5, (j + 1) * screen.size.second / 3.f - 5),
                    std::make_pair((i + 1) * screen.size.first / 3.f - 20, (j + 1) * screen.size.second / 3.f - 5)
                };
                graphics.draw4PointRect(points, graphics.red);
                points[0] = std::make_pair((i + 1) * screen.size.first / 3.f - 5, (j == 0) ? 5 : 10 + j * screen.size.second / 3.f);
                points[1] = std::make_pair((i + 1) * screen.size.first / 3.f - 20, (j == 0) ? 5 : 10 + j * screen.size.second / 3.f);
                points[2] = std::make_pair((i == 0) ? 5 : 10 + i * screen.size.first / 3.f, (j + 1) * screen.size.second / 3.f - 5);
                points[3] = std::make_pair((i == 0) ? 20 : 25 + i * screen.size.first / 3.f, (j + 1) * screen.size.second / 3.f - 5);                
                graphics.draw4PointRect(points, graphics.red);
            }
            else if (matrix[j][i] == 2) {
                graphics.drawOutlinedEllipse(std::make_pair(
                    i * screen.size.first / 3.f + screen.size.first / 6.f + 2.5, j * screen.size.second / 3.f + screen.size.second / 6.f + 2.5
                ), std::make_pair(
                    screen.size.first / 6.f - 10, screen.size.second / 6.f - 10
                ), 10, graphics.blue);
            }
        }
    }
}

std::pair<int, int> pInterface::getCursorPos() {
    return std::make_pair(screen.mousePointer.second / (screen.size.second / 3), screen.mousePointer.first / (screen.size.first / 3));
}

void pInterface::handleClick() {
    if (state != 0) {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                matrix[i][j] = 0;
        state = 0;
        turn = 0;
        firstTurn = 1;
        screen.render();
    }

    std::pair<int, int> pos = getCursorPos();

    //printf("%d, %d\n", pos.first, pos.second);
    if (!AI) {
        if (matrix[pos.first][pos.second] == 0 && state == 0) {
            matrix[pos.first][pos.second] = turn + 1;
            turn = !turn;
            screen.render();
            detectState();
        }
    }
    else {
        if (!turn && state == 0) { // AI
            if (AIrandomX && firstTurn) {
                generateRandomStartX();
                firstTurn = false;
                turn = !turn;
                screen.render();
                return;
            }
            int bestScore = INT_MIN;
            std::pair<int, int> bestMove;
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    if (matrix[i][j] == 0) {
                        matrix[i][j] = 1;
                        int aux = state;
                        int score = minimax(matrix, 0, false);
                        state = aux;
                        if (bestScore < score) {
                            bestScore = score;
                            bestMove = std::make_pair(i, j);
                        }
                        matrix[i][j] = 0;
                    }
            matrix[bestMove.first][bestMove.second] = 1;
            turn = !turn;
            screen.render();
            detectState(true);
        }
        else if (matrix[pos.first][pos.second] == 0) { // HUMAN
            matrix[pos.first][pos.second] = 2;
            turn = !turn;
            screen.render();
            detectState(true);
            if (state == 0) handleClick();
        }
    }
}

int pInterface::minimax(int mat[3][3], int d, bool player) {
    detectState();
    if (state != 0) {
        if (state == 1) return 10 - d;
        if (state == 2) return d - 10;
        return 0;
    }
    int bestScore = player ? INT_MIN : INT_MAX;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        if (mat[i][j] == 0) {
            mat[i][j] = !player + 1;
            if (player) bestScore = std::max(minimax(mat, d + 1, false), bestScore);
            else bestScore = std::min(minimax(mat, d + 1, true), bestScore);
            mat[i][j] = 0;
        }
    return bestScore;
}

bool pInterface::eq3(int a, int b, int c) {
    return a == b && b == c && a != 0;
}

void pInterface::detectState(bool showOutput) {
    for (int i = 0; i < 3 && !state; ++i) {
        if (eq3(matrix[i][0], matrix[i][1], matrix[i][2]))
            state = matrix[i][0];
        if(eq3(matrix[0][i], matrix[1][i], matrix[2][i]))
            state = matrix[0][i];
    }
    if (!state && (eq3(matrix[0][0], matrix[1][1], matrix[2][2]) || eq3(matrix[0][2], matrix[1][1], matrix[2][0])))
        state = matrix[1][1];
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
        if (state == 1)
            printf("X Wins!\n");
        else if (state == 2)
            printf("0 Wins!\n");
        else if (state == 3)
            printf("Tie!\n");
    }
}

void pInterface::generateRandomStartX() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 2);
    matrix[dist(mt)][dist(mt)] = 1;
}