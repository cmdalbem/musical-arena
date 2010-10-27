CC = g++

CFLAGS = -Wall `pkg-config glib-2.0 --cflags`

OBJS = Decoder.o

LIBS = -lGL -lglut -lGLU -lglib-2.0 -lm -lsmf -lpthread -I"irrKlang-1.3.0/include" irrKlang-1.3.0/bin/linux-gcc/libIrrKlang.so

all: musa
 
musa: main.cpp $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(FLAGS) $(LIBS) 

Decoder.o: Decoder.cpp Decoder.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS) 

clean:
	rm -f musa
	rm -f *.o
