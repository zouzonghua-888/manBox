#ifndef __MANBOX_H_
#define __MANBOX_H_

#include <iostream>
#include <windows.h>
#include <string>
#include <graphics.h>
#include <conio.h>

#include "dataBase.h"

#define SCREEN_WIDTH  960  //��Ϸ���ڵĿ�
#define SCREEN_HIGHT  768 //��Ϸ���ڵĸ�

//#define LINE      9  
//#define COLUMN     12

#define  ROATE  61  //ͼƬ���
#define START_X  150 //ͼƬ�ڴ��ڿ�ʼ��ʾ�ĺ�����
#define START_Y  100  //ͼƬ�ڴ��ڿ�ʼ��ʾ��������

using namespace std;

enum IMAGESOURCE{
	WALL,  //ǽ
	FLOOR, //�ذ�
	BOX_DES, //����Ŀ�ĵ�
	MAN,     //С��
	BOX,  //����
	BOX_HIT, //��������Ŀ��
	ALL
};

#define KEY_UP     'w'
#define KEY_DOWN   's'
#define KEY_LEFT   'a'
#define KEY_RIGHT  'd'
#define KEY_QUIT   'q'

#define MAX_LOGIN_TIMES 4 //��¼��ೢ�Դ���

enum  DIRECTION{
	UP,
	DOWN,
	LEFT,
	RIGHT
};



IMAGE images[ALL] = {0};

//��ͼ����
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
