TARGET := libcheetah.so
CFLAGS += -fPIC -O3 -fomit-frame-pointer -ffast-math -Wall
LDFLAGS += -shared -L"lib" -lSDL -lGL -s
RESFLAGS := -I"inc"
SOURCEDIR := src src/SOIL

SOURCES := $(wildcard $(addsuffix /*.c*, $(SOURCEDIR)))
OBJECTS := $(SOURCES:.c=.o)
INCLUDES := $(wildcard $(addsuffix /*.h*, $(SOURCEDIR)))
all : $(TARGET)
 
$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(CFLAGS) $(LDFLAGS)


%.o : %.c $(INCLUDES)
	$(CC) $(CFLAGS) $(RESFLAGS) -o "$@" -c "$<"

.PHONY : clean
 
clean :
	rm $(OBJECTS)