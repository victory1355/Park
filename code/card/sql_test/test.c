#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <termios.h> 
#include <sys/types.h>
#include <sys/select.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include "sqlite3.h"



/*回调函数*/
int show_result(void *arg,int column,char **column_val,char **column_name)
{
	int i;
	printf("test\n");
	
	
	for(i=0; i<column; i++)
	{	
		printf("column_name = %s\n",*(column_name+i));
		printf("column_values = %s\n",*(column_val+i));
	}
	return 0;//一定要写上 ,否则sqlite3_exec调用失败
}
/*
     创建数据库，成功返回数据库指针，失败返回NULL
	 参数：数据库名字
*/



int calculate_total_parking_time(sqlite3 *car_owner_database)
{

    char buffer[100];
	char *p = "123000";
	bzero(buffer, 100);

	sprintf(buffer, "select card_id from owner_information_table where card_id='%s';", p);
	printf("select information :%s\n", buffer);
	
    int ret=sqlite3_exec(car_owner_database, buffer, show_result,NULL,NULL);
	if(ret!=SQLITE_OK)
	{
		
		printf("select card_id = %s failed\n", p);

	}
	else
	{
		printf("select card_id = %s ok\n", p);
		
	}

	return 0;
}


sqlite3 *create_car_owner_database(char *databasename)
{
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
	ret=sqlite3_exec(car_owner_database,"create table if not exists owner_information_table(card_id text unique,time int);",NULL,NULL,NULL);
	if(ret!=SQLITE_OK)
	{
		printf("create table owner_information_table failed\n");
		return NULL;
	}
	else
		printf("create table ok\n");
	
	return car_owner_database;

}


int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Usage:./  /../../databasename\n");
		
		return -1;
	}
	sqlite3 *car_owner_database;
	
	car_owner_database = create_car_owner_database(argv[1]);
	if(car_owner_database == NULL)
	{
		//exit(1);
		return -1;
	}
    int ret=sqlite3_exec(car_owner_database,"insert into owner_information_table values(\"123000\" ,56);",NULL,NULL,NULL);
	if(ret!=SQLITE_OK)
	{
		printf("insert failed\n");
		return -1;
	}
	else
		printf("insert ok\n");
	
	
	
	calculate_total_parking_time(car_owner_database);
	

	return 0;
}






