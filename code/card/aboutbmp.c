#include "head.h"

/*
	����24λbmpͼƬ��ʾ�����к�����������
	��ʾȫ��
	����λ����ʾ�����С
	�������С��bmpͼƬ��д�������
*/

// �Զ��庯����ר��������ʾ800*480bmp��ʽ��ͼƬ
int show_fullbmp(char *bmpname)   //printf
{
	int i;
	int x,y;
	int lcdfd;
	int bmpfd;
	// ����һ���������ڴ��bmp����ɫֵ
	char bmpbuf[800*480*3];  // charռһ���ֽ�    bmpbuf[0] bmpbuf[1] bmpbuf[2]һ��
	int lcdbuf[800*480]; // intռ4�ֽ�
	
	// ����һ����ʱ����������ɫֵ
	int tempbuf[800*480];
	// ��lcd������
	lcdfd = open("/dev/fb0",O_RDWR);
	// ������
	if(lcdfd == -1)
	{
		printf("open lcd error!\n");
		return -1;
	}
	// ����Ҫ��ʾ��bmpͼƬ
	bmpfd = open(bmpname,O_RDONLY);
	if(bmpfd == -1)
	{
		printf("open bmp error!\n");
		return -1;
	}
	// �ڶ�֮ǰ��������ǰ��54�ֽ�
	lseek(bmpfd,54,SEEK_SET);
	// ��ȡbmpͼƬ����ɫֵ,��55�ֽ�λ�ÿ�ʼ��ȡ��
	read(bmpfd,bmpbuf,800*480*3);
	
	// ����ȡ����ɫֵ3�ֽ�---��ת����4�ֽ�
	for(i=0; i<800*480; i++)
		lcdbuf[i]=bmpbuf[3*i]|bmpbuf[3*i+1]<<8|bmpbuf[3*i+2]<<16|0x00<<24;

	// ���ߵ���ͼƬ��ת����(x,y)--->(x,479-y)
	for(x=0; x<800; x++)
		for(y=0; y<480; y++)
			//lcdbuf[(479-y)*800+x] = lcdbuf[y*800+x];
			tempbuf[(479-y)*800+x] = lcdbuf[y*800+x];
			
	
	//��ת��֮�����ɫֵд�뵽lcd��������
	write(lcdfd,tempbuf,800*480*4);
	
	// �ر�lcd��bmp
	close(bmpfd);
	close(lcdfd);
	return 0;
}

// �Զ��庯��������λ����ʾ�����С��bmp
/*
	x,y��ͼƬ��ʾ���Ͻǵ�����
	w,h��ͼƬ����Ŀ�͸�   ����
*/
int show_shapebmp(int x,int y,int w,int h,char *bmpname)   //printf
{
	int i,j;
	int lcdfd;
	int bmpfd;
	int *lcdmem;
	// ����һ���������ڴ��bmp����ɫֵ
	char bmpbuf[w*h*3];  // charռһ���ֽ�    bmpbuf[0] bmpbuf[1] bmpbuf[2]һ��
	int lcdbuf[w*h]; // intռ4�ֽ�
	
	// ��lcd������
	lcdfd = open("/dev/fb0",O_RDWR);
	// ������
	if(lcdfd == -1)
	{
		printf("open lcd error!\n");
		return -1;
	}

	// ��lcd�ĵ�ַӳ�䵽�û��ռ�
	lcdmem = (int *)mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcdfd,0);
	if(lcdmem==NULL)
	{
		printf("map lcd error!\n");
		return -1;
	}
	// ����Ҫ��ʾ��bmpͼƬ
	bmpfd = open(bmpname,O_RDONLY);
	if(bmpfd == -1)
	{
		printf("open bmp error!\n");
		return -1;
	}
	// �ڶ�֮ǰ��������ǰ��54�ֽ�
	lseek(bmpfd,54,SEEK_SET);
	// ��ȡbmpͼƬ����ɫֵ,��55�ֽ�λ�ÿ�ʼ��ȡ��
	read(bmpfd,bmpbuf,w*h*3);
	
	// ����ȡ����ɫֵ3�ֽ�---��ת����4�ֽ�
	for(i=0; i<w*h; i++)
		lcdbuf[i]=bmpbuf[3*i]|bmpbuf[3*i+1]<<8|bmpbuf[3*i+2]<<16|0x00<<24;

	// ����i,j��---���������Ļ��i+x,j+y����λ��
	for(i=0; i<w; i++)
		for(j=0; j<h; j++)
			*(lcdmem+(j+y)*800+i+x) = lcdbuf[(h-1-j)*w+i];
	// �ر�lcd��bmp
	close(bmpfd);
	close(lcdfd);
	// ����ڴ�ӳ��
	munmap(lcdmem,800*480*4);
	return 0;
}

/*�������С��ͼƬ��С��ĳ�ֱ���
	�������ܣ�x ��y --�����Ͻ�x,y����
	          w ---��ͼƬ��ʵ�ʿ�
			  h ---��ͼƬ��ʵ�ʸ�
			  scale---����С�ı���
			  bmpname---��ͼƬ��·��
	�����ص���ʾ��m�൱������С֮���ͼƬ�Ŀ�n�൱������С֮���ͼƬ�ĸ�
*/           
int show_shortbmp(int x,int y,int w,int h,int scale,char *bmpname)   
{
	int i,j;
	int m,n;
	int lcdfd;
	int bmpfd;
	int *lcdmem;
	// ����һ���������ڴ��bmp����ɫֵ
	char bmpbuf[w*h*3];  // charռһ���ֽ�    bmpbuf[0] bmpbuf[1] bmpbuf[2]һ��
	int lcdbuf[w*h]; // intռ4�ֽ�
	
	// ��lcd������
	lcdfd = open("/dev/fb0",O_RDWR);
	// ������
	if(lcdfd == -1)
	{
		printf("open lcd error!\n");
		return -1;
	}

	// ��lcd�ĵ�ַӳ�䵽�û��ռ�
	lcdmem = (int *)mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcdfd,0);
	if(lcdmem==NULL)
	{
		printf("map lcd error!\n");
		return -1;
	}
	// ����Ҫ��ʾ��bmpͼƬ
	bmpfd = open(bmpname,O_RDONLY);
	if(bmpfd == -1)
	{
		printf("open bmp error!\n");
		return -1;
	}
	// �ڶ�֮ǰ��������ǰ��54�ֽ�
	lseek(bmpfd,54,SEEK_SET);
	// ��ȡbmpͼƬ����ɫֵ,��55�ֽ�λ�ÿ�ʼ��ȡ��
	read(bmpfd,bmpbuf,w*h*3);
	
	// ����ȡ����ɫֵ3�ֽ�---��ת����4�ֽ�
	for(i=0; i<w*h; i++)
		lcdbuf[i]=bmpbuf[3*i]|bmpbuf[3*i+1]<<8|bmpbuf[3*i+2]<<16|0x00<<24;

	// ����i,j��---���������Ļ��i+x,j+y����λ��
	for(i=0,m=0; i<w; i+=scale,m++)
		for(j=0,n=0; j<h; j+=scale,n++)
			*(lcdmem+(n+y)*800+m+x) = lcdbuf[(h-1-j)*w+i];//��w*hͼƬ�ϵ�ĳ������䵽ָ��λ��
	// �ر�lcd��bmp
	close(bmpfd);
	close(lcdfd);
	// ����ڴ�ӳ��
	munmap(lcdmem,800*480*4);
	return 0;
}
