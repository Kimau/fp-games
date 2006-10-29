#CC = gcc -Wall -ansi

#all:
#	$(CC) lesson01.c -o lesson01 -lGL -lGLU `sdl-config --cflags --libs`

objects = random.o loop.o loopfactory.o blind.o blindfactory.o lefty.o leftyfactory.o item.o itemfactory.o gold.o goldfactory.o book.o bookfactory.o seeker.o seekerfactory.o pawn.o player.o ghost.o ghostfactory.o footsteps.o camera.o bunnyfactory.o bunny.o beholder.o beholderfactory.o pointXYZ.o maze.o cellstack.o misc.o commongl.o controller.o controllerfactory.o monster.o monsterfactory.o levelmap.o level.o game.o main.o 
sources = random.cpp loop.cpp loopfactory.cpp blind.cpp blindfactory.cpp lefty.cpp leftyfactory.cpp item.cpp itemfactory.cpp gold.cpp goldfactory.cpp book.cpp bookfactory.cpp seeker.cpp seekerfactory.cpp pawn.cpp player.cpp ghost.cpp ghostfactory.cpp footsteps.cpp camera.cpp bunnyfactory.cpp bunny.cpp beholder.cpp beholderfactory.cpp pointXYZ.cpp maze.cpp cellstack.cpp misc.cpp commongl.cpp controller.cpp controllerfactory.cpp monster.cpp monsterfactory.cpp levelmap.cpp level.cpp game.cpp main.cpp

all: $(objects)
	@echo Building Game...
	g++ $(objects) -o Game -lGL -lGLU `sdl-config --libs`
	@echo Done

debug:
	@echo Building debug mode Game...
	g++ $(sources) -o DGame -D DEBUG_MODE -lGL -lGLU `sdl-config --cflags --libs`
	@echo Done

%.o: %.cpp
	g++ -c $< `sdl-config --cflags`
	
#tar: $(foreach file,$(basename $(wildcard lesson[1-9].c lesson1[0-3].c)),$(file).tar.gz)
Game.tar.gz : 
	tar cvf Game.tar *.cpp *.h Makefile Data/*.bmp
	gzip Game.tar
	
cleanobj:
	@echo Deleting .o files...
	@rm *.o
	@echo Done.

clean:
	@echo Cleaning up...
	@rm Game
	@echo Done.

cleanall: cleanobj clean
	@echo All done

rebuild: cleanobj all

