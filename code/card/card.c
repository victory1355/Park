#include "head.h"

#define PICTURE_IN "/root/project/park/in.bmp"
#define PICTURE_OUT "/root/project/park/out.bmp"

/*
	处理入库动作，成功返回0，失败返回-1
	参数：要插入的信息，比如卡号
*/
int card_in(char *cardid,sqlite3 *car_owner_database)
{
		
	if(cardid == NULL || car_owner_database == NULL)
	{
		return -1;
	}

	char insert_information[300];
	memset(insert_information,0,sizeof(insert_information));
	
    long int start_time;
	start_time = get_local_time();
	char buffer[30];
	bzero(buffer,30);

	sprintf(buffer,"%ld",start_time);

	sprintf(insert_information,"insert into owner_information_table values(\"%s\", \"%s\");",cardid, buffer);
	//打印调试信息
	//printf("insert information:%s\n", insert_information);
	
	
	int ret=sqlite3_exec(car_owner_database,insert_information,NULL,NULL,NULL);
	if(ret!=SQLITE_OK)
	{
		
		printf("insert action failed\n");
		return -1;
	}
	else
	{
		printf("the car(%s) is coming >------> the park\n", cardid);
		
		//显示车位状态
		show_shapebmp(40,250,98,178,PICTURE_IN);
		return 0;
	}
}


/*
	处理出库动作，成功返回0，失败返回-1
	参数：要删除的信息，比如卡号
*/
int card_out(char *cardid, sqlite3 *car_owner_database)
{
		
	if(cardid == NULL || car_owner_database == NULL)
	{
		return -1;
	}

	char delete_information[300];
	
	memset(delete_information,0,sizeof(delete_information));
	
	//删除之前打印费用和时间
	calculate_total_parking_time(cardid, car_owner_database);
	
	sprintf(delete_information,"delete from owner_information_table where card_id='%s';",cardid);
	//打印调试信息
	//printf("delete information:%s\n", delete_information);
	
	//删除车主信息
	int ret=sqlite3_exec(car_owner_database,delete_information,NULL,NULL,NULL);
	if(ret!=SQLITE_OK)
	{
		
		printf("delete action failed\n");
		return -1;
	}
	else
	{
		printf("the car(%s) is leaving <------< the park\n", cardid);
		//显示车位状态
		//show_shapebmp(40,250,98,178,PICTURE_OUT);
		show_fullbmp(PICTURE_PATH); 
		return 0;
	}
}


//




























