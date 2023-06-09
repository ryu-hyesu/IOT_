
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
SHELL=/bin/sh
MAKE = make

INC = -I$(PWD)/include
LIBS = -L$(PWD)/libs
INSTALLDIR = $(PWD)/output
IOTLIB = $(PWD)/libs/libiot.a
SUBDIRS = libs src

export INC
export INSTALLDIR
export IOTLIB

all :
	@for i in $(SUBDIRS); do \
    echo "make all in $$i..."; \
    (cd $$i; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) all); done

install:
	mkdir $(INSTALLDIR)
	@for i in $(SUBDIRS); do \
	echo "Installing in $$i..."; \
	(cd $$i; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) install); done
								 
clean:
	rm -rf $(INSTALLDIR)
	@for i in $(SUBDIRS); do \
	echo "Clearing in $$i..."; \
	(cd $$i; $(MAKE) $(MFLAGS) $(MYMAKEFLAGS) clean); done
															 
