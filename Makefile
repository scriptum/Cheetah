TARGET := libcheetah.so
CFLAGS += -fPIC -O3 -fomit-frame-pointer -ffast-math -Wall
LDFLAGS += -shared -s -L"lib" -lSDL -lGL
RESFLAGS := -I"inc"

SOURCEDIR := src
SOURCES := $(wildcard $(addsuffix /*.c*, src src/SOIL))
OBJECTS := $(SOURCES:.c=.o)

all : $(TARGET)
 
$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(CFLAGS) $(LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) $(RESFLAGS) -o "$@" -c "$<"
 
.PHONY : clean
 
clean :
	rm $(OBJECTS)