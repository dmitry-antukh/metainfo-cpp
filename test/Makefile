

CC=$(PREFIX)g++
LD=$(PREFIX)ld
STRIP=$(PREFIX)strip -s

TARGET=test
OBJ=test.o test1.o
HEADERS=pch.h PrintData.h struct.h test.h treater.h

CCFLAGS= -I ../
#OBJ_DIR=debug.gcc

all:	$(TARGET)

%.o:%.cpp
	$(CC) $(CCFLAGS) -c $<


$(TARGET):$(OBJ)
	$(CC) -o $@ $^
#	$(STRIP) $@

clean:
	rm -f *.o tcan
