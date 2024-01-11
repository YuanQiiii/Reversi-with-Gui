#include "jsoncpp/json.h"
#include <climits>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#define M 8
using namespace std;
vector<vector<int>> gridInfo(M,
                             vector<int>(M, 0)); // 先x后y，记录棋盘状态1/-1/0
int currBotColor; // 我所执子颜色（1为黑，-1为白，棋盘状态亦同）
int resultX, resultY; // 输出
double beginTime{0}, currentTime{0};
bool ValidityReverse(vector<vector<int>> &gridInfo, int i, int j, int color,
                     bool workmodel) {
  // color 黑色1/白色-1
  //  合法性&落子翻转复用
  //  model来决定函数的工作模式,返回值为落子的合法性(bool)
  //  true->合法检查并落子
  //  false->仅检查
  if (gridInfo[i][j]) {
    return false;
  }
  int opponentColor = -color;
  bool directionValidity[M]{0};
  int di[M]{1, -1, 0, 0, 1, -1, 1, -1};
  int dj[M]{0, 0, 1, -1, 1, -1, -1, 1};
  int I{-1}, J{-1};
  int count{0};
  for (int direction{0}; direction < M; direction++) {
    J = j + dj[direction], I = i + di[direction];
    while (J >= 0 && I >= 0 && J < M && I < M &&
           gridInfo[I][J] == opponentColor) {
      J += dj[direction];
      I += di[direction];
    }
    if (J >= 0 && I >= 0 && J < M && I < M && gridInfo[I][J] == color &&
        !(J == j + dj[direction] && I == i + di[direction])) {
      directionValidity[direction] = 1;
      count++;
    }
  }
  if (workmodel && count > 0) {
    gridInfo[i][j] = color;
    for (int direction{0}; direction < M; direction++) {
      if (directionValidity[direction]) {
        J = j + dj[direction], I = i + di[direction];
        while (J >= 0 && I >= 0 && J < M && I < M &&
               gridInfo[I][J] == opponentColor) {
          gridInfo[I][J] = color;
          J += dj[direction];
          I += di[direction];
        }
      }
    }
  }
  return (count > 0);
}

int WinnerCheck(vector<vector<int>> &gridInfo) {
  // 黑方胜利返回1,白方-1,平局和无胜负为0
  int blackNumber{0}, whiteNumber{0};
  for (int i{0}; i < M; i++) {
    for (int j{0}; j < M; j++) {
      if (gridInfo[i][j] == 1) {
        blackNumber++;
      } else if (gridInfo[i][j] == -1) {
        whiteNumber++;
      } else {
        if (ValidityReverse(gridInfo, i, j, 1, false) ||
            ValidityReverse(gridInfo, i, j, 1, false)) {
          return 0;
        }
      }
    }
  }
  if (blackNumber == whiteNumber) {
    return 0;
  }
  return (blackNumber > whiteNumber) ? 1 : -1;
}

int Evaluation(vector<vector<int>> &gridInfo, int color) {
  // 只对自己评分
  int winNumber{0};
  // 考虑胜负
  if (color == 1) {
    winNumber = WinnerCheck(gridInfo) * INT_MAX;
  } else {
    winNumber = WinnerCheck(gridInfo) * INT_MIN;
  }
  //  考虑对方的行动力,己方的棋子数
  int myNumber{0};
  int actionNumber{0};
  for (int i{0}; i < M; i++) {
    for (int j{0}; j < M; j++) {
      if (gridInfo[i][j] == color) {
        myNumber++;
      }
      if (ValidityReverse(gridInfo, i, j, -color, false)) {
        actionNumber++;
      }
    }
  }
  // 考虑稳定子
  int stableNumber{0};
  if (gridInfo[0][0] == color)
    stableNumber++;
  if (gridInfo[0][M - 1] == color)
    stableNumber++;
  if (gridInfo[M - 1][0] == color)
    stableNumber++;
  if (gridInfo[M - 1][M - 1] == color)
    stableNumber++;

  // 检查棋盘的四个边
  for (int i = 1; i < M - 1; i++) {
    if (gridInfo[0][i] == color && gridInfo[0][i - 1] == color &&
        gridInfo[0][i + 1] == color)
      stableNumber++;
    if (gridInfo[M - 1][i] == color && gridInfo[M - 1][i - 1] == color &&
        gridInfo[M - 1][i + 1] == color)
      stableNumber++;
    if (gridInfo[i][0] == color && gridInfo[i - 1][0] == color &&
        gridInfo[i + 1][0] == color)
      stableNumber++;
    if (gridInfo[i][M - 1] == color && gridInfo[i - 1][M - 1] == color &&
        gridInfo[i + 1][M - 1] == color)
      stableNumber++;
  }
  for (int i{1}; i < M - 1; i++) {
    for (int j{1}; j < M - 1; j++) {
      if (gridInfo[i][j] == color) {
        if ((gridInfo[i - 1][j] == -color) && (gridInfo[i + 1][j] == -color) &&
            (gridInfo[i][j - 1] == -color) && (gridInfo[i][j + 1] == -color)) {
          stableNumber++;
        }
      }
    }
  }
  // 权重
  const int weightMyNumber = 50;
  const int weightActionNumber = -100;
  const int weightStableNumber = 200;

  return weightStableNumber * stableNumber + weightActionNumber * actionNumber +
         weightMyNumber * myNumber + winNumber;
}

