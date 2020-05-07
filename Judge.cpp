#include "Judge.h"

bool unitWin(const int num, const int x, const int y, const int M, const int N, int *const *board) {
    int i, j;

    //横向检测
    int count = 0;
    for (i = y; i >= 0; i--)
        if (board[x][i] != num)
            break;
    count += (y - i);
    for (i = y; i < N; i++)
        if (board[x][i] != num)
            break;
    count += (i - y - 1);
    if (count >= 4)
        return true;

    //纵向检测
    count = 0;
    for (i = x; i < M; i++)
        if (board[i][y] != num)
            break;
    count += (i - x);
    if (count >= 4)
        return true;

    //左下-右上
    count = 0;
    for (i = x, j = y; i < M && j >= 0; i++, j--)
        if (board[i][j] != num)
            break;
    count += (y - j);
    for (i = x, j = y; i >= 0 && j < N; i--, j++)
        if (board[i][j] != num)
            break;
    count += (j - y - 1);
    if (count >= 4)
        return true;

    //左上-右下
    count = 0;
    for (i = x, j = y; i >= 0 && j >= 0; i--, j--)
        if (board[i][j] != num)
            break;
    count += (y - j);
    for (i = x, j = y; i < M && j < N; i++, j++)
        if (board[i][j] != num)
            break;
    count += (j - y - 1);

    return count >= 4;
}

bool attemptWin(const int num, const int x, const int y, Map &map) {
    map.board[x][y] = num;

    int i, j, M = map.M, N = map.N;

    //横向检测
    int count = 0;
    for (i = y; i >= 0; i--)
        if (map.board[x][i] != num)
            break;
    count += (y - i);
    for (i = y; i < N; i++)
        if (map.board[x][i] != num)
            break;
    count += (i - y - 1);
    if (count >= 4) {
        map.board[x][y] = 0;
        return true;
    }

    //左下-右上
    count = 0;
    for (i = x, j = y; i < M && j >= 0; i++, j--)
        if (map.board[i][j] != num)
            break;
    count += (y - j);
    for (i = x, j = y; i >= 0 && j < N; i--, j++)
        if (map.board[i][j] != num)
            break;
    count += (j - y - 1);
    if (count >= 4) {
        map.board[x][y] = 0;
        return true;
    }

    //左上-右下
    count = 0;
    for (i = x, j = y; i >= 0 && j >= 0; i--, j--)
        if (map.board[i][j] != num)
            break;
    count += (y - j);
    for (i = x, j = y; i < M && j < N; i++, j++)
        if (map.board[i][j] != num)
            break;
    count += (j - y - 1);

    map.board[x][y] = 0;

    return count >= 4;
}
