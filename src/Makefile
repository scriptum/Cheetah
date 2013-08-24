TARGET := libcheetah.so
CFLAGS += -pipe -fPIC

EXTRA_WARNINGS = -Wall -Wextra -Wno-unused-parameter -Wmissing-include-dirs -Wuninitialized -Wstrict-overflow=5 -Wshadow -Wunsafe-loop-optimizations -Wbad-function-cast -Wcast-qual -Wconversion

LDFLAGS += -shared -L"../lib" -lSDL -lGL
INCLUDES := -I"../inc"
SOURCEDIR := $(shell find . -type d)

SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
HEADERS := $(wildcard *.h)

THIRDPARTY_SOURCES = $(wildcard SOIL/*.c) image_write.c
THIRDPARTY_TARGETS = $(THIRDPARTY_SOURCES:.c=.o)

all : $(TARGET)

$(TARGET) : $(OBJECTS) $(THIRDPARTY_TARGETS)
	@echo "Linking $(TARGET)";\
	$(CC) -o $@ $(OBJECTS) $(CFLAGS) $(LDFLAGS)

$(THIRDPARTY_TARGETS): %.o: %.c $(HEADERS)
	mkdir -p .obj
	@echo "Building thirdparty $<";\
	$(CC) $(CFLAGS) $(INCLUDES) -o "$@" -c "$<"

%.o : %.c $(HEADERS)
	mkdir -p .obj
	@echo "Building $<";\
	$(CC) $(CFLAGS) $(EXTRA_WARNINGS) $(INCLUDES) -o "$@" -c "$<"

clean :
	@echo "Cleaning...";\
	rm -f $(OBJECTS) $(THIRDPARTY_TARGETS)
