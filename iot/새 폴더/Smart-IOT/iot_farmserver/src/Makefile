
 #############################################################################
 #
 # File Name: Makefile
 #
 # Copyright (C) 2014 HUINS,Inc.
 # Programmed by Kim suhak
 # email : shkim@huins.com
 # file creation : 2014/12/23
 #
 #############################################################################
.SUFFIXES : .c .o
CC = gcc

INC += 
LIBS +=
CFLAGS = -g -c $(INC) 
DEFINES = 
SUBDIRS = farm

all : $(TARGET)
	@for i in $(SUBDIRS); do \
    echo "make all in $$i..."; \
    (cd $$i; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) all); done

install:
	@for i in $(SUBDIRS); do \
	echo "Installing in $$i..."; \
	(cd $$i; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) install); done

clean :
	@for i in $(SUBDIRS); do \
	echo "Clearing in $$i..."; \
	(cd $$i; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean); done

new : 
	$(MAKE) clean 
	$(MAKE) 

