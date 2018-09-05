#ifndef _HEAD_H
#define _HEAD_H


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


#define PICTURE_PATH  "/root/project/park/ui.bmp"
#define DATABASE_PATH  "/root/project/park/car_owner_database.db"


int flag;

/*
	打开串口设备并初始化
	参数：串口的文件，
*/

int open_tty(int *fd);







// 自定义函数，专门用于显示800*480bmp格式的图片
int show_fullbmp(char *bmpname);
/*
	x,y是图片显示左上角的坐标
	w,h是图片本身的宽和高   像素
*/
int show_shapebmp(int x,int y,int w,int h,char *bmpname);
/*将任意大小的图片缩小成某种比例
	参数介绍：x ，y --》左上角x,y坐标
	          w ---》图片的实际宽
			  h ---》图片的实际高
			  scale---》缩小的倍数
			  bmpname---》图片的路径
	代码重点提示：m相当于是缩小之后的图片的宽，n相当于是缩小之后的图片的高
*/           
int show_shortbmp(int x,int y,int w,int h,int scale,char *bmpname);




//	停车场计费标准：3元每个小时

int select_time_result(void *arg,int column,char **column_val,char **column_name);
/*
	获取系统时间，成功返回进车的时间点，失败返回-1
	参数：无
*/
int get_local_time();
/*
	计算停车时间并打印停车费用，
	参数：数据库指针
*/

int calculate_total_parking_time(char *cardid, sqlite3 *car_owner_database);
/*
	根据停车时间计算停车费用，返回停车费用
	参数：停车总时间
*/

double calculate_charge_park(long int time);





/* 设置窗口参数:9600速率 */
void init_tty(int fd);
/*计算校验和*/
unsigned char CalBCC(unsigned char *buf, int n);
//  发送A命令
int PiccRequest(int fd);
/*防碰撞，获取范围内最大ID*/
int PiccAnticoll(int fd);
/*
	实时检测串口的数据，判断进车还是出车动作，成功返回卡的ID，失败返回-1
	参数：数据库指针
*/
char *carin_or_carout(sqlite3 *car_owner_database, int fd);




int select_cardid_result(void *arg,int column,char **column_val,char **column_name);
/*
     创建数据库，成功返回数据库指针，失败返回NULL
	 参数：数据库名字
*/
sqlite3 *create_car_owner_database(char *databasename);
/*
	查询数据库是否有该卡号，成功返回0，失败返回-1
	参数：数据指针car_owner_database，卡号cardid，
*/
int select_cardid(char *cardid,sqlite3 *car_owner_database);



/*
	处理入库动作，成功返回0，失败返回-1
	参数：要插入的信息，比如卡号
*/
int card_in(char *cardid,sqlite3 *car_owner_database);

/*
	处理出库动作，成功返回0，失败返回-1
	参数：要删除的信息，比如卡号
*/
int card_out(char *cardid,sqlite3 *car_owner_database);


#endif
