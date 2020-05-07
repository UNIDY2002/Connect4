#include <ctime>
#include <cstdlib>
#include "Strategy.h"
#include "Judge.h"

using namespace std;

int UCT_Node::allTrials;

extern "C" Point *getPoint(const int M, const int N, const int *top, const int *board,
                           const int, const int, const int noX, const int noY) {
    auto startTime = clock();
    srand(time(nullptr));
    int x = -1, y = -1;
    UCT_Node::allTrials = 0;
    UCT_Node root{};
    root.num = 1;
    while (clock() - startTime < 2.85 * CLOCKS_PER_SEC) {
        Map map(M, N, board, top, noX, noY);
        auto cursor = &root;
        while (cursor->definite == 0 && cursor->childCount && cursor->lastChild()->next == N) {
            cursor = cursor->mostConfident();
            oneStep(map, cursor->optX, cursor->optY, cursor->num);
        }
        int curr;
        if (cursor->childCount != 0)
            curr = cursor->lastChild()->next;
        else
            for (curr = 0; curr < N; ++curr)
                if (map.top[curr] > 0)
                    break;
        if (curr != N) {
            int next = curr + 1;
            for (; next < N; ++next)
                if (map.top[next] > 0)
                    break;
            cursor = cursor->addChild(map.top[curr] - 1, curr, cursor, 3 - cursor->num, next);
            oneStep(map, cursor->optX, cursor->optY, cursor->num);
        }
        if (cursor->definite == 0 && unitWin(cursor->num, cursor->optX, cursor->optY, map.M, map.N, map.board))
            cursor->definite = 1;
        int result = cursor->definite == 1 ? cursor->num : MonteCarlo(map, 3 - cursor->num);
        ++UCT_Node::allTrials;
        while (cursor) {
            ++cursor->total;
            if (cursor->num == result) ++cursor->win;
            cursor = cursor->father;
        }
    }

    if (root.childCount) {
        auto mostPossible = root.mostPossible();
        x = mostPossible->optX, y = mostPossible->optY;
    }

    if (y < 0 || y >= N || x < 0 || x != top[y] - 1) {
        for (int i = 0; i < N; ++i) {
            if (top[i] > 0) {
                x = top[i] - 1;
                y = i;
                break;
            }
        }
    }
    return new Point(x, y);
}

extern "C" void clearPoint(Point *p) {
    delete p;
}

void oneStep(Map &map, const int optX, const int optY, const int num) {
    map.board[optX][optY] = num;
    map.top[optY] = (optX > 0 && map.board[optX - 1][optY]) ? optX - 1 : optX;
}

int choose(Map &map, int num) {
    int avail[13], t = 0;
    for (int i = 0; i < map.N; ++i) {
        if (map.top[i] > 0) {
            avail[t++] = i;
            int tryX = map.top[i] - 1, tryY = i;
            map.board[tryX][tryY] = num;
            if (unitWin(num, tryX, tryY, map.M, map.N, map.board)) {
                map.board[tryX][tryY] = 0;
                return i;
            }
            map.board[tryX][tryY] = 0;
        }
    }
    auto suggestion = quickSuggestion(map, num);
    return (suggestion.first >= 0 && (((unsigned int) rand()) & 63u) < suggestion.second * 64) ? suggestion.first :
           (t ? avail[rand() % t] : -1);
}

int MonteCarlo(Map &map, int num) {
    int choiceX, choiceY;
    while ((choiceY = choose(map, num)) != -1) {
        choiceX = map.top[choiceY] - 1;
        oneStep(map, choiceX, choiceY, num);
        if (unitWin(num, choiceX, choiceY, map.M, map.N, map.board))
            return num;
        num = 3 - num;
    }
    return 0;
}

pair<int, double> quickSuggestion(Map &map, int num) {
    return catchControl(map, num);
    // return make_pair(-1, 0);
}

std::pair<int, double> catchControl(Map &map, int num) {
    bool control[13][13]{};
    for (int j = 0; j < map.N; ++j) {
        int threshold = j == 0 ? map.top[1] - 1 : (j == map.N - 1 ? map.top[map.N - 2] - 1 :
                                                   min(map.top[j - 1], map.top[j + 1]) - 1);
        if (threshold < 0) threshold = 0;
        for (int i = threshold; i < map.top[j]; ++i)
            if (map.board[i][j] == 3 || attemptWin(num, i, j, map))
                control[i][j] = true;
        if (map.top[j] >= 3 && control[map.top[j] - 3][j] && control[map.top[j] - 2][j]
            && map.board[map.top[j] - 3][j] == 0 && map.board[map.top[j] - 2][j] == 0
            && !attemptWin(3 - num, map.top[j] - 2, j, map))
            return make_pair(j, attemptWin(3 - num, map.top[j] - 3, j, map) ? 0.72 : 0.85);
    }
    const int sequence[] = {1, -1, 2, -2, 3, -3};
    int maxProfit = 0, maxColumn = -1;
    for (int j = 0; j < map.N; ++j) {
        if (map.top[j] > 0) {
            map.board[map.top[j] - 1][j] = num;
            for (int k:sequence) {
                int y = j + k, profit = 0;
                if (y >= 0 && y < map.N) {
                    int diff = abs(k), x = map.top[j] - 1, lowest = -1, last = -1, singleProfit = 0;
                    if (x - diff >= 0 && map.board[x - diff][y] == 0 && !control[x - diff][y]
                        && attemptWin(num, x - diff, y, map))
                        ++singleProfit, lowest = x - diff;
                    if (map.board[x][y] == 0 && !control[x][y] && attemptWin(num, x, y, map))
                        ++singleProfit, last = lowest, lowest = x;
                    if (x + diff < map.M - 1 && map.board[x + diff][y] == 0 && !control[x + diff][y]
                        && attemptWin(num, x + diff, y, map))
                        ++singleProfit, last = lowest, lowest = x + diff;

                    if (lowest != -1) {
                        if ((lowest > 0 && (control[lowest - 1][y] || last == lowest - 1)) || control[lowest + 1][y])
                            return make_pair(j, map.top[y] - lowest > 3 ? 0.70 : 0.86);
                        if (map.top[y] - lowest > 2) {
                            bool connect = true, linked = true;
                            for (int t = lowest + 1; t < map.top[y]; ++t) {
                                if (map.board[t][y] == 0 && !control[t][y]) {
                                    if (t < map.top[y] - 1)
                                        --singleProfit, linked = false;
                                    if (attemptWin(3 - num, t, y, map))
                                        connect = false;
                                }
                            }
                            if (connect) return make_pair(j, linked ? 0.88 : 0.75);
                        }
                    }
                    if (singleProfit > 0) profit += singleProfit;
                }
                if (maxProfit < profit) maxProfit = profit, maxColumn = j;
            }
            map.board[map.top[j] - 1][j] = 0;
        }
    }
    if (maxProfit == 0)
        return make_pair(-1, 0);
    else
        return make_pair(maxColumn, maxProfit < 4 ? 0.50 : 0.72);
}