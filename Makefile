CC = g++

CFLAGS = -Wall `pkg-config glib-2.0 --cflags`

OBJS = Decoder.o utils.o

LIBS = -lGL -lglut -lGLU -lglib-2.0 -lm -lsmf -lpthread -lncurses
#-I"irrKlang-1.3.0/include" irrKlang-1.3.0/bin/linux-gcc/libIrrKlang.so

all: musa

musa: main.cpp $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(FLAGS) $(LIBS)

Decoder.o: Decoder.cpp Decoder.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS)

stone.o: stone.cpp stone.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS)

utils.o: utils.cpp utils.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS)

clean:
	rm -f musa
	rm -f *.o
