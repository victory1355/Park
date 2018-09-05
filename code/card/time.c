#include "head.h"


//	停车场计费标准：3元每个小时

int select_time_result(void *arg,int column,char **column_val,char **column_name)
{
	//printf("two test\n");
	int i;
	char buffer_time[50];
	
	bzero(buffer_time, 50);
	sprintf(buffer_time, "%s", *(column_val));

	//打印调试信息
	printf("\n----------------------------------------- total charge--------------------------------------\n");
	printf("| start_time = %ld  | ", atol(buffer_time));


	time_t end_time;
	time_t start_time = atol(buffer_time);
	time(&end_time);
	printf("end_time = %ld  |   ", end_time);
	
	long int total_time;
	total_time = end_time-start_time;

	printf("total_time:%ld  |   ", total_time);
	
	double charge = calculate_charge_park(total_time);
	printf("charge = %f |\n", charge);
	
	time_t time_record;
	time(&time_record);
	printf("consumed time:%s\n", (ctime(&time_record)));
	printf("\n--------------------------------------------------------------------------------------------\n");

	
	return 0;//一定要写上
}

/*
	获取系统时间，成功返回进车的时间点，失败返回-1
	参数：无
*/
int get_local_time()
{
	time_t start_t;
	
	int ret = time(&start_t);
	if(ret == -1)
	{
		return -1;
	}
	else
		return start_t;

}


/*
	计算停车时间并打印停车费用
	参数：数据库指针
*/

int calculate_total_parking_time(char *cardid, sqlite3 *car_owner_database)
{
    	
	if(cardid == NULL || car_owner_database == NULL)
	{
		return -1;
	}

    char select_information[300];

	bzero(select_information, 300);
	
	sprintf(select_information,"select time from owner_information_table where card_id='%s';",cardid);
	//打印调试信息
	//printf("select_information calculate_charge_park :%s\n", select_information);

    int ret=sqlite3_exec(car_owner_database, select_information, select_time_result,NULL,NULL);
	if(ret != SQLITE_OK)
	{
		printf("calculate_charge_park failed\n");
		
	}
	return 0;
}


/*
	根据停车时间计算停车费用，返回停车费用
	参数：停车总时间
*/

double calculate_charge_park(long int time)
{
	double charge;
	
	bzero(&charge,sizeof(charge));
	charge = (time/3600.0)*3.0;
	
	return charge;
	
}



