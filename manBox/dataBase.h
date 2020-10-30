#pragma once
#include <mysql.h>
#include <string>

#define DB_HONST        "127.0.0.1"
#define DB_USER         "root"
#define DB_USER_PASSWD  "zzh123456"
#define DB_PORT         3306
#define DB_NAME         "box_man"

#define LINE   48
#define COLUMN 48

using namespace std;

typedef struct _userInfo {
	int id;            //用户ID
	string userName;  //用户名
	string passWord;  //用户登录密码
	int level_id;    //用户当前关卡
}userInfo;

typedef struct _levelInfo {
	int id;                //关卡ID
	string mapName;       //关卡名字
	int row;             //地图行数
	int column;         //地图列数
	string mapData;       //地图数据
	int nextLevelID;  //下一关卡ID
}levelInfo;

bool fetchUserInfo(userInfo& user);
static bool connect_db(MYSQL& mysql);

//根据关卡ID 获取关卡数据
bool fetchLevelInfo(levelInfo& level,int levelID);

//解析数据库地图数据
bool transform_map_db2array(levelInfo& level, int map[LINE][COLUMN]);

//更新玩家关卡ID
bool updateUserLevel(userInfo& user, int next_level_id);