#include "head.h"

//查询结果
int select_cardid_result(void *arg,int column,char **column_val,char **column_name)
{
	
	//printf("one test\n");
	flag = 1;
	return 0;//一定要写上
}
/*
     创建数据库，成功返回数据库指针，失败返回NULL
	 参数：数据库名字
*/
sqlite3 *create_car_owner_database(char *databasename)
{
	if(databasename == NULL)
		return NULL;
	
	
	sqlite3 *car_owner_database;
	int ret;
	
	//打开你要操作的数据库文件
	ret=sqlite3_open(databasename,&car_owner_database);
	if(ret!=SQLITE_OK)
	{
		printf("create database failed\n");
		return NULL;
	}
	else
		printf("create database ok\n");
	//执行相关的命令
	ret=sqlite3_exec(car_owner_database,"create table if not exists owner_information_table(card_id text unique,time text);",NULL,NULL,NULL);
	if(ret!=SQLITE_OK)
	{
		printf("create table owner_information_table failed\n");
		return NULL;
	}
	else
		printf("create table ok\n");
	
	return car_owner_database;

}

/*
	查询数据库是否有该卡号，成功返回0，失败返回-1
	参数：数据指针car_owner_database，卡号cardid，
*/
int select_cardid(char *cardid,sqlite3 *car_owner_database)
{

//	使用查询命令的时候一定要使用第三个参数，函数指针
	
	if(cardid == NULL || car_owner_database == NULL)
	{
		return -1;
	}

	char select_information[300];
	
	//bzero(error_msg, 100);
	memset(select_information,0,sizeof(select_information));	
	sprintf(select_information,"select card_id from owner_information_table where card_id='%s';",cardid);
	//打印调试信息
	//printf("select information:%s\n", select_information);
	
	if(strlen(cardid) == 8)
	{
		int ret=sqlite3_exec(car_owner_database,select_information,select_cardid_result,NULL,NULL);
		//printf("flag = %d\n", flag);
		if(flag != 1)
		{
			//printf("the card_id not exist\n");
			card_in(cardid,car_owner_database);
		}
		else
		{
			flag = 0;
			//printf("the card_id has existed\n");
			card_out(cardid, car_owner_database);
		}
	}

	return 0;
}


































