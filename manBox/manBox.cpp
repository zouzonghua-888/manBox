#include "manBox.h"


using namespace std;

#define isValid(pos)  pos.x >= 0 && pos.x < LINE  &&\
				      pos.y > 0 && pos.y < COLUMN


void init_graph(IMAGE& image) {
	initgraph(SCREEN_WIDTH, SCREEN_HIGHT);
	loadimage(&image, _T("blackground.bmp"));
	putimage(0, 0, &image);

	//加载图片资源
	loadimage(&images[WALL], _T("wall_right.bmp"));
	loadimage(&images[FLOOR], _T("floor.bmp"));
	loadimage(&images[BOX_DES], _T("des.bmp"));
	loadimage(&images[MAN], _T("man.bmp"));
	loadimage(&images[BOX], _T("box.bmp"));
	loadimage(&images[BOX_HIT], _T("box.bmp"));
}

void gameCotrol(DIRECTION direction)
{
	//int x = man.x;
	//int y = man.y;
	pos next_pos = man;
	pos next_next_pos = man; //箱子

	switch(direction){
	case UP:
		next_pos.x--;
		next_next_pos.x -= 2;
		break;
	case DOWN:
		next_pos.x++;
		next_next_pos.x += 2;
		break;
	case LEFT:
		next_pos.y--;
		next_next_pos.y -= 2;
		break;
	case RIGHT:
		next_pos.y++;
		next_next_pos.y += 2;
		break;
	default:
		break;

	}

	if( isValid(next_pos) && map[next_pos.x][next_pos.y] == FLOOR){ //人的前方是地板
		changeMap(&next_pos,MAN);
		changeMap(&man,FLOOR);
		man = next_pos;		
	}
	else if(isValid(next_next_pos) && map[next_pos.x][next_pos.y] == BOX){//人的前方是箱子
		if(map[next_next_pos.x][next_next_pos.y] == FLOOR){ //箱子前面是地板
			changeMap(&next_next_pos,BOX);
			changeMap(&next_pos,MAN);
			changeMap(&man,FLOOR);
			man = next_pos;
		}
		else if(map[next_next_pos.x][next_next_pos.y] == BOX_DES){ //箱子前面是目的地
			changeMap(&next_next_pos,BOX_HIT);
			changeMap(&next_pos,MAN);
			changeMap(&man,FLOOR);
			man = next_pos;
		}
	}

	/* 被上面代码优化
	switch(direction){
		case UP:
			if((x - 1) > 0 && map[x-1][y] == FLOOR){
				changeMap(x-1,y,MAN);
				man.x = x - 1;
				changeMap(x,y,FLOOR);
			}
			break;
		case DOWN:
			if((x + 1) < LINE && map[x + 1][y] == FLOOR){
				changeMap(x+1, y,MAN);
				man.x = x + 1;
				changeMap(x,y,FLOOR);
			}
			break;
		case LEFT:
			if((y - 1) > 0 && map[x][y-1] == FLOOR){
				changeMap(x,y-1, MAN);
				man.y = y - 1;
				changeMap(x,y,FLOOR);
			}
			break;
		case RIGHT:
			if((y + 1) > 0 && map[x][y+1] == FLOOR){
				changeMap(x,y+1, MAN);
				man.y = y + 1;
				changeMap(x,y,FLOOR);
			}
			break;
		default:
			break;
	}*/

}

void changeMap(pos* pos1,IMAGESOURCE image){
	 map[pos1->x][pos1->y] = image;
	putimage(START_X + pos1->y * ROATE,START_Y + pos1->x * ROATE,&images[image]);
}

bool  isGameOver(levelInfo& level){
	for(int i = 0; i < level.row; i++){
		for(int j = 0; j < level.column; j++){
			if(map[i][j] ==  BOX_DES ){
				return false;
			}
		}
	}
	return true;
}

void gameNextLevel(IMAGE* image) {
	putimage(0, 0, image);
	settextcolor(WHITE);
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HIGHT };
	settextstyle(20, 0, _T("宋体"));
	drawtext(_T("恭喜您挑战成功，按任意键进入下一关卡！"), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	::system("pause");
	cleardevice();
}

void gameOver(IMAGE* image){
	putimage(0,0,image);
	settextcolor(WHITE);
	RECT rect ={0,0,SCREEN_WIDTH,SCREEN_HIGHT};
	settextstyle(20,0,_T("宋体"));
	drawtext(_T("恭喜您成为搬箱子老司机!"), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

bool login(userInfo& user) {
	//登录次数限定
	int times = 0;//记录登录次数
	bool ret = false;

	do {
		cout << "请输入用户名：";
		cin >> user.userName;
		cout << "请输入密码：";
		cin >> user.passWord;

		ret = fetchUserInfo(user);
		times++;
		
		if (times >= MAX_LOGIN_TIMES) {
			break;
		}
		if (ret == false) {
			cout << "用户名或密码错误，请重新输入!" << endl;
		}		
	} while (!ret);

	return ret;
}

int main(void){
	 IMAGE image;
	//输入用户登录信息
	userInfo user;
	levelInfo level;
	bool quit = false;

	//登录服务器验证
	if (login(user) == false) {
		cout << "登录失败，请重新登录！\n" << endl;
		exit(-1);
	}
	else {
		cout << "登录成功，用户id : " << user.id << "您当前的关卡是：" << user.level_id 
				<<"请开始你的表演！" << endl;
		::system("pause");
	}

	//加载图片资源
	init_graph(image);

	//进入游戏
	do {
		//从数据库获取关卡信息
		bool ret = fetchLevelInfo(level, user.level_id);
		if (!ret) {
			cout << "获取关卡信息失败，请重试！" << endl;
			::system("pause");
			exit(-1);
		}

		//解析数据库地图数据
		transform_map_db2array(level, map);

		for (int i = 0; i < level.row; i++) {
			for (int j = 0; j < level.column; j++) {
				if (map[i][j] == MAN) {  //找出小人的位置
					man.x = i;
					man.y = j;
				}
				putimage(START_X + ROATE * j, START_Y + ROATE * i, &images[map[i][j]]);
			}
		}

		//热键处理		
		do {
			if (_kbhit()) {
				char ch = _getch();

				switch (ch) {
				case KEY_UP:
					gameCotrol(UP);
					break;
				case KEY_DOWN:
					gameCotrol(DOWN);
					break;
				case KEY_LEFT:
					gameCotrol(LEFT);
					break;
				case KEY_RIGHT:
					gameCotrol(RIGHT);
					break;
				case KEY_QUIT:
					quit = true;
					break;
				default:
					break;
				}

				if (isGameOver(level)) {
					if (level.nextLevelID < 1) {
						gameOver(&image);
						quit = true;
						break;
					}

					gameNextLevel(&image);

					//修改数据库玩家关卡ID
					if (updateUserLevel(user, level.nextLevelID)) {
						user.level_id = level.nextLevelID;	
					}
					
					//updateUserLevel(user, level.nextLevelID);
					break;

					//gameOver(&image);
					//quit = true;
				}
			}

			Sleep(100);
		} while (!quit);

	} while (!quit);

	system("pause");

	closegraph();
	return 0;
}