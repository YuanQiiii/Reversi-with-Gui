#include <algorithm>
#include <conio.h>
#include <fstream>
#include <graphics.h>
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>
using namespace std;
#define M 8
int ChessBoard[M][M]{};
int DisplayBoard[M][M]{};
int ChessBoard_AI[M][M]{};
struct PossibleChoice {
  int x, y, points;
};
int In_X{}, In_Y{};
int Game_X{}, Game_Y{};
bool Your_Color = true, First_Color = true;
ExMessage interaction;
int blacknumber, whitenumber;
IMAGE image;
void Interaction_Memu();
void Interaction_GameLoad();
void Game_RankBoard();

// 棋盘开局初始化
void ChessBoard_Initialization() {
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      ChessBoard[i][j] = 0;
    }
  }
  ChessBoard[3][3] = -1;
  ChessBoard[4][4] = -1;
  ChessBoard[3][4] = 1;
  ChessBoard[4][3] = 1;
}
// 落子判定
bool Play_Chess_Validity_Check_Up(int x, int y, bool color) {
  int opponentColor = color ? -1 : 1;
  int i = y - 1;
  while (i >= 0 && ChessBoard[i][x] == opponentColor) {
    i--;
  }
  if (i >= 0 && ChessBoard[i][x] == (color ? 1 : -1) && i != y - 1) {
    return true;
  }
  return false;
}

bool Play_Chess_Validity_Check_Down(int x, int y, bool color) {
  int opponentColor = color ? -1 : 1;
  int i = y + 1;
  while (i < M && ChessBoard[i][x] == opponentColor) {
    i++;
  }
  if (i < M && ChessBoard[i][x] == (color ? 1 : -1) && i != y + 1) {
    return true;
  }
  return false;
}

bool Play_Chess_Validity_Check_Left(int x, int y, bool color) {
  int opponentColor = color ? -1 : 1;
  int i = x - 1;
  while (i >= 0 && ChessBoard[y][i] == opponentColor) {
    i--;
  }
  if (i >= 0 && ChessBoard[y][i] == (color ? 1 : -1) && i != x - 1) {
    return true;
  }
  return false;
}

bool Play_Chess_Validity_Check_Right(int x, int y, bool color) {
  int opponentColor = color ? -1 : 1;
  int i = x + 1;
  while (i < M && ChessBoard[y][i] == opponentColor) {
    i++;
  }
  if (i < M && ChessBoard[y][i] == (color ? 1 : -1) && i != x + 1) {
    return true;
  }
  return false;
}

bool Play_Chess_Validity_Check_RightUp(int x, int y, bool color) {
  int opponentColor = color ? -1 : 1;
  int i = x + 1, j = y - 1;
  while (i < M && j >= 0 && ChessBoard[j][i] == opponentColor) {
    i++;
    j--;
  }
  if (i < M && j >= 0 && ChessBoard[j][i] == (color ? 1 : -1) &&
      !(i == x + 1 && j == y - 1)) {
    return true;
  }
  return false;
}

bool Play_Chess_Validity_Check_LeftUp(int x, int y, bool color) {
  int opponentColor = color ? -1 : 1;
  int i = x - 1, j = y - 1;
  while (i >= 0 && j >= 0 && ChessBoard[j][i] == opponentColor) {
    i--;
    j--;
  }
  if (i >= 0 && j >= 0 && ChessBoard[j][i] == (color ? 1 : -1) &&
      !(i == x - 1 && j == y - 1)) {
    return true;
  }
  return false;
}

bool Play_Chess_Validity_Check_RightDown(int x, int y, bool color) {
  int opponentColor = color ? -1 : 1;
  int i = x + 1, j = y + 1;
  while (i < M && j < M && ChessBoard[j][i] == opponentColor) {
    i++;
    j++;
  }
  if (i < M && j < M && ChessBoard[j][i] == (color ? 1 : -1) &&
      !(i == x + 1 && j == y + 1)) {
    return true;
  }
  return false;
}

