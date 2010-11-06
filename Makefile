CC = g++

FLAGS = -Wall -I"/usr/include" -I"irrlicht-1.7.1/include" -I"fmod/api/inc"

OBJS = Decoder.o utils.o Stone.o

# Takes the fmod library name for your computer =D
MYSYSTEM := $(shell uname -m)
ifeq ($(MYSYSTEM), x86_64)
FMODLIB = libfmodex64.so
else
FMODLIB = libfmodex.so
endif

LIBS = -lGL -lglut -lGLU -lglib-2.0 -lm -lsmf -lpthread irrlicht-1.7.1/lib/Linux/libIrrlicht.a -lXxf86vm -lXext -lX11 `pkg-config glib-2.0 --cflags` fmod/api/lib/$(FMODLIB)
#-I"irrKlang-1.3.0/include" irrKlang-1.3.0/bin/linux-gcc/libIrrKlang.so


all: musa

musa: main.cpp $(OBJS)
	$(CC) $(FLAGS) $^ -o $@ $(FLAGS) $(LIBS)

Decoder.o: Decoder.cpp Decoder.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS)

Stone.o: Stone.cpp Stone.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS)

utils.o: utils.cpp utils.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS)

clean:
	rm -f musa
	rm -f *.o
