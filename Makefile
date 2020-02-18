CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: spacepatrol

spacepatrol: spacepatrol.cpp log.cpp timers.cpp andrewM.cpp williamS.cpp doneyP.cpp raagP.cpp
	g++ $(CFLAGS) spacepatrol.cpp log.cpp timers.cpp andrewM.cpp williamS.cpp doneyP.cpp raagP.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -ospacepatrol

clean:
	rm -f spacepatrol
	rm -f *.o

