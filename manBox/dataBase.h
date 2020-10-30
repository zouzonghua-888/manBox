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
	int id;            //�û�ID
	string userName;  //�û���
	string passWord;  //�û���¼����
	int level_id;    //�û���ǰ�ؿ�
}userInfo;

typedef struct _levelInfo {
	int id;                //�ؿ�ID
	string mapName;       //�ؿ�����
	int row;             //��ͼ����
	int column;         //��ͼ����
	string mapData;       //��ͼ����
	int nextLevelID;  //��һ�ؿ�ID
}levelInfo;

bool fetchUserInfo(userInfo& user);
static bool connect_db(MYSQL& mysql);

//���ݹؿ�ID ��ȡ�ؿ�����
bool fetchLevelInfo(levelInfo& level,int levelID);

//�������ݿ��ͼ����
bool transform_map_db2array(levelInfo& level, int map[LINE][COLUMN]);

//������ҹؿ�ID
bool updateUserLevel(userInfo& user, int next_level_id);