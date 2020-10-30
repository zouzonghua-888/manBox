#include "dataBase.h"
#include <stdio.h>

static int debug = 1;

/***********************************************
* 
*���ܣ������û�������������ݿ��ȡ�û�ID�͹ؿ�ID
* ������
*		user - �û���Ϣ�ṹ��
* ����ֵ��
*		��ȡ�ɹ�����true ,  ʧ�� false
* 
************************************************/
bool fetchUserInfo(userInfo& user)
{
	MYSQL mysql;
	MYSQL_RES* res;//�������ݿ��ѯ�����
	MYSQL_ROW row; //����ӽ�����л�ȡ������Ϣ
	char sql[256];

	//�������ݿ�
	if (connect_db(mysql) == false) {
		return false;
	}
	//�����û�����ȡ�û�ID����Ϸ�ؿ�
	snprintf(sql, 256, "select id, level_id from users where username='%s' and password=md5('%s');",
		user.userName.c_str(),user.passWord.c_str());

	bool ret = mysql_query(&mysql,sql); //����0����ʾ��ѯ�ɹ�

	if (ret) {
		printf("���ݲ�ѯʧ�ܣ�ʧ��ԭ��:%s\n", mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}

	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if (row == NULL) { //û�в�ѯ����¼
		mysql_free_result(res);
		mysql_close(&mysql);
		return false;
	}

	user.id = atoi(row[0]);
	user.level_id = atoi(row[1]);
	if(debug) printf("�û�ID��%d,�û���ǰ�ؿ�:%d\n",user.id,user.level_id);

	//�ͷŽ����
	mysql_free_result(res);
	//�ر����ݿ�
	mysql_close(&mysql);

	return true;
}

bool connect_db(MYSQL& mysql)
{
	
	//1,��ʼ�����
	mysql_init(&mysql);

	//2�������ַ�����
	mysql_options(&mysql,MYSQL_SET_CHARSET_NAME,"gbk");


	//3,�������ݿ�
	if (mysql_real_connect(&mysql, DB_HONST, DB_USER, DB_USER_PASSWD, DB_NAME, DB_PORT, NULL, 0) == NULL) {
		printf("���ݿ����ӳ�������ԭ��%s\n", mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}
	
	return true;
}

/*************************************************
*
*���ܣ����ݹؿ�ID �������ݿ��ȡ�ؿ�������Ϣ
* ������
*	level - ����ؿ���Ϣ�ṹ��
*	levelID - Ҫ��ȡ��ϸ�ؿ���Ϣ�Ĺؿ�ID
* ����ֵ��
*	��ȡ�ɹ�����true , ʧ��false
* 
**************************************************/
bool fetchLevelInfo(levelInfo& level, int levelID)
{
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	char sql[256];

	//�������ݿ�
	if (connect_db(mysql) == false) {
		return false;
	}

	//���ݹؿ�ID��ȡ�ؿ���Ϣ
	snprintf(sql,256,"select map_name, map_row, map_column, map_data,next_level_id from levels where id=%d;",levelID);
	bool ret = mysql_query(&mysql,sql);
	if (ret) {
		printf("���ݲ�ѯʧ�ܣ�ʧ��ԭ��:%s\n", mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}

	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if (row == NULL) {
		printf("δ��ѯ���ؿ����ݣ�\n");
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

	//�ͷŽ����
	mysql_free_result(res);
	//�ر����ݿ�
	mysql_close(&mysql);

	return true;
}

bool transform_map_db2array(levelInfo& level, int map[LINE][COLUMN])
{
	if (level.row > LINE || level.column > COLUMN) {
		printf("��ͼ���ݳ�������������!\n");
		return false;
	}

	if (level.mapData.length() < 0) {
		printf("û�е�ͼ���ݿɽ�����\n");
		return false;
	}

	int start = 0, end = 0;
	int row = 0, column = 0;
	//������ͼ����
	do {
		
		//�Ƚ���ͼÿһ���н�������
		end = level.mapData.find('|',start);
		if (end < 0) { //���������һ�����ݣ����û���ַ���|��
			end = level.mapData.length();
		}

		if (start >= end) {
			break;
		}

		string line = level.mapData.substr(start,end - start);
		if (debug) printf("%s \n",line.c_str());

		//�����������ݵ�����map		
		char*  next_strtok = NULL;
		char* item = strtok_s((char*)line.c_str(),",",&next_strtok);

		column = 0; //��������һ�к�column��ֵ��ӵ���β
		while (item && column < level.column) {
			printf("%s ",item);
			map[row][column] = atoi(item);
			column++;

			item = strtok_s(NULL,",",&next_strtok);
		}
		
		if (column < level.column) {
			printf("���ݽ���������ֹ!\n");
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

	//�������ݿ�
	if (connect_db(mysql) == false) {
		return false;
	}

	//���ݹؿ�ID��ȡ�ؿ���Ϣ
	snprintf(sql, 256, "update users set level_id=%d where id=%d;",next_level_id,user.id);
	bool ret = mysql_query(&mysql, sql);
	if (ret) {
		printf("���ݸ���ʧ�ܣ�ʧ��ԭ��:%s\n", mysql_error(&mysql));
		mysql_close(&mysql);
		return false;
	}

	mysql_close(&mysql);
	return true;

}
