#ifndef __MANBOX_H_
#define __MANBOX_H_

#include <iostream>
#include <windows.h>
#include <string>
#include <graphics.h>
#include <conio.h>

#include "dataBase.h"

#define SCREEN_WIDTH  960  //游戏窗口的宽
#define SCREEN_HIGHT  768 //游戏窗口的高

//#define LINE      9  
//#define COLUMN     12

#define  ROATE  61  //图片宽度
#define START_X  150 //图片在窗口开始显示的横坐标
#define START_Y  100  //图片在窗口开始显示的纵坐标

using namespace std;

enum IMAGESOURCE{
	WALL,  //墙
	FLOOR, //地板
	BOX_DES, //箱子目的地
	MAN,     //小人
	BOX,  //箱子
	BOX_HIT, //箱子命中目标
	ALL
};

#define KEY_UP     'w'
#define KEY_DOWN   's'
#define KEY_LEFT   'a'
#define KEY_RIGHT  'd'
#define KEY_QUIT   'q'

#define MAX_LOGIN_TIMES 4 //登录最多尝试次数

enum  DIRECTION{
	UP,
	DOWN,
	LEFT,
	RIGHT
};



IMAGE images[ALL] = {0};

//地图数据
int map[LINE][COLUMN] = {0};
/*
int map[LINE][COLUMN] = {
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,0,1,1,1,1,1,1,1,0,0},
	{0,1,4,1,0,2,1,0,2,1,0,0},
	{0,1,0,1,0,1,0,0,1,1,1,0},
	{0,1,0,2,0,1,1,4,1,1,1,0},
	{0,1,1,1,0,3,1,1,1,4,1,0},
	{0,1,2,1,1,4,1,1,1,1,1,0},
	{0,1,0,0,1,0,1,1,0,0,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
};
*/
typedef struct POS{
	int x;
	int y;
}pos;

pos  man;

void init_graph(IMAGE& image);
void gameCotrol(DIRECTION direction);
void changeMap(pos* pos1,IMAGESOURCE image);
bool isGameOver(levelInfo& level);
void gameOver(IMAGE* image);
bool login(userInfo& user);

#endif
