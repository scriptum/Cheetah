TARGET := cheetah
CFLAGS += -pipe -Os -g -fvisibility=hidden

EXTRA_WARNINGS = -Wall -Wextra -Wno-unused-parameter -Wuninitialized -Wstrict-overflow -Wshadow -Wbad-function-cast 
#EXTRA_WARNINGS += -Wmissing-include-dirs -Winline -Wdisabled-optimization -Wconversion
LIBS=../lib/linux64/libSDL2.a ../lib/linux64/libluajit.a
LDFLAGS += -g -ldl -lm -lrt -lpthread -lGLU -lGL -Wl,--export-dynamic,--gc-sections
INCLUDES := -I"../inc" -I"../inc/SDL2"

SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)
HEADERS := $(wildcard *.h)

THIRDPARTY_SOURCES = $(wildcard SOIL/*.c) image_write.c
THIRDPARTY_TARGETS = $(THIRDPARTY_SOURCES:.c=.o)

all : $(TARGET)

$(TARGET) : $(OBJECTS) $(THIRDPARTY_TARGETS)
	@echo "Linking $(TARGET)";\
	$(CC) $(LDFLAGS) $^ $(LIBS) -o "$@"

$(THIRDPARTY_TARGETS): %.o: %.c $(HEADERS)
	@echo "Building thirdparty $<";\
	$(CC) $(CFLAGS) $(INCLUDES) -o "$@" -c "$<"

%.o : %.c $(HEADERS)
	@echo "Building $<";\
	$(CC) $(CFLAGS) $(EXTRA_WARNINGS) $(INCLUDES) -o "$@" -c "$<"

$(TARGET).txt:
	@echo "Creating $@";\
	echo "{" > $@;\
	echo "    global : " >> $@;\
	grep -h '^[a-zA-Z].*(.*) {\s*$$' *.c | sed -e 's/(.*/;/' -e 's/.* \**/    /' >> $@;\
	echo "};" >> $@

check:
	@echo "Checking project...";\
	cppcheck -q $(SOURCES) $(THIRDPARTY_SOURCES)

clean :
	@echo "Cleaning...";\
	rm -f $(OBJECTS) $(THIRDPARTY_TARGETS) $(TARGET).txt $(TARGET)
