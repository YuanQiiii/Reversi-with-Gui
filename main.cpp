// 使用了EasyX库实现了项目的图形化
#include <EasyX.h>
#include <windows.h>

#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
#define M 8
// mainGrid 为游戏棋盘
vector<vector<int>> mainGrid(M, vector<int>(M, 0));
// displayGrid 为图形化棋盘
vector<vector<int>> displayGrid(M, vector<int>(M, 0));
// color代表织棋颜色,全局统一 , 1->黑色, -1->白色
int currBotColor{0};
int firstColor{1};

ExMessage interaction; // 鼠标事件
// 一些函数的声明(函数原型),用于解决找不到指定标识符的错误
void Play(int color);
int InteractionMenu();
bool InteractionGameIn();
void DisplayStartMenu();
int InteractionStartMenu();
int RoundLoad(vector<vector<int>> &mainGrid, int order);
void InteractionLoad_Part(int order);
void Initialization(vector<vector<int>> &mainGrid) {
  // 初始化参数中的棋盘
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      mainGrid[i][j] = 0;
    }
  }
  mainGrid[3][3] = -1;
  mainGrid[4][4] = -1;
  mainGrid[3][4] = 1;
  mainGrid[4][3] = 1;
}
bool ValidityReverse(vector<vector<int>> &mainGrid, int i, int j, int color,
                     bool workModel) {
  // 相比于上一代的游戏相应逻辑实现做了简化
  //  color 黑色1/白色-1
  //   合法性&落子翻转复用
  //   model来决定函数的工作模式,返回值为落子的合法性(bool)
  //   true->合法检查并落子
  //   false->仅检查
  if (mainGrid[i][j]) {
    return false;
  }
  int opponentColor{-color};
  bool directionValidity[M]{0};
  int di[M]{1, -1, 0, 0, 1, -1, 1, -1};
  int dj[M]{0, 0, 1, -1, 1, -1, -1, 1};
  int I{-1}, J{-1};
  int count{0};
  for (int direction{0}; direction < M; direction++) {
    J = j + dj[direction], I = i + di[direction];
    while (J >= 0 && I >= 0 && J < M && I < M &&
           mainGrid[I][J] == opponentColor) {
      J += dj[direction];
      I += di[direction];
    }
    if (J >= 0 && I >= 0 && J < M && I < M && mainGrid[I][J] == color &&
        !(J == j + dj[direction] && I == i + di[direction])) {
      directionValidity[direction] = 1;
      count++;
    }
  }
  if (workModel && count > 0) {
    mainGrid[i][j] = color;
    for (int direction{0}; direction < M; direction++) {
      if (directionValidity[direction]) {
        J = j + dj[direction], I = i + di[direction];
        while (J >= 0 && I >= 0 && J < M && I < M &&
               mainGrid[I][J] == opponentColor) {
          mainGrid[I][J] = color;
          J += dj[direction];
          I += di[direction];
        }
      }
    }
  }
  return (count > 0);
}
int WinnerCheck(vector<vector<int>> &mainGrid) {
  // 主要用于评估函数
  // 黑方胜利返回1,白方-1,平局和无胜负为0
  int blackNumber{0}, whiteNumber{0};
  for (int i{0}; i < M; i++) {
    for (int j{0}; j < M; j++) {
      if (mainGrid[i][j] == 1) {
        blackNumber++;
      } else if (mainGrid[i][j] == -1) {
        whiteNumber++;
      } else {
        if (ValidityReverse(mainGrid, i, j, 1, false) ||
            ValidityReverse(mainGrid, i, j, 1, false)) {
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

int Evaluation(vector<vector<int>> &mainGrid, int color) {
  // 只对自己评分,是minimax算法实现的主要函数
  int winNumber{0};
  // 考虑胜负
  if (color == 1) {
    winNumber = WinnerCheck(mainGrid) * INT_MAX;
  } else {
    winNumber = WinnerCheck(mainGrid) * INT_MIN;
  }
  //  考虑对方的行动力,己方的棋子数
  int myNumber{0};
  int actionNumber{0};
  for (int i{0}; i < M; i++) {
    for (int j{0}; j < M; j++) {
      if (mainGrid[i][j] == color) {
        myNumber++;
      }
      if (ValidityReverse(mainGrid, i, j, -color, false)) {
        actionNumber++;
      }
    }
  }
  // 权重
  const int weightMyNumber{25};
  const int weightActionNumber{-50};

  return weightActionNumber * actionNumber + weightMyNumber * myNumber +
         winNumber;
}

int MiniMaxAlphaBeta(vector<vector<int>> &mainGrid, int color, int depth,
                     int alpha, int beta) {
  // alpha 和 beta 在不同的决策层更新,如果 alpha 大于等于 beta
  // ,则该决策层的父节点不会在这一支上选择,可以被剪去
  vector<vector<int>> saveGrid = mainGrid;
  if (depth == 0) {
    return Evaluation(mainGrid, currBotColor);
  }
  if (color == currBotColor) {
    // max层
    int bestValue{INT_MIN}; // 根节点初始值
    for (int i = 0; i < M; i++) {
      for (int j = 0; j < M; j++) {
        if (ValidityReverse(mainGrid, i, j, color, true)) {
          int value{MiniMaxAlphaBeta(mainGrid, -color, depth - 1, alpha, beta)};
          mainGrid = saveGrid;
          bestValue = max(bestValue, value); // 更新根节点
          alpha = max(alpha, bestValue);     // 更新 alpha
          if (beta <= alpha) {               // alpha剪枝
            return bestValue;
          }
        }
      }
    }
    return bestValue;
  } else {
    // mini层
    int bestValue{INT_MAX}; // 根节点初始值
    for (int i = 0; i < M; i++) {
      for (int j = 0; j < M; j++) {
        if (ValidityReverse(mainGrid, i, j, color, true)) {
          int value{MiniMaxAlphaBeta(mainGrid, -color, depth - 1, alpha, beta)};
          mainGrid = saveGrid;
          bestValue = min(bestValue, value); // 更新根节点
          beta = min(beta, bestValue);       // 更新 beta
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
  // 决策函数
  int value{INT_MIN};
  bool action{0}; // 用于检测是否有合法步
  Move bestMove;
  // 用于保存原始的棋盘,在模拟下子时候恢复棋盘
  vector<vector<int>> saveGrid(M, vector<int>(M, 0));
  saveGrid = mainGrid;
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      if (ValidityReverse(mainGrid, i, j, currBotColor, false)) {
        action = 1;
        break;
      }
    }
  }
  // 没有合法步,返回只经过初始化的Move
  if (!action) {
    return bestMove;
  }
  // 有合法步,调用MiniMaxAlphaBeta得到最优解并返回
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      if (ValidityReverse(mainGrid, i, j, currBotColor, true)) {
        int tempValue{MiniMaxAlphaBeta(mainGrid, -currBotColor, depth - 1,
                                       INT_MIN, INT_MAX)};
        mainGrid = saveGrid;
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
bool Rights(vector<vector<int>> &mainGrid, int color) {
  // 用于判断是否有下棋的权力
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      if (ValidityReverse(mainGrid, i, j, color, false)) {
        return true;
      }
    }
  }
  return false;
}
int PieceNumber(vector<vector<int>> &mainGrid, int color) {
  // 用于统计棋子数目
  int sum = 0;
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      if (mainGrid[i][j] == color) {
        sum++;
      }
    }
  }
  return sum;
}
void GridCopy(vector<vector<int>> &mainGrid, vector<vector<int>> &displayGrid,
              int color) {
  // 实现mainGrid到displayGrid的赋值,同时将相应的提示符一并赋值
  displayGrid = mainGrid;
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      if (ValidityReverse(displayGrid, i, j, color, false)) {
        displayGrid[i][j] = 2; // 用于绘出提示符
      }
    }
  }
}
void DisplayPreparation() { initgraph(900, 700); }

void DisplayMenu() {
  // 显示主菜单
  setbkcolor(RGB(173, 131, 139));
  cleardevice();
  setfillcolor(RGB(146, 172, 209));
  solidcircle(180, 450, 70);
  solidcircle(360, 450, 70);
  solidcircle(540, 450, 70);
  solidcircle(720, 450, 70);
  setbkmode(TRANSPARENT);
  settextstyle(50, 0, _T("Consolas"));
  RECT r = {200, 100, 700, 200};
  drawtext(_T("Othello"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  settextstyle(20, 0, _T("Consolas"));
  r = {90, 440, 270, 460};
  drawtext(_T("Start New Game"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  r = {270, 440, 450, 460};
  drawtext(_T("Load Game"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  r = {450, 440, 630, 460};
  drawtext(_T("Rank"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  r = {630, 440, 810, 460};
  drawtext(_T("Quit"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
void DisplayClear() {
  // 用于在绘制下一次对局内容前清空原来的对局内容
  // 利用BeginBatchDraw()和EndBatchDraw(),防止闪烁
  BeginBatchDraw();
  setbkcolor(RGB(225, 218, 211));
  cleardevice();
  setfillcolor(RGB(133, 133, 140));
  solidroundrect(25, 25, 675, 675, 40, 40);
  solidroundrect(700, 25, 875, 675, 40, 40);
  line(70, 70, 70, 630);
  line(140, 70, 140, 630);
  line(210, 70, 210, 630);
  line(280, 70, 280, 630);
  line(350, 70, 350, 630);
  line(420, 70, 420, 630);
  line(490, 70, 490, 630);
  line(560, 70, 560, 630);
  line(630, 70, 630, 630);
  line(70, 70, 630, 70);
  line(70, 140, 630, 140);
  line(70, 210, 630, 210);
  line(70, 280, 630, 280);
  line(70, 350, 630, 350);
  line(70, 420, 630, 420);
  line(70, 490, 630, 490);
  line(70, 560, 630, 560);
  line(70, 630, 630, 630);
  setfillcolor(RGB(146, 172, 209));
  solidcircle(787, 440, 50);
  solidcircle(787, 550, 50);
  setfillcolor(RGB(83, 82, 78));
  solidcircle(787, 120, 30);
  setfillcolor(RGB(229, 227, 221));
  solidcircle(787, 180, 30);
  setbkmode(TRANSPARENT);
  settextcolor(WHITE);
  settextstyle(25, 0, _T("Consolas"));
  RECT r = {740, 420, 840, 460}; // left,top,right,bottom
  drawtext(_T("Save"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  r = {740, 530, 840, 570};
  drawtext(_T("Quit"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  EndBatchDraw();
}
void DisplayGameLoad() {
  // 显示读档菜单
  setbkcolor(RGB(225, 218, 211));
  cleardevice();
  setfillcolor(RGB(133, 133, 140));
  solidrectangle(200, 100, 700, 200); // left,top,right,bottom
  solidrectangle(200, 300, 700, 400);
  solidrectangle(200, 500, 700, 600);
  setbkmode(TRANSPARENT);
  settextstyle(50, 0, _T("Consolas"));
  RECT r = {200, 100, 700, 200};
  drawtext(_T("file 1"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  r = {200, 300, 700, 400};
  drawtext(_T("file 2"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  r = {200, 500, 700, 600};
  drawtext(_T("file 3"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
void InteractionLoad_Part(int order) {
  // Interaction_GameLoad()的子函数,用于减少代码重复
  int num{RoundLoad(mainGrid, order)};
  if (num == 0) {
    cout << "Please Save Your Game first!!" << endl;
    DisplayMenu();
    InteractionMenu();
  } else {
    if (num == 1) {
      firstColor = 1;
      DisplayStartMenu();
      currBotColor = -InteractionStartMenu();
      Play(firstColor);
    } else if (num == -1) {
      firstColor = -1;
      DisplayStartMenu();
      currBotColor = -InteractionStartMenu();
      Play(firstColor);
    }
  }
  return;
}
void InteractionLoad() {
  // 读档交互
  int x, y;
  while (true) {
    getmessage(&interaction, EX_MOUSE);
    if (interaction.lbutton) {
      x = interaction.x;
      y = interaction.y;
      if (x >= 200 && x <= 700 && y >= 100 && y <= 200) {
        InteractionLoad_Part(1);
        return;
      } else if (x >= 200 && x <= 700 && y >= 300 && y <= 400) {
        InteractionLoad_Part(2);
        return;
      } else if (x >= 200 && x <= 700 && y >= 500 && y <= 600) {
        InteractionLoad_Part(3);
        return;
      }
    }
  }
}
void DrawGrid(int x, int y, int color) {
  // 绘制棋子
  int tempX{105 + x * 70}, tempY{105 + y * 70};
  if (color == 1) {
    setfillcolor(RGB(83, 82, 78));
    solidcircle(tempX, tempY, 30);
  } else if (color == -1) {
    setfillcolor(RGB(229, 227, 221));
    solidcircle(tempX, tempY, 30);
  }
}
IMAGE image;
void Expressions() {
  // 表情包
  int blackNumber{PieceNumber(mainGrid, 1)},
      whiteNumber{PieceNumber(mainGrid, -1)};
  if (currBotColor == -1) {
    if (blackNumber > whiteNumber) {
      loadimage(&image, _T("sad.ico"));
      putimage(723, 247, &image);
    } else if (blackNumber < whiteNumber) {
      loadimage(&image, _T("scornful.ico"));
      putimage(723, 247, &image);
    } else if (blackNumber == whiteNumber) {
      loadimage(&image, _T("careful.ico"));
      putimage(723, 247, &image);
    }
  } else {
    if (blackNumber < whiteNumber) {
      loadimage(&image, _T("sad.ico"));
      putimage(723, 247, &image);
    } else if (blackNumber > whiteNumber) {
      loadimage(&image, _T("scornful.ico"));
      putimage(723, 247, &image);
    } else if (blackNumber == whiteNumber) {
      loadimage(&image, _T("careful.ico"));
      putimage(723, 247, &image);
    }
  }
}
void DisplayGrid() {
  // 绘制棋盘棋子和相关元素
  int blackNumber{0};
  int whiteNumber{0};
  int tempX, tempY;
  BeginBatchDraw();
  DisplayClear();
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      switch (displayGrid[i][j]) {
      case 1:
        DrawGrid(j, i, 1);
        blackNumber++;
        break;
      case -1:
        DrawGrid(j, i, -1);
        whiteNumber++;
        break;
      case 2: // 提示符
        tempX = 105 + j * 70;
        tempY = 105 + i * 70;
        setfillcolor(RGB(196, 122, 155));
        solidcircle(tempX, tempY, 10);
        break;
      }
    }
  }
  // 显示双方棋子数
  settextstyle(20, 0, _T("Consolas"));
  setbkmode(TRANSPARENT);
  settextcolor(WHITE);
  TCHAR w[5];
  _stprintf_s(w, _T("%d"), blackNumber);
  outtextxy(780, 110, w);
  settextcolor(BLACK);
  TCHAR b[5];
  _stprintf_s(b, _T("%d"), whiteNumber);
  outtextxy(780, 175, b);
  Expressions();
  EndBatchDraw();
}

void DisplayStartMenu() {
  // 绘制选择颜色的菜单
  setbkcolor(RGB(173, 131, 139));
  cleardevice();
  setfillcolor(RGB(83, 82, 78));
  solidcircle(300, 450, 100);
  setfillcolor(RGB(229, 227, 221));
  solidcircle(600, 450, 100);
  RECT r = {300, 140, 600, 280};
  settextstyle(25, 0, _T("Consolas"));
  drawtext(_T("Choose Your Color"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
int InteractionStartMenu() {
  // 交互选择颜色,并返回选择的颜色
  int x{0}, y{0};
  while (true) {
    getmessage(&interaction, EX_MOUSE);
    if (interaction.lbutton) {
      x = interaction.x;
      y = interaction.y;
      if ((x - 300) * (x - 300) + (y - 450) * (y - 450) < 10000) {
        return 1;
      } else if ((x - 600) * (x - 600) + (y - 450) * (y - 450) < 10000) {
        return -1;
      }
    }
  }
}
void GameOver() {
  // 用于显示对局结果
  setbkcolor(RGB(225, 218, 211));
  cleardevice();
  setbkmode(TRANSPARENT);
  settextstyle(50, 0, _T("Consolas"));
  RECT r = {200, 100, 700, 200};
  if (PieceNumber(mainGrid, 1) > PieceNumber(mainGrid, -1)) {
    drawtext(_T("Black Win !"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  } else if (PieceNumber(mainGrid, 1) < PieceNumber(mainGrid, -1)) {
    drawtext(_T("White Win !"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  } else {
    drawtext(_T("All Even !"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  }
  Sleep(1000); // 休眠1000ms
}
struct Input {
  int x{0}, y{0};
} inAction; // 用于用户输入操作

struct SaveFile {
  int mainGrid[8][8]{0};
}; // 用于存档
struct RankFile {
  int blackNumber{0}, whiteNumber{0};
}; // 用于保存对局数据
void RoundSave(vector<vector<int>> &mainGrid) {
  // 存档,使用了二进制写入,相比于之前的文本文档写入节省了空间和读取效率,略去了文本文件需要的字符串转化操作
  SaveFile oneFile;
  fstream outfile;
  // 使用固定长度的数组存储对局
  for (int i{0}; i < M; i++) {
    for (int j{0}; j < M; j++) {
      oneFile.mainGrid[i][j] = mainGrid[i][j];
    }
  }
  // 如果目录下没有相应的文件,则创建新文件,否则追加写入
  outfile.open("SaveFile.bin", ios::app | ios::binary);
  outfile.write((const char *)&oneFile, sizeof(oneFile));
  outfile.close();
  return;
}
int RoundLoad(vector<vector<int>> &mainGrid, int order) {
  // 读档
  // 存档编号不存在,返回0
  // 读取时会计算先手方,若黑棋先手,返回1,否则为-1
  vector<SaveFile> files;
  SaveFile oneFile;
  fstream infile;
  infile.open("SaveFile.bin", ios::in | ios::binary);
  streampos begin = infile.tellp();
  infile.seekp(0, ios::end); // 文件指针跳转到文件尾部
  streampos end = infile.tellp();
  infile.seekg(0, ios::beg);                          // 跳转回开头
  int length{(int)((end - begin) / sizeof(oneFile))}; // 用于计算存档数量
  cout << "round number: " << length << endl;
  if (order > length) {
    return 0;
  }
  // 使用动态数组读档减少了内存浪费
  for (int i{0}; i < length; i++) {
    infile.read((char *)&oneFile, sizeof(oneFile));
    files.push_back(oneFile);
  }
  // 更改棋盘
  for (int i{0}; i < M; i++) {
    for (int j{0}; j < M; j++) {
      mainGrid[i][j] = files[length - order].mainGrid[i][j];
    }
  }
  // 根据棋子数奇数偶数计算先手
  int allNumber{PieceNumber(mainGrid, 1) + PieceNumber(mainGrid, -1)};
  return (allNumber % 2 == 0) ? 1 : -1;
}

void RankSave(vector<vector<int>> &mainGrid) {
  // 存储对局结果
  RankFile oneRank;
  fstream outfile;
  oneRank.blackNumber = PieceNumber(mainGrid, 1);
  oneRank.whiteNumber = PieceNumber(mainGrid, -1);
  // 如果目录下没有相应的文件,则创建新文件,否则追加写入
  outfile.open("RankFile.bin", ios::app | ios::binary);
  outfile.write((const char *)&oneRank, sizeof(oneRank));
  outfile.close();
  return;
}
void DisplayRank() {
  // 显示对局结果
  vector<RankFile> ranks;
  RankFile oneRank;
  int h{100};
  fstream infile;
  infile.open("RankFile.bin", ios::in | ios::binary);
  streampos begin = infile.tellp();
  infile.seekp(0, ios::end);
  streampos end = infile.tellp();
  infile.seekg(0, ios::beg);
  int length{(int)((end - begin) / sizeof(oneRank))};
  for (int i{0}; i < length; i++) {
    infile.read((char *)&oneRank, sizeof(oneRank));
    ranks.push_back(oneRank);
  }
  cout << "rank length: " << length << endl;
  setbkcolor(RGB(225, 218, 211));
  cleardevice();
  setbkmode(TRANSPARENT);
  settextcolor(BLACK);
  int i{0};
  // 在规定长度内显示对局结果
  while (h < 690 && i < length) {
    settextstyle(20, 0, _T("Consolas"));
    TCHAR w[1000];
    _stprintf_s(w,
                _T("%d - - - - - - - - - - - - - - - - - - - - - - - - - - - ")
                _T("- - BLACK %d / WHITE %d"),
                i + 1, ranks[i].blackNumber, ranks[i].whiteNumber);
    outtextxy(100, h, w);
    h += 20;
    i++;
  }
  Sleep(1000);
  DisplayMenu();
  InteractionMenu();
}

void Play(int color) {
  // 对局函数,设置了两种对局顺序
  // 玩家先手
  if (color == -currBotColor) {
    while (true) {
      GridCopy(mainGrid, displayGrid, -currBotColor);
      DisplayGrid();
      if (Rights(mainGrid, -currBotColor)) {
        while (true) {
          if (InteractionGameIn()) {
            // 注意从交互操作中获取的xy是真实的xy,x正方向在右方,y正方向在下方
            if (ValidityReverse(mainGrid, inAction.y, inAction.x, -currBotColor,
                                true)) {
              cout << "user: " << '(' << inAction.x << ',' << inAction.y << ')'
                   << endl;
              break;
            }
          }
        }
      } else {
        cout << "You have no right to go, so AI go." << endl;
      }
      if (!Rights(mainGrid, 1) && !Rights(mainGrid, -1)) {
        Sleep(500);
        GameOver();
        RankSave(mainGrid);
        cout << "rank save successfully!" << endl;
        DisplayRank();
        break;
      }
      if (Rights(mainGrid, currBotColor)) {
        Move oneStep = Decision(3);
        if (ValidityReverse(mainGrid, oneStep.x, oneStep.y, currBotColor,
                            true) &&
            oneStep.x != -1) {
          cout << "AI: " << '(' << oneStep.x << ',' << oneStep.y << ')' << endl;
        } else {
          cout << "AI has no right to go, so you go." << endl;
        }
      }
    }
  } else {
    // AI先手
    while (true) {
      if (Rights(mainGrid, currBotColor)) {
        Move oneStep = Decision(3);
        if (ValidityReverse(mainGrid, oneStep.x, oneStep.y, currBotColor,
                            true) &&
            oneStep.x != -1) {
          cout << "AI: " << '(' << oneStep.x << ',' << oneStep.y << ')' << endl;
        }
      } else {
        cout << "AI has no right to go, so you go." << endl;
      }
      if (!Rights(mainGrid, 1) && !Rights(mainGrid, -1)) {
        Sleep(500);
        GameOver();
        RankSave(mainGrid);
        cout << "rank save successfully!" << endl;
        DisplayRank();
        break;
      }
      GridCopy(mainGrid, displayGrid, -currBotColor);
      DisplayGrid();
      if (Rights(mainGrid, -currBotColor)) {
        while (true) {
          if (InteractionGameIn()) {
            if (ValidityReverse(mainGrid, inAction.y, inAction.x, -currBotColor,
                                true)) {
              cout << "user: " << '(' << inAction.x << ',' << inAction.y << ')'
                   << endl;
              break;
            }
          }
        }
      } else {
        cout << "You have no right to go, so AI go." << endl;
      }
    }
  }
}

int InteractionMenu() {
  // 主菜单交互
  int x{0}, y{0};
  while (true) {
    getmessage(&interaction, EX_MOUSE);
    if (interaction.lbutton) {
      x = interaction.x;
      y = interaction.y;
      // Start New Game
      if ((x - 180) * (x - 180) + (y - 450) * (y - 450) < 4900) {
        Initialization(mainGrid);
        DisplayStartMenu();
        currBotColor = -InteractionStartMenu();
        Play(1);
        return 1;
        // Load Game
      } else if ((x - 360) * (x - 360) + (y - 450) * (y - 450) < 4900) {
        DisplayGameLoad();
        InteractionLoad();
        return 1;
        // Rank
      } else if ((x - 540) * (x - 540) + (y - 450) * (y - 450) < 4900) {
        DisplayRank();
        return 1;
        // Quit
      } else if ((x - 720) * (x - 720) + (y - 450) * (y - 450) < 4900) {
        closegraph();
        return 0;
      }
    }
  }
}
bool InteractionGameIn() {
  // 对局内交互
  getmessage(&interaction, EX_MOUSE);
  int x{0}, y{0};
  if (interaction.lbutton) {
    x = interaction.x;
    y = interaction.y;
    if ((x - 787.5) * (x - 787.5) + (y - 440) * (y - 440) < 2500) {
      // Save
      RoundSave(mainGrid);
      cout << "save successfully !" << endl;
      return true;
    } else if ((x - 787.5) * (x - 787.5) + (y - 550) * (y - 550) < 2500) {
      // Quit
      DisplayMenu();
      InteractionMenu();
      return true;
    } else if (70 < x && x < 630 && 70 < y && y < 630) {
      // 鼠标点击操作在棋盘范围内
      x = (x - 70) / 70;
      y = (y - 70) / 70;
      if (x != inAction.x || y != inAction.y) { // 修复点击失效的错误
        inAction.x = x;
        inAction.y = y;
        return true;
      }
      return false;
    }
  }
  return false;
}
int main() {
  DisplayPreparation();
  DisplayMenu();
  InteractionMenu();
  return 0;
}
