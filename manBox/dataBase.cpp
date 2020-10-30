#include "dataBase.h"
#include <stdio.h>

static int debug = 1;

/***********************************************
* 
*功能：根据用户名和密码从数据库获取用户ID和关卡ID
* 参数：
*		user - 用户信息结构体
* 返回值：
*		获取成功返回true ,  失败 false
* 
************************************************/
bool fetchUserInfo(userInfo& user)
{
	MYSQL mysql;
	MYSQL_RES* res;//保存数据库查询结果集
	MYSQL_ROW row; //保存从结果集中获取单行信息
	char sql[256];

	//连接数据库
	if (connect_db(mysql) == false) {
		return false;
	}
	//根据用户名获取用户ID和游戏关卡
	snprintf(sql, 256, "select id, level_id from users where username='%s' and password=md5('%s');",
		user.userName.c_str(),user.passWord.c_str());

	bool ret = mysql_query(&mysql,sql); //返回0，表示查询成功

	if (ret) {
		printf("数据查询失败，失败原因:%s\n", mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}

	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if (row == NULL) { //没有查询到记录
		mysql_free_result(res);
		mysql_close(&mysql);
		return false;
	}

	user.id = atoi(row[0]);
	user.level_id = atoi(row[1]);
	if(debug) printf("用户ID：%d,用户当前关卡:%d\n",user.id,user.level_id);

	//释放结果集
	mysql_free_result(res);
	//关闭数据库
	mysql_close(&mysql);

	return true;
}

bool connect_db(MYSQL& mysql)
{
	
	//1,初始化句柄
	mysql_init(&mysql);

	//2，设置字符编码
	mysql_options(&mysql,MYSQL_SET_CHARSET_NAME,"gbk");


	//3,连接数据库
	if (mysql_real_connect(&mysql, DB_HONST, DB_USER, DB_USER_PASSWD, DB_NAME, DB_PORT, NULL, 0) == NULL) {
		printf("数据库连接出错，错误原因：%s\n", mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}
	
	return true;
}

/*************************************************
*
*功能：根据关卡ID ，从数据库获取关卡数据信息
* 参数：
*	level - 保存关卡信息结构体
*	levelID - 要获取详细关卡信息的关卡ID
* 返回值：
*	获取成功返回true , 失败false
* 
**************************************************/
bool fetchLevelInfo(levelInfo& level, int levelID)
{
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	char sql[256];

	//连接数据库
	if (connect_db(mysql) == false) {
		return false;
	}

	//根据关卡ID获取关卡信息
	snprintf(sql,256,"select map_name, map_row, map_column, map_data,next_level_id from levels where id=%d;",levelID);
	bool ret = mysql_query(&mysql,sql);
	if (ret) {
		printf("数据查询失败，失败原因:%s\n", mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}

	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if (row == NULL) {
		printf("未查询到关卡数据！\n");
		mysql_free_result(res);
		mysql_close(&mysql);
		return false;
	}

	level.id = levelID;
	level.mapName = row[0];
	level.row = atoi(row[1]);
	level.column = atoi(row[2]);
	level.mapData = row[3];
	level.nextLevelID = atoi(row[4]);
	
	if (debug)
		printf("level ID: %d map name: %s  map row: %d  map column: %d map data: %s map nextLeveID: %d \n",
			level.id,level.mapName.c_str(),level.row,level.column,level.mapData.c_str(),level.nextLevelID);

	//释放结果集
	mysql_free_result(res);
	//关闭数据库
	mysql_close(&mysql);

	return true;
}

bool transform_map_db2array(levelInfo& level, int map[LINE][COLUMN])
{
	if (level.row > LINE || level.column > COLUMN) {
		printf("地图数据超大，请重新设置!\n");
		return false;
	}

	if (level.mapData.length() < 0) {
		printf("没有地图数据可解析！\n");
		return false;
	}

	int start = 0, end = 0;
	int row = 0, column = 0;
	//解析地图数据
	do {
		
		//先将地图每一整行解析出来
		end = level.mapData.find('|',start);
		if (end < 0) { //解析到最后一行数据，最后没有字符‘|’
			end = level.mapData.length();
		}

		if (start >= end) {
			break;
		}

		string line = level.mapData.substr(start,end - start);
		if (debug) printf("%s \n",line.c_str());

		//解析单行数据到数组map		
		char*  next_strtok = NULL;
		char* item = strtok_s((char*)line.c_str(),",",&next_strtok);

		column = 0; //解析完上一行后column的值会加到行尾
		while (item && column < level.column) {
			printf("%s ",item);
			map[row][column] = atoi(item);
			column++;

			item = strtok_s(NULL,",",&next_strtok);
		}
		
		if (column < level.column) {
			printf("数据解析出错，终止!\n");
			return false;
		}
		printf("\n");
		row++;

		if (row > level.row) break;

		start = end + 1;
	} while (1);

	return true;
}

bool updateUserLevel(userInfo& user,int next_level_id) {
	MYSQL mysql;
	char sql[256];

	//连接数据库
	if (connect_db(mysql) == false) {
		return false;
	}

	//根据关卡ID获取关卡信息
	snprintf(sql, 256, "update users set level_id=%d where id=%d;",next_level_id,user.id);
	bool ret = mysql_query(&mysql, sql);
	if (ret) {
		printf("数据更新失败，失败原因:%s\n", mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}

	mysql_close(&mysql);
	return true;

}
