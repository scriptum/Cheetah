TARGET := libcheetah.so
CFLAGS += -Wall -Winline -pipe
LDFLAGS += -shared -L"lib" -lSDL -lGL
RESFLAGS := -I"inc"
SOURCEDIR := src src/SOIL

SOURCES := $(wildcard $(addsuffix /*.c*, $(SOURCEDIR)))
OBJECTS := $(SOURCES:.c=.o)
INCLUDES := $(wildcard $(addsuffix /*.h*, $(SOURCEDIR)))
all : $(TARGET)
 
$(TARGET) : $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(CFLAGS) $(LDFLAGS)


%.o : %.c $(INCLUDES)
	@echo "Building $<";\
	$(CC) $(CFLAGS) $(RESFLAGS) -o "$@" -c "$<"

.PHONY : clean
 
clean :
	rm -f $(OBJECTS)