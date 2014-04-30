TARGET := cheetah
CFLAGS += -pipe -O2 -ftree-vectorize -g -fvisibility=hidden

EXTRA_WARNINGS = -Wall -Wextra -Wno-unused -Wuninitialized -Wstrict-overflow -Wshadow -Wbad-function-cast 
#EXTRA_WARNINGS += -Wmissing-include-dirs -Winline -Wdisabled-optimization -Wconversion
LIBS=../lib/linux64/libSDL2.a ../lib/linux64/libluajit.a
LDFLAGS += -g -ldl -lm -lrt -lpthread -lGLU -lGL -Wl,--export-dynamic,--gc-sections
INCLUDES := -I"../inc" -I"../inc/SDL2"

SOURCES := $(wildcard *.c)
TESTS_SRC := $(wildcard tests/*.c)
TESTS := $(TESTS_SRC:.c=)
OBJECTS := $(SOURCES:.c=.o)
LIBRARY_OBJECTS := $(OBJECTS:SDL_win32_main.o=)
LIBRARY_OBJECTS := $(LIBRARY_OBJECTS:main.o=)
LIBRARY_OBJECTS := $(LIBRARY_OBJECTS:image_write.o=)
TEST_OBJECTS := $(LIBRARY_OBJECTS:.o=_testobj.o)
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

%_testobj.o : %.c $(HEADERS)
	@echo "Building test object $<";\
	$(CC) $(CFLAGS) $(EXTRA_WARNINGS) $(INCLUDES) -DTEST_MODE -o "$@" -c "$<"

tests/% : tests/%.c $(TEST_OBJECTS) $(THIRDPARTY_TARGETS)
	$(CC) $(CFLAGS) $(EXTRA_WARNINGS) $(INCLUDES) $(LDFLAGS) $^ $(LIBS) -o "$@"

$(TARGET).txt:
	@echo "Creating $@";\
	echo "{" > $@;\
	echo "    global : " >> $@;\
	grep -h '^[a-zA-Z].*(.*) {\s*$$' *.c | sed -e 's/(.*/;/' -e 's/.* \**/    /' >> $@;\
	echo "};" >> $@

test: $(TESTS)

check:
	@echo "Checking project...";\
	cppcheck -q $(SOURCES) $(THIRDPARTY_SOURCES)

clean:
	@echo "Cleaning...";\
	rm -f $(OBJECTS) $(THIRDPARTY_TARGETS) $(TARGET).txt $(TARGET) $(TESTS)
