
# Finds out if Make has to use the precompiled fmod library for 32 or 64 bits
MYSYSTEM := $(shell uname -m)
ifeq ($(MYSYSTEM), x86_64)
FMODFILE = libfmodex64.so
else
FMODFILE = libfmodex.so
endif

CC = g++
FLAGS = -Wall -I"/usr/include" -I"irrlicht-1.7.1/include" -I"fmod/api/inc" -I"lib/tree-2.65/src" -DHAVE_FMOD

LIBS = -lGL -lglut -lGLU -lglib-2.0 -lm -lsmf -lpthread -lXxf86vm -lXext -lX11 `pkg-config glib-2.0 --cflags` "irrlicht-1.7.1/lib/Linux/libIrrlicht.a" $(FMODLIB)

FMODLIB = fmod/api/lib/$(FMODFILE)

OBJDIR = obj
OBJECTS = Decoder.o utils.o Stone.o Fretting.o Track.o Player.o Screen.o Skill.o EventReceiver.o CBoltSceneNode.o TrackSceneNode.o
OBJS = $(addprefix $(OBJDIR)/,$(OBJECTS))

BIN = musa


all: main.cpp $(OBJS)
	$(CC) $(FLAGS) $^ -o $(BIN) $(FLAGS) $(LIBS)

$(OBJDIR)/%.o: %.cpp %.h | $(OBJDIR)
	$(CC) $(FLAGS) -c $< -o $@ $(FLAGS) $(LIBS) 

$(OBJDIR):
	mkdir $(OBJDIR) -p

clean:
	rm -f musa
	rm -f *.o
	rm -f -r $(OBJDIR)
