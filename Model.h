#ifndef MODEL_H_
#define MODEL_H_

#include <algorithm>
#include <cmath>
#include "Point.h"

struct Map {
    int **board, *top, M, N;

    Map(int M, int N, const int *_board, const int *_top, int noX, int noY) : M(M), N(N) {
        board = new int *[M];
        for (int i = 0; i < M; ++i) {
            board[i] = new int[N];
            for (int j = 0; j < N; ++j) {
                board[i][j] = _board[i * N + j];
            }
        }
        board[noX][noY] = 3;
        top = new int[N];
        for (int i = 0; i < N; ++i) top[i] = _top[i];
    }

    ~Map() {
        for (int i = 0; i < M; i++) delete[] board[i];
        delete[] board;
        delete[] top;
    }
};

struct UCT_Node {
    int optX{}, optY{};
    UCT_Node *father = nullptr;
    int num{}, win{}, total{}, next{}, definite{};
    UCT_Node *child[13]{};
    int childCount{};

    static int allTrials;

    UCT_Node() = default;

    UCT_Node(int optX, int optY, UCT_Node *father, int num, int next) :
            optX(optX), optY(optY), father(father), num(num), next(next) {}

    ~UCT_Node() { for (int i = 0; i < childCount; ++i) delete child[i]; }

    double confidence() const {
        return (double) win / (double) total + sqrt(log((double) allTrials) / (double) total);
    }

    UCT_Node *lastChild() const { return child[childCount - 1]; }

    UCT_Node *mostConfident() const {
        return *(std::max_element(child, child + childCount, [](UCT_Node *x, UCT_Node *y) {
            return x->confidence() < y->confidence();
        }));
    }

    UCT_Node *mostPossible() const {
        return *(std::max_element(child, child + childCount, [](UCT_Node *x, UCT_Node *y) {
            return x->win * y->total < y->win * x->total;
        }));
    }

    UCT_Node *addChild(int _optX, int _optY, UCT_Node *_father, int _num, int _next) {
        return child[childCount++] = new UCT_Node(_optX, _optY, _father, _num, _next);
    }
};

#endif
