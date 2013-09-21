TARGET := libcheetah.so
CFLAGS += -pipe -fPIC

EXTRA_WARNINGS = -Wall -Wextra -Wno-unused-parameter -Wuninitialized -Wstrict-overflow -Wshadow -Wbad-function-cast -Wconversion
#EXTRA_WARNINGS += -Wmissing-include-dirs -Winline -Wdisabled-optimization

LDFLAGS += -shared -L"../lib" -lSDL -lGL
INCLUDES := -I"../inc"

SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
HEADERS := $(wildcard *.h)

THIRDPARTY_SOURCES = $(wildcard SOIL/*.c) image_write.c
THIRDPARTY_TARGETS = $(THIRDPARTY_SOURCES:.c=.o)

all : $(TARGET)

$(TARGET) : $(OBJECTS) $(THIRDPARTY_TARGETS)
	@echo "Linking $(TARGET)";\
	$(CC) -o "$@" $^ $(CFLAGS) $(LDFLAGS)

$(THIRDPARTY_TARGETS): %.o: %.c $(HEADERS)
	@echo "Building thirdparty $<";\
	$(CC) $(CFLAGS) $(INCLUDES) -o "$@" -c "$<"

%.o : %.c $(HEADERS)
	@echo "Building $<";\
	$(CC) $(CFLAGS) $(EXTRA_WARNINGS) $(INCLUDES) -o "$@" -c "$<"

clean :
	@echo "Cleaning...";\
	rm -f $(OBJECTS) $(THIRDPARTY_TARGETS)