bool Play_Chess_Validity_Check_Leftdown(int x, int y, bool color) {
  int opponentColor = color ? -1 : 1;
  int i = x - 1, j = y + 1;
  while (i >= 0 && j < M && ChessBoard[j][i] == opponentColor) {
    i--;
    j++;
  }
  if (i >= 0 && j < M && ChessBoard[j][i] == (color ? 1 : -1) &&
      !(i == x - 1 && j == y + 1)) {
    return true;
  }
  return false;
}
// 落子判定集合
bool Play_Chess_Validity_Check(int x, int y, bool color) {
  if (ChessBoard[y][x] == 0) {
    if (Play_Chess_Validity_Check_Up(x, y, color) ||
        Play_Chess_Validity_Check_Down(x, y, color) ||
        Play_Chess_Validity_Check_Left(x, y, color) ||
        Play_Chess_Validity_Check_Right(x, y, color) ||
        Play_Chess_Validity_Check_RightUp(x, y, color) ||
        Play_Chess_Validity_Check_LeftUp(x, y, color) ||
        Play_Chess_Validity_Check_RightDown(x, y, color) ||
        Play_Chess_Validity_Check_Leftdown(x, y, color)) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}
// 棋盘变化集合
void ChessBoard_Transformation(int x, int y, bool color) {
  if (Play_Chess_Validity_Check_Up(x, y, color)) {
    int opponentColor = color ? -1 : 1;
    int i = y - 1;
    while (i >= 0 && ChessBoard[i][x] == opponentColor) {
      ChessBoard[i][x] = color ? 1 : -1;
      i--;
    }
  }
  if (Play_Chess_Validity_Check_Down(x, y, color)) {
    int opponentColor = color ? -1 : 1;
    int i = y + 1;
    while (i < M && ChessBoard[i][x] == opponentColor) {
      ChessBoard[i][x] = color ? 1 : -1;
      i++;
    }
  }
  if (Play_Chess_Validity_Check_Left(x, y, color)) {
    int opponentColor = color ? -1 : 1;
    int i = x - 1;
    while (i >= 0 && ChessBoard[y][i] == opponentColor) {
      ChessBoard[y][i] = color ? 1 : -1;
      i--;
    }
  }
  if (Play_Chess_Validity_Check_Right(x, y, color)) {
    int opponentColor = color ? -1 : 1;
    int i = x + 1;
    while (i < M && ChessBoard[y][i] == opponentColor) {
      ChessBoard[y][i] = color ? 1 : -1;
      i++;
    }
  }
  if (Play_Chess_Validity_Check_RightUp(x, y, color)) {
    int opponentColor = color ? -1 : 1;
    int i = x + 1, j = y - 1;
    while (i < M && j >= 0 && ChessBoard[j][i] == opponentColor) {
      ChessBoard[j][i] = color ? 1 : -1;
      i++;
      j--;
    }
  }
  if (Play_Chess_Validity_Check_Leftdown(x, y, color)) {
    int opponentColor = color ? -1 : 1;
    int i = x - 1, j = y + 1;
    while (i >= 0 && j < M && ChessBoard[j][i] == opponentColor) {
      ChessBoard[j][i] = color ? 1 : -1;
      i--;
      j++;
    }
  }
  if (Play_Chess_Validity_Check_LeftUp(x, y, color)) {
    int opponentColor = color ? -1 : 1;
    int i = x - 1, j = y - 1;
    while (i >= 0 && j >= 0 && ChessBoard[j][i] == opponentColor) {
      ChessBoard[j][i] = color ? 1 : -1;
      i--;
      j--;
    }
  }
  if (Play_Chess_Validity_Check_RightDown(x, y, color)) {
    int opponentColor = color ? -1 : 1;
    int i = x + 1, j = y + 1;
    while (i < M && j < M && ChessBoard[j][i] == opponentColor) {
      ChessBoard[j][i] = color ? 1 : -1;
      i++;
      j++;
    }
  }
}
// 落子函数
bool Play_Chess(int x, int y, bool color) {
  if (ChessBoard[y][x] == 0 && Play_Chess_Validity_Check(x, y, color)) {
    ChessBoard[y][x] = color ? 1 : -1;
    ChessBoard_Transformation(x, y, color);
    return true;
  } else {
    return false;
  }
}
// 下棋权力判定
bool Play_Chess_Right(bool color) {
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      if (Play_Chess_Validity_Check(j, i, color)) {
        return true;
      }
    }
  }
  return false;
}
// 一方棋子数目统计
int ChessBoard_Statistics(bool color) {
  int sum = 0;
  int targetcolor = color ? 1 : -1;
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      if (ChessBoard[i][j] == targetcolor) {
        sum++;
      }
    }
  }
  return sum;
}
// 将当前棋盘转移到要显示的棋盘上（包括提示符）
void ChessBoard_Copy() {
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      if (ChessBoard[i][j] == 1) {
        DisplayBoard[i][j] = 1; // black
      } else if (ChessBoard[i][j] == -1) {
        DisplayBoard[i][j] = -1; // white
      } else {
        if (Play_Chess_Validity_Check(j, i, true) && Your_Color) {
          DisplayBoard[i][j] = 11; // black choice
        } else if (Play_Chess_Validity_Check(j, i, false) && !Your_Color) {
          DisplayBoard[i][j] = -11; // white choice
        } else {
          DisplayBoard[i][j] = 0; // blank
        }
      }
    }
  }
}
// 对局结果保存
void Rank_Save() {
  stringstream temp;
  temp << ChessBoard_Statistics(true);
  temp << ' ';
  temp << ChessBoard_Statistics(false);
  fstream outfile;
  outfile.open("Rank.txt", ios::app);
  outfile << temp.str() << endl;
  outfile.close();
}
// 存档函数
void ChessBoard_Save() {
  stringstream temp;
  int sum = ChessBoard_Statistics(true) + ChessBoard_Statistics(false);
  temp << sum - 4;
  temp << ' ';
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      if (ChessBoard[i][j] == 1) {
        temp << 'B';
      } else if (ChessBoard[i][j] == -1) {
        temp << 'W';
      } else {
        temp << 'S';
      }
    }
  }
  fstream outfile;
  outfile.open("ChessBoard.txt", ios::app);
  outfile << temp.str() << endl;
  outfile.close();
}
// 读档函数（只接受0，1，2作为参数）,倒序排列，最新的存档为第一个，不会覆盖（依次次延后）
int ChessBoard_Load(int step) {
  string temp_s[1000];
  int temp_s_i = 0;
  string number_s;
  string chessboard_s;
  fstream infile;
  infile.open("ChessBoard.txt", ios::in);
  while (getline(infile, temp_s[temp_s_i])) {
    temp_s_i++;
  }
  cout << "file number:" << temp_s_i << endl;
  if (step + 1 > temp_s_i) {
    return -1;
  } else {
    infile.close();
    istringstream is(temp_s[temp_s_i - step - 1]);
    is >> number_s;
    is >> chessboard_s;
    int number = stoi(number_s);
    for (int i = 0; i < M; i++) {
      for (int j = i * M; j < (i + 1) * M; j++) {
        if (chessboard_s[j] == 'B')
          ChessBoard[i][j % 8] = 1;
        else if (chessboard_s[j] == 'W') {
          ChessBoard[i][j % 8] = -1;
        } else if (chessboard_s[j] == 'S') {
          ChessBoard[i][j % 8] = 0;
        }
      }
    }
    if (number % 2 == 0) {
      return 0;
    } else {
      return 1;
    }
  }
}
// 将图形化界面得到的操作转化为点(与之前不一样的点)
bool Play_Chess_TP() {
  if (70 < In_X && In_X < 630 && 70 < In_Y && In_Y < 630) {
    int a, b;
    a = (int)(In_X - 70) / 70;
    b = (int)(In_Y - 70) / 70;
    if (a != Game_X || b != Game_Y) {
      Game_X = a;
      Game_Y = b;
      return true;
    }
    return false;
  } else {
    return false;
  }
}
// 在推演前先保存棋盘
void ChessBoard_Save_AI() {
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      ChessBoard_AI[i][j] = ChessBoard[i][j];
    }
  }
}
// 在得出操作之后还原棋盘
void ChessBoard_Load_AI() {
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      ChessBoard[i][j] = ChessBoard_AI[i][j];
    }
  }
}
// 自定义比较函数
bool PossibleChoice_cmp(PossibleChoice a, PossibleChoice b) {
  return a.points > b.points;
}
// ai下棋函数
void AI_Chess_Play(bool color) {
  int pointsmap[M][M] = {
      {100, -5, 10, 10, 10, 10, -5, 100}, {-5, -100, -5, -5, -5, -100, -5},
      {10, -5, 0, 0, 0, 0, -5, 10},       {10, -5, 0, 0, 0, 0, -5, 10},
      {10, -5, 0, 0, 0, 0, -5, 10},       {10, -5, 0, 0, 0, 0, -5, 10},
      {-5, -100, -5, -5, -5, -100, -5},   {100, -5, 10, 10, 10, 10, -5, 100}};
  PossibleChoice possiblechoices[M * M];
  int possiblechoice_i = 0;
  int points = 0;
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      if (ChessBoard[i][j] == 0) {
        if (Play_Chess_Validity_Check(j, i, color)) {
          points = ChessBoard_Statistics(color);
          possiblechoices[possiblechoice_i].x = j;
          possiblechoices[possiblechoice_i].y = i;
          ChessBoard_Save_AI();
          Play_Chess(j, i, color);
          ChessBoard_Transformation(j, i, color);
          possiblechoices[possiblechoice_i].points =
              ChessBoard_Statistics(color) - points + pointsmap[j][i];
          ChessBoard_Load_AI();
          possiblechoice_i++;
        }
      }
    }
  }
  sort(possiblechoices, possiblechoices + possiblechoice_i, PossibleChoice_cmp);
  Play_Chess(possiblechoices[0].x, possiblechoices[0].y, color);
  ChessBoard_Transformation(possiblechoices[0].x, possiblechoices[0].y, color);
}
// 初始化
void Display_Preparation() { initgraph(900, 700); }
// 主菜单界面
void Display_Memu() {
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
  drawtext(_T("Reversi"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  settextstyle(20, 0, _T("Consolas"));
  r = {90, 440, 270, 460};
  drawtext(_T("Start New Game"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  r = {270, 440, 450, 460};
  drawtext(_T("Load Game"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  r = {450, 440, 630, 460};
  drawtext(_T("Rank"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  r = {630, 440, 810, 460};
  drawtext(_T("Quit"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  r = {200, 800, 700, 850};
  drawtext(_T("插图作者：幽糖"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
// 在每一次刷新棋盘之前的生成空游戏界面（最底下的一个图层）
void Display_Clear() {
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
  solidcircle(787.5, 440, 50); //
  solidcircle(787.5, 550, 50); //
  setfillcolor(RGB(83, 82, 78));
  solidcircle(787.5, 120, 30);
  setfillcolor(RGB(229, 227, 221));
  solidcircle(787.5, 180, 30);
  setbkmode(TRANSPARENT);
  settextcolor(WHITE);
  settextstyle(25, 0, _T("Consolas"));
  RECT r = {740, 420, 840, 460}; // int left,int top,int right,int bottom
  drawtext(_T("Save"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  r = {740, 530, 840, 570};
  drawtext(_T("Quit"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  EndBatchDraw();
}
// 画出棋子
void DisplayBoard_Draw(int x, int y, bool color) {
  int temp_x, temp_y;
  temp_x = 105 + x * 70;
  temp_y = 105 + y * 70;
  if (color) {
    setfillcolor(RGB(83, 82, 78));
    solidcircle(temp_x, temp_y, 30);
  } else {
    setfillcolor(RGB(229, 227, 221));
    solidcircle(temp_x, temp_y, 30);
  }
}
// 嘲讽
void Expressions() {
  if (Your_Color) {
    if (blacknumber > whitenumber) {
      loadimage(&image, _T("sad.ico"));
      putimage(723, 247, &image);
    } else if (blacknumber < whitenumber) {
      loadimage(&image, _T("scornful.ico"));
      putimage(723, 247, &image);
    } else if (blacknumber == whitenumber) {
      loadimage(&image, _T("careful.ico"));
      putimage(723, 247, &image);
    }
  } else {
    if (blacknumber < whitenumber) {
      loadimage(&image, _T("sad.ico"));
      putimage(723, 247, &image);
    } else if (blacknumber > whitenumber) {
      loadimage(&image, _T("scornful.ico"));
      putimage(723, 247, &image);
    } else if (blacknumber == whitenumber) {
      loadimage(&image, _T("careful.ico"));
      putimage(723, 247, &image);
    }
  }
}
// 调试函数1
void Display_DisplayBoard_S() {
  cout << "-------------------displayboard----------------------" << endl;
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      switch (DisplayBoard[i][j]) {
      case 1:
        cout << "   1 ";
        break;
      case -1:
        cout << "  -1 ";
        break;
      case 11:
        cout << "  11 ";
        break;
      case -11:
        cout << " -11 ";
        break;
      case 0:
        cout << "  0  ";
        break;
      }
    }
    cout << endl;
  }
}
// 调试函数2
void Display_ChessBoard_S() {
  cout << "-------------------chessboard----------------------" << endl;
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      switch (ChessBoard[i][j]) {
      case 1:
        cout << " 1 ";
        break;
      case -1:
        cout << "-1 ";
        break;
      case 0:
        cout << " 0 ";
        break;
      }
    }
    cout << endl;
  }
}

// 绘制对局棋盘及相关信息（接受的bool变量为当前下棋方的颜色）
void Display_DisplayBoard(bool color) {
  blacknumber = 0;
  whitenumber = 0;
  int temp_x, temp_y;
  BeginBatchDraw();
  Display_Clear();
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < M; j++) {
      switch (DisplayBoard[i][j]) {
      case 1:
        DisplayBoard_Draw(j, i, true);
        blacknumber++;
        break;
      case -1:
        DisplayBoard_Draw(j, i, false);
        whitenumber++;
        break;
      case 11:
        if (color) {
          temp_x = 105 + j * 70;
          temp_y = 105 + i * 70;
          setfillcolor(RGB(196, 122, 155));
          solidcircle(temp_x, temp_y, 10);
          break;
        }
        break;
      case -11:
        if (!color) {
          temp_x = 105 + j * 70;
          temp_y = 105 + i * 70;
          setfillcolor(RGB(196, 122, 155));
          solidcircle(temp_x, temp_y, 10);
          break;
        }
        break;
      }
    }
  }

  // 显示双方棋子数
  settextstyle(20, 0, _T("Consolas"));
  setbkmode(TRANSPARENT);
  settextcolor(WHITE);
  TCHAR w[5];
  _stprintf_s(w, _T("%d"), blacknumber);
  outtextxy(780, 110, w);
  settextcolor(BLACK);
  TCHAR b[5];
  _stprintf_s(b, _T("%d"), whitenumber);
  outtextxy(780, 175, b);
  Expressions();
  EndBatchDraw();
}
// 在游戏中获取用户操作
bool Interaction_Game() {
  getmessage(&interaction, EX_MOUSE);
  int x, y;
  if (interaction.lbutton) {
    x = interaction.x;
    y = interaction.y;
    // 存档
    if ((x - 787.5) * (x - 787.5) + (y - 440) * (y - 440) < 2500) {
      ChessBoard_Save();
      cout << "save successfully!!" << endl;
      return true;
    }
    // 返回主界面
    else if ((x - 787.5) * (x - 787.5) + (y - 550) * (y - 550) < 2500) {
      Display_Memu();
      Interaction_Memu();
      return true;
    }
    // 当前操作是一个在棋盘上的操作
    else if (70 < x && x < 630 && 70 < y && y < 630) {
      In_X = x;
      In_Y = y;
      int a, b;
      a = (int)(In_X - 70) / 70;
      b = (int)(In_Y - 70) / 70;
      if (a != Game_X || b != Game_Y) {
        Game_X = a;
        Game_Y = b;
        return true;
      }
      return false;
    }
  }
}

// 选择你的颜色界面
void Display_Start_Memu() {
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
// 选择你的颜色交互
bool Interaction_Start_Memu() {
  int x, y;
  while (true) {
    getmessage(&interaction, EX_MOUSE);
    if (interaction.lbutton) {
      x = interaction.x;
      y = interaction.y;
      if ((x - 300) * (x - 300) + (y - 450) * (y - 450) < 10000) {
        return true;
      } else if ((x - 600) * (x - 600) + (y - 450) * (y - 450) < 10000) {
        return false;
      }
    }
  }
}
// 显示天梯榜（两秒之后回到主界面）
void Game_RankBoard() {
  int h = 100;
  string temp_s[1000];
  int temp_s_i = 0;
  fstream infile;
  infile.open("Rank.txt", ios::in);
  while (getline(infile, temp_s[temp_s_i])) {
    temp_s_i++;
  }
  setbkcolor(RGB(225, 218, 211));
  cleardevice();
  setbkmode(TRANSPARENT);
  settextcolor(BLACK);
  int i = 0;
  while (h < 690 && i < temp_s_i) {
    istringstream is(temp_s[i]);
    is >> blacknumber;
    is >> whitenumber;
    settextstyle(20, 0, _T("Consolas"));
    TCHAR w[1000];
    _stprintf_s(w,
                _T("%d - - - - - - - - - - - - - - - - - - - - - - - - - - - ")
                _T("- - BLACK %d / WHITE %d"),
                i + 1, blacknumber, whitenumber);
    outtextxy(100, h, w);
    h += 20;
    i++;
  }
  Sleep(2000);
  Display_Memu();
  Interaction_Memu();
}
// 显示结算画面（两秒之后回到主界面）
void GameOver() {
  setbkcolor(RGB(225, 218, 211));
  cleardevice();
  setbkmode(TRANSPARENT);
  settextstyle(50, 0, _T("Consolas"));
  RECT r = {200, 100, 700, 200};
  if (ChessBoard_Statistics(Your_Color) > ChessBoard_Statistics(!Your_Color)) {
    drawtext(_T("You Win!!!"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  } else if (ChessBoard_Statistics(Your_Color) <
             ChessBoard_Statistics(!Your_Color)) {
    drawtext(_T("You Lost!!!"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  } else if (ChessBoard_Statistics(Your_Color) ==
             ChessBoard_Statistics(!Your_Color)) {
    drawtext(_T("All Even!!!"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  }
  Sleep(2000);
}
// 显示读档界面
void GameLoad() {
  setbkcolor(RGB(225, 218, 211));
  cleardevice();
  setfillcolor(RGB(133, 133, 140));
  solidrectangle(200, 100, 700, 200); // int left,int top,int right,int bottom
  solidrectangle(200, 300, 700, 400);
  solidrectangle(200, 500, 700, 600);
  setbkmode(TRANSPARENT);
  settextstyle(50, 0, _T("Consolas"));
  RECT r = {200, 100, 700, 200};
  drawtext(_T(" file 1"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  r = {200, 300, 700, 400};
  drawtext(_T("file 2"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
  r = {200, 500, 700, 600};
  drawtext(_T("file 3"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// 下棋对局函数，接受变量为当前先手（bool）
void Play(bool color) {
  if (color == Your_Color) {
    while (true) {
      ChessBoard_Copy();
      Display_DisplayBoard(Your_Color);
      // Display_DisplayBoard_S();
      // Display_ChessBoard_S();
      if (Play_Chess_Right(Your_Color)) {
        while (true) {
          Interaction_Game();
          Play_Chess_TP();
          if (Play_Chess(Game_X, Game_Y, Your_Color)) {
            break;
          }
        }
      } else {
        cout << "You have no right to go, so AI go." << endl;
      }

      if (Play_Chess_Right(!Your_Color)) {
        AI_Chess_Play(!Your_Color);
      } else {
        cout << "AI has no right to go, so you go." << endl;
      }
      if (!Play_Chess_Right(!Your_Color) && !Play_Chess_Right(Your_Color)) {
        GameOver();
        Rank_Save();
        cout << "rank save successfully!" << endl;
        Game_RankBoard();
        break;
      }
    }
  } else {
    while (true) {
      if (Play_Chess_Right(!Your_Color)) {
        AI_Chess_Play(!Your_Color);
      } else {
        cout << "AI has no right to go, so you go." << endl;
      }
      ChessBoard_Copy();
      Display_DisplayBoard(Your_Color);
      // Display_DisplayBoard_S();
      // Display_ChessBoard_S();
      if (Play_Chess_Right(Your_Color)) {
        while (true) {
          Interaction_Game();
          Play_Chess_TP();
          if (Play_Chess(Game_X, Game_Y, Your_Color)) {
            break;
          }
        }
      } else {
        cout << "You have no right to go, so AI go." << endl;
      }
      if (!Play_Chess_Right(!Your_Color) && !Play_Chess_Right(Your_Color)) {
        GameOver();
        Rank_Save();
        cout << "rank save successfully!" << endl;
        Game_RankBoard();
        break;
      }
    }
  }
}
// 主界面交互
void Interaction_Memu() {
  int x, y;
  while (true) {
    getmessage(&interaction, EX_MOUSE);
    if (interaction.lbutton) {
      x = interaction.x;
      y = interaction.y;
      // Start New Game
      if ((x - 180) * (x - 180) + (y - 450) * (y - 450) < 4900) {
        ChessBoard_Initialization();
        Display_Start_Memu();
        Your_Color = Interaction_Start_Memu();
        Play(true);
        return;
        // Load Game
      } else if ((x - 360) * (x - 360) + (y - 450) * (y - 450) < 4900) {
        GameLoad();
        Interaction_GameLoad();
        return;
        // Rank
      } else if ((x - 540) * (x - 540) + (y - 450) * (y - 450) < 4900) {
        Game_RankBoard();
        return;
        // Quit
      } else if ((x - 720) * (x - 720) + (y - 450) * (y - 450) < 4900) {
        closegraph();
        return;
      }
    }
  }
}
// 读档界面交互
void Interaction_GameLoad() {
  int x, y;
  while (true) {
    getmessage(&interaction, EX_MOUSE);
    if (interaction.lbutton) {

      x = interaction.x;
      y = interaction.y;
      if (x >= 200 && x <= 700 && y >= 100 && y <= 200) {
        int num = ChessBoard_Load(0);
        if (num == -1) {
          cout << "Please Save Your Game first!!" << endl;
          Display_Memu();
          Interaction_Memu();
        } else {
          if (num == 0) {
            First_Color = true;
            Display_Start_Memu();
            Your_Color = Interaction_Start_Memu();
            Play(First_Color);
          } else if (num == 1) {
            First_Color = false;
            Display_Start_Memu();
            Your_Color = Interaction_Start_Memu();
            Play(First_Color);
          }
        }
        return;
      } else if (x >= 200 && x <= 700 && y >= 300 && y <= 400) {
        int num = ChessBoard_Load(1);
        if (num == -1) {
          cout << "Please Save Your Game first!!" << endl;
          Display_Memu();
          Interaction_Memu();
        } else {
          if (num == 0) {
            First_Color = true;
            Display_Start_Memu();
            Your_Color = Interaction_Start_Memu();
            Play(First_Color);
          } else if (num == 1) {
            First_Color = false;
            Display_Start_Memu();
            Your_Color = Interaction_Start_Memu();
            Play(First_Color);
          }
        }
        return;
      } else if (x >= 200 && x <= 700 && y >= 500 && y <= 600) {
        int num = ChessBoard_Load(2);
        if (num == -1) {
          cout << "Please Save Your Game first!!" << endl;
          Display_Memu();
          Interaction_Memu();
        } else {
          if (num == 0) {
            First_Color = true;
            Display_Start_Memu();
            Your_Color = Interaction_Start_Memu();
            Play(First_Color);
          } else if (num == 1) {
            First_Color = false;
            Display_Start_Memu();
            Your_Color = Interaction_Start_Memu();
            Play(First_Color);
          }
        }
        return;
      }
    }
  }
}
int main() {

  Display_Preparation();
  Display_Memu();
  Interaction_Memu();
  return 0;
}
