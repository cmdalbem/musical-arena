
# Finds out if Make has to use the precompiled fmod library for 32 or 64 bits
MYSYSTEM := $(shell uname -m)
ifeq ($(MYSYSTEM), x86_64)
FMODFILE = libfmodex64.so
else
FMODFILE = libfmodex.so
endif

CC = g++
FLAGS = -Wall -I"/usr/include" -I"irrlicht-1.7.2/include" -I"fmod/api/inc" -I"lib/tree-2.65/src" -I"nodes" -DHAVE_FMOD

LIBS = -lGL -lglut -lGLU -lglib-2.0 -lm -lsmf -lpthread -lXxf86vm -lXext -lX11 `pkg-config glib-2.0 --cflags` "irrlicht-1.7.2/lib/Linux/libIrrlicht.a" $(FMODLIB)

FMODLIB = fmod/api/lib/$(FMODFILE)

OBJDIR = obj
OBJNAMES = Decoder utils Stone Fretting Track Player Screen Skill EventReceiver CBoltSceneNode TrackSceneNode CBloodEffect CBloodShader VxHealthSceneNode CShieldManager
OBJECTS = $(addsuffix .o,$(OBJNAMES))
OBJS = $(addprefix $(OBJDIR)/,$(OBJECTS))

BIN = main


all: main.cpp $(OBJS)
	$(CC) $(FLAGS) $^ -o $(BIN) $(FLAGS) $(LIBS)

$(OBJDIR)/%.o: %.cpp %.h | $(OBJDIR)
	$(CC) $(FLAGS) -c $< -o $@ $(FLAGS) $(LIBS)

$(OBJDIR)/%.o: nodes/%.cpp nodes/%.h | $(OBJDIR)
	$(CC) $(FLAGS) -c $< -o $@ $(FLAGS) $(LIBS)

$(OBJDIR):
	mkdir $(OBJDIR) -p

clean:
	rm -f musa
	rm -f *.o
	rm -f -r $(OBJDIR)
