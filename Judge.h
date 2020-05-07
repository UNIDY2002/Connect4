/********************************************************
*	Judge.h : 用于棋局评判的辅助函数                    *	
*	张永锋                                              *
*	zhangyf07@gmail.com                                 *
*	2014.5                                              *
*********************************************************/

#ifndef JUDGE_H_
#define JUDGE_H_

#include "Model.h"

/**
 * 判断某一方是否赢
 *
 * @param num    玩家编号
 * @param x      玩家的落子行
 * @param y      玩家的落子列
 * @param M      棋盘的行数
 * @param N      棋盘的列数
 * @param board  待检测的棋盘二维数组
 *
 * @return true  玩家赢
 *         false 玩家未赢
 *
 * @note         必须在落子前的棋盘基础上修改好(x,y)处的值之后再执行该判断函数。
 *               我们约定，检测玩家是否赢一定是在当前玩家落子之后，所以此时不需要考虑对手是否赢。
 *               实际上，对手一定没有赢，此时的情况只有两种:
 *                 1. 游戏尚未结束，双方都未赢；
 *                 2. 游戏以平局结束。
 *               对于以上这两种情况，我们可以用isTie函数进行进一步的判断。
 */
bool unitWin(int num, int x, int y, int M, int N, int *const *board);

/**
 * 与unitWin的区别在于，该函数有试填-擦除的步骤，且没有对竖直方向的检测。
 */
bool attemptWin(int num, int x, int y, Map &map);

#endif
