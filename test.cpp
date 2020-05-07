#include <iostream>
#include "Strategy.h"

using namespace std;

int main() {
    freopen("in.txt", "r", stdin);
    int M, N, noX, noY;
    cin >> M >> N;
    int *board = new int[M * N]{};
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> board[i * N + j];
            if (board[i * N + j] == 3)
                noX = i, noY = j;
        }
    }
    int *top = new int[N]{};
    for (int j = 0; j < N; ++j) {
        for (int i = M - 1; i >= -1; --i) {
            if (i == -1 || board[i * N + j] == 0) {
                top[j] = i + 1;
                break;
            }
        }
    }
    auto result = getPoint(M, N, top, board, 0, 0, noX, noY);
    cout << result->x << ' ' << result->y << endl;
}