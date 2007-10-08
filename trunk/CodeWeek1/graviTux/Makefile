CCFLAGS=-O0 -W -Wall
LDFLAGS=./*.o -lSDL -lGL
DBFLAGS=-ggdb #-DDEBUG

CC=g++ -c
LD=g++

all:
	$(CC) $(CCFLAGS) $(DBFLAGS) ./base.cpp
	$(CC) $(CCFLAGS) $(DBFLAGS) ./drawable.cpp
	$(CC) $(CCFLAGS) $(DBFLAGS) ./graviTux.cpp
	$(CC) $(CCFLAGS) $(DBFLAGS) ./effects.cpp
	$(LD) ./*.o -lSDL -lGL -o graviTux

clean:
	rm ./*.o
