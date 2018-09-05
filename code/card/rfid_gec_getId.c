#include "head.h"

volatile unsigned int cardid ;
static struct timeval timeout;
#define DEV_PATH   "/dev/ttySAC1"

/* ���ô��ڲ���:9600���� */
void init_tty(int fd)
{    
	//�������ô��ڵĽṹ��
	struct termios termios_new;
	//����ոýṹ��
	bzero( &termios_new, sizeof(termios_new));
	//	cfmakeraw()�����ն����ԣ���������termios�ṹ�еĸ���������
	cfmakeraw(&termios_new);
	//���ò�����
	//termios_new.c_cflag=(B9600);
	cfsetispeed(&termios_new, B9600);
	cfsetospeed(&termios_new, B9600);
	//CLOCAL��CREAD�ֱ����ڱ������Ӻͽ���ʹ�ܣ���ˣ�����Ҫͨ��λ����ķ�ʽ����������ѡ�    
	termios_new.c_cflag |= CLOCAL | CREAD;
	//ͨ��������������λΪ8λ
	termios_new.c_cflag &= ~CSIZE;
	termios_new.c_cflag |= CS8; 
	//��������żУ��
	termios_new.c_cflag &= ~PARENB;
	//һλֹͣλ
	termios_new.c_cflag &= ~CSTOPB;
	tcflush(fd,TCIFLUSH);
	// �����ý����ַ��͵ȴ�ʱ�䣬������Ҫ����Խ�������Ϊ0
	termios_new.c_cc[VTIME] = 10;
	termios_new.c_cc[VMIN] = 1;
	// �����������/���������
	tcflush (fd, TCIFLUSH);
	//������ú󣬿���ʹ�����º������������
	if(tcsetattr(fd,TCSANOW,&termios_new) )
		printf("Setting the serial1 failed!\n");

}


/*����У���*/
unsigned char CalBCC(unsigned char *buf, int n)
{
	int i;
	unsigned char bcc=0;
	for(i = 0; i < n; i++)
	{
		bcc ^= *(buf+i);
	}
	return (~bcc);
}

//  ����A����
int PiccRequest(int fd)
{
	unsigned char WBuf[128], RBuf[128];
	int  ret, i;
	fd_set rdfd;
	
	memset(WBuf, 0, 128);
	memset(RBuf,1,128);
	WBuf[0] = 0x07;	//֡��= 7 Byte
	WBuf[1] = 0x02;	//����= 0 , ��������= 0x01
	WBuf[2] = 0x41;	//����= 'A'
	WBuf[3] = 0x01;	//��Ϣ����= 1
	WBuf[4] = 0x52;	//����ģʽ:  ALL=0x52
	WBuf[5] = CalBCC(WBuf, WBuf[0]-2);		//У���
	WBuf[6] = 0x03; 	//������־

	FD_ZERO(&rdfd);
	FD_SET(fd,&rdfd);

	write(fd, WBuf, 7);;
	ret = select(fd + 1,&rdfd, NULL,NULL,&timeout);
	switch(ret)
	{
		case -1:
			//perror("select error\n");
			break;
		case  0:
			//printf("Request timed out.\n");
			break;
		default:
			ret = read(fd, RBuf, 8);
			if (ret < 0)
			{
				//printf("ret = %d, %m\n", ret, errno);
				break;
			}
			if (RBuf[2] == 0x00)	 	//Ӧ��֡״̬����Ϊ0 ������ɹ�
			{
				return 0;
			}
			break;
	}
	return -1;
}


/*����ײ����ȡ��Χ�����ID*/
int PiccAnticoll(int fd)
{
	unsigned char WBuf[128], RBuf[128];
	int ret, i;
	fd_set rdfd;;
	memset(WBuf, 0, 128);
	memset(RBuf,0,128);
	WBuf[0] = 0x08;	//֡��= 8 Byte
	WBuf[1] = 0x02;	//����= 0 , ��������= 0x01
	WBuf[2] = 0x42;	//����= 'B'
	WBuf[3] = 0x02;	//��Ϣ����= 2
	WBuf[4] = 0x93;	//����ײ0x93 --һ������ײ
	WBuf[5] = 0x00;	//λ����0
	WBuf[6] = CalBCC(WBuf, WBuf[0]-2);		//У���
	WBuf[7] = 0x03; 	//������־
	
	FD_ZERO(&rdfd);
	FD_SET(fd,&rdfd);
	write(fd, WBuf, 8);

	ret = select(fd + 1,&rdfd, NULL,NULL,&timeout);
	switch(ret)
	{
		case -1:
			//perror("select error\n");
			break;
		case  0:
			//perror("Timeout:");
			break;
		default:
			ret = read(fd, RBuf, 10);
			if (ret < 0)
			{
				//printf("ret = %d, %m\n", ret, errno);
				break;
			}
			if (RBuf[2] == 0x00) //Ӧ��֡״̬����Ϊ0 ���ȡID �ɹ�
			{
				cardid = (RBuf[7]<<24) | (RBuf[6]<<16) | (RBuf[5]<<8) | RBuf[4];//����õ��Ŀ�ID
				return 0;
			}
	}
	return -1;
}

/*
	ʵʱ��⴮�ڵ����ݣ��жϽ������ǳ����������ɹ�����0��ʧ�ܷ���-1
	���������ݿ�ָ��
*/
char *carin_or_carout(sqlite3 *car_owner_database, int fd)
{
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	
	while(1) //�������
	{
		/*�������߷�Χ�Ŀ�   ����A����*/
		if ( PiccRequest(fd)==0 )
		{
			printf("there was one card\n");	
		}
		usleep(100000);
		/*���з���ײ����ȡ���߷�Χ������ID   ����B����*/
		if( PiccAnticoll(fd)==0 )
		{
			break;
		}		
		usleep(100000);
	}
		
	//printf("172 found the RFID  %d\n", cardid);
	char buf[100];
	char *p = buf;
	bzero(buf, 100);
	sprintf(buf, "%d", cardid);
	//printf("177 found the RFID  %s\n", buf);
	//printf("178 found the RFID  %s\n", p);
	
	if(p != NULL)
		return p;
	else
		return 	NULL;
}


/*
	�򿪴����豸����ʼ��
	���������ڵ��ļ���
*/

int open_tty(int *fd)
{
	*fd = open(DEV_PATH, O_RDWR | O_NOCTTY | O_NONBLOCK);//�Է�����
	if (fd < 0)
	{
		fprintf(stderr, "Open Gec210_ttySAC1 fail!\n");
		return -1;
	}
	init_tty(*fd);
	
	return 0;
}


















