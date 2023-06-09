
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

INC += 
LIBS += -lmariadbclient -lpthread -lwiringPi
CFLAGS = -g -c $(INC) 
OBJECTS = farm_main.o farm_sensor.o
SRCS =  farm_main.c farm_sensor.c
DEFINES = 
TARGET = server_farm


all : $(TARGET)
$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(IOTLIB) $(LIBS) 
	
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(DEFINES)

install :
	cp -a $(TARGET) $(INSTALLDIR)/

clean :
	rm -rf $(OBJECTS) core

new : 
	$(MAKE) clean 
	$(MAKE) 
 
