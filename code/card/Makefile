
CC=arm-none-linux-gnueabi-gcc

TARGET=park

SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))

$(TARGET):$(OBJS)
	$(CC)  $(OBJS) -o $(TARGET) -I /home/gec/develop/yueqian/sqlite3/sqlite-autoconf-3240000 -L /home/gec/develop/yueqian/sqlite3/sqlite-autoconf-3240000/.lib -lsqlite3 

$(OBJS):%.o:%.c
	$(CC) -c $^ -o $@
        
clean: 
	rm *.o $(TARGET) 




