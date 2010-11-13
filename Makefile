
# Takes the fmod library name for your computer =D
MYSYSTEM := $(shell uname -m)
ifeq ($(MYSYSTEM), x86_64)
FMODLIB = libfmodex64.so
else
FMODLIB = libfmodex.so
endif

CC = g++

FLAGS = -Wall -I"/usr/include" -I"irrlicht-1.7.1/include" -I"fmod/api/inc" -I"lib/tree-2.65/src" -DHAVE_FMOD

OBJDIR = obj
OBJSNAMES = Decoder.o utils.o Stone.o Fretting.o Track.o Player.o Screen.o Skill.o EventReceiver.o CBoltSceneNode.o TrackSceneNode.o
$OBJS = $(objnames:%.o=$(OBJDIR)/%.o)

LIBS = -lGL -lglut -lGLU -lglib-2.0 -lm -lsmf -lpthread irrlicht-1.7.1/lib/Linux/libIrrlicht.a -lXxf86vm -lXext -lX11 `pkg-config glib-2.0 --cflags` fmod/api/lib/$(FMODLIB)


%.o: %.cpp %.h
	$(CC) $(FLAGS) $< -c $(FLAGS) $(LIBS)

all: musa

musa: main.cpp $(OBJSNAMES)
	$(CC) $(FLAGS) $^ -o $@ $(FLAGS) $(LIBS)

clean:
	rm -f musa
	rm -f *.o
