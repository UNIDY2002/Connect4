/********************************************************
*	Strategy.h : 策略接口文件                           *
*	张永锋                                              *
*	zhangyf07@gmail.com                                 *
*	2014.5                                              *
*********************************************************/

#ifndef STRATEGY_H_
#define STRATEGY_H_

#include <utility>
#include "Model.h"

extern "C" Point *getPoint(int M, int N, const int *top, const int *board, int lastX, int lastY, int noX, int noY);

extern "C" void clearPoint(Point *p);

inline void oneStep(Map &map, int optX, int optY, int num);

inline int choose(Map &map, int num);

inline int MonteCarlo(Map &map, int num);

inline std::pair<int, double> quickSuggestion(Map &map, int num);

inline std::pair<int, double> catchControl(Map &map, int num);

#endif