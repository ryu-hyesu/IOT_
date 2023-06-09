
 #############################################################################
 #
 # File Name: Makefile
 #
 # Copyright (C) 2014 HUINS,Inc.
 # Programmed by Kim suhak
 # email : shkim@huins.com
 # file creation : 2014/12/26
 #
 #############################################################################
.SUFFIXES : .c .o
CC = gcc
AR = ar

INC += -I/usr/include/mysql
LIBS += -L/usr/lib/i386-linux-gnu/
CFLAGS = -c $(INC) 
OBJECTS = iot_db.o parser.o
SRCS = iot_db.o parser.o
TARGET = libiot.a

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(AR) rcs $@ -o $(OBJECTS)

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@ $(DEFINES)

clean :
	rm -rf $(OBJECTS) $(TARGET) core

new : 
	$(MAKE) clean 
	$(MAKE) 