int MiniMaxAlphaBeta(vector<vector<int>> &gridInfo, int color, int depth,
                     int alpha, int beta) {
  // alpha和beta在不同的决策层更新,如果alpha大于等于beta,则该决策层的父节点不会在这一支上选择路线,可以被剪去
  vector<vector<int>> saveGrid = gridInfo;
  currentTime = clock();
  if (depth == 0 || (currentTime - beginTime) / CLOCKS_PER_SEC * 1000 > 995) {
    return Evaluation(gridInfo, currBotColor);
  }
  if (color == currBotColor) {
    // max层
    int bestValue = INT_MIN; // 根节点初始值
    for (int i = 0; i < M; i++) {
      for (int j = 0; j < M; j++) {
        if (ValidityReverse(gridInfo, i, j, color, true)) {
          int value =
              MiniMaxAlphaBeta(gridInfo, -color, depth - 1, alpha, beta);
          gridInfo = saveGrid;
          bestValue = max(bestValue, value); // 更新根节点
          alpha = max(alpha, bestValue);     // 更新alpha
          if (beta <= alpha) {               // alpha剪枝
            return bestValue;
          }
        }
      }
    }
    return bestValue;
  } else {
    // mini层
    int bestValue = INT_MAX; // 根节点初始值
    for (int i = 0; i < M; i++) {
      for (int j = 0; j < M; j++) {
        if (ValidityReverse(gridInfo, i, j, color, true)) {
          int value =
              MiniMaxAlphaBeta(gridInfo, -color, depth - 1, alpha, beta);
          gridInfo = saveGrid;
          bestValue = min(bestValue, value); // 更新根节点
          beta = min(beta, bestValue);       // 更新beta
          if (beta <= alpha) {               // beta剪枝
            return bestValue;
          }
        }
      }
    }
    return bestValue;
  }
}

struct Move {
  int x{-1}, y{-1};
};

Move Decision(int depth) {
  int value = INT_MIN;
  bool action{0};
  Move bestMove;
  vector<vector<int>> saveGrid(M, vector<int>(M, 0));
  saveGrid = gridInfo;
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      if (ValidityReverse(gridInfo, i, j, currBotColor, false)) {
        action = 1;
        break;
      }
    }
  }
  if (!action) {
    bestMove.x = -1;
    bestMove.y = -1;
    return bestMove;
  }
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      if (ValidityReverse(gridInfo, i, j, currBotColor, true)) {
        int tempValue = MiniMaxAlphaBeta(gridInfo, -currBotColor, depth - 1,
                                         INT_MIN, INT_MAX);
        gridInfo = saveGrid;
        if (tempValue >= value) {
          value = tempValue;
          bestMove.x = i;
          bestMove.y = j;
        }
      }
    }
  }
  return bestMove;
}

int main() {
  beginTime = clock();
  int x, y;
  gridInfo[3][4] = gridInfo[4][3] = 1;
  gridInfo[3][3] = gridInfo[4][4] = -1;
  string str;
  getline(cin, str);
  Json::Reader reader;
  Json::Value input;
  reader.parse(str, input);
  int turnID = input["responses"].size();
  currBotColor =
      input["requests"][(Json::Value::UInt)0]["x"].asInt() < 0 ? 1 : -1;
  for (int i = 0; i < turnID; i++) {
    x = input["requests"][i]["x"].asInt();
    y = input["requests"][i]["y"].asInt();
    if (x >= 0)
      ValidityReverse(gridInfo, x, y, -currBotColor, true);
    x = input["responses"][i]["x"].asInt();
    y = input["responses"][i]["y"].asInt();
    if (x >= 0)
      ValidityReverse(gridInfo, x, y, currBotColor, true);
  }
  x = input["requests"][turnID]["x"].asInt();
  y = input["requests"][turnID]["y"].asInt();
  if (x >= 0)
    ValidityReverse(gridInfo, x, y, -currBotColor, true);

  int allNumber{0};
  for (int i{0}; i < M; i++) {
    for (int j{0}; j < M; j++) {
      if (gridInfo[i][j]) {
        allNumber++;
      }
    }
  }
  int stageDepth{7};
  Move outAction = Decision(stageDepth);
  resultX = outAction.x;
  resultY = outAction.y;

  Json::Value ret;
  ret["response"]["x"] = resultX;
  ret["response"]["y"] = resultY;
  Json::FastWriter writer;
  cout << writer.write(ret) << endl;
  return 0;
}