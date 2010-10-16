CC = g++

CFLAGS = -Wall `pkg-config glib-2.0 --cflags`

#OBJS = Utils.o

LIBS = -lGL -lglut -lGLU -lglib-2.0 -lm -lsmf

all: musa
 
musa: main.c
	$(CC) $(CFLAGS) $^ -o $@ $(FLAGS) $(LIBS) 

#Utils.o: utils.cpp utils.h
#	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS) 

clean:
	rm -f musa
	rm -f *.o
