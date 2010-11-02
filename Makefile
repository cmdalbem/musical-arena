CC = g++

CFLAGS = -Wall

OBJS = Decoder.o utils.o Stone.o

LIBS = -lGL -lglut -lGLU -lglib-2.0 -lm -lsmf -lpthread -I"irrKlang-1.3.0/include" irrKlang-1.3.0/bin/linux-gcc/libIrrKlang.so -I"irrlicht-1.7.1/include" irrlicht-1.7.1/lib/Linux/libIrrlicht.a -lXxf86vm -lXext -lX11 -I"/usr/include" `pkg-config glib-2.0 --cflags`

all: musa

musa: main.cpp $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(FLAGS) $(LIBS)

Decoder.o: Decoder.cpp Decoder.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS)

Stone.o: Stone.cpp Stone.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS)

utils.o: utils.cpp utils.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS)

clean:
	rm -f musa
	rm -f *.o
