#include "head.h"




int main(void)
{
	int serial_fd;
	flag = 0;
	sqlite3 * car_owner_database = NULL;
	
	//初始化串口
	open_tty(&serial_fd);
	
	//显示停车场背景图
    show_fullbmp(PICTURE_PATH);   
	

	//创建数据库
	car_owner_database = create_car_owner_database(DATABASE_PATH);
	if(car_owner_database == NULL)
	{
		return -1;
	}
    //检测进车或者出车动作
	char *p;
	char *pp = (char *)malloc(9);
	bzero(pp ,9);
	
	while(1)
	{
		if((p = carin_or_carout(car_owner_database, serial_fd)) != NULL)
		{
			if(strlen(p) == 8)
			{
				//printf("p values = %s      ", p);
				sprintf(pp, "%s", p);
				//printf("pp values = %s\n", pp);
				if(strlen(pp) == 8)
					select_cardid(pp, car_owner_database);
			}
		}
		else
			sleep(3);
	}
		 
    return 0;
}
