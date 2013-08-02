CFLAGS += -Ofast -pipe

LDFLAGS += -lm

SOURCES = $(wildcard *.c)
TARGETS := $(SOURCES:.c=)
HEADERS := $(wildcard ../src/*.h)

all : $(TARGETS)

% : %.c $(HEADERS)
	@echo "Building $<";\
	$(CC) $(CFLAGS) -o "$@" "$<"

clean :
	@echo "Cleaning...";\
	rm -f $(TARGETS)
