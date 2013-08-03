CFLAGS += -Ofast -pipe

LDFLAGS += -lm

FONT_ADD_TARGETS = font_hash_1251 font_hash_nodraw font_hash_noptr

SOURCES = $(wildcard *.c)
TARGETS := $(SOURCES:.c=) $(FONT_ADD_TARGETS)
HEADERS := $(wildcard ../src/*.h)

all : $(TARGETS)

% : %.c $(HEADERS)
	$(CC) $(CFLAGS) -o "$@" "$<"

font_hash_1251: font_hash.c $(HEADERS)
	$(CC) $(CFLAGS) -DTEST_NOUNICODE font_hash.c -o font_hash_1251
font_hash_nodraw: font_hash.c $(HEADERS)
	$(CC) $(CFLAGS) -DTEST_NODRAW font_hash.c -o font_hash_nodraw
font_hash_noptr: font_hash.c $(HEADERS)
	$(CC) $(CFLAGS) -DTEST_NOHASHPTR font_hash.c -o font_hash_noptr

test_font_hash_ru: font_hash $(FONT_ADD_TARGETS)
	@./font_hash font_data/font.fnt font_data/russian.txt "Cheetah hash rus, no kern"
	@./font_hash_noptr font_data/font.fnt font_data/russian.txt "Hash without ptrs rus"
	@./font_hash_1251 font_data/font_1251.fnt font_data/russian_1251.txt "1251 encoding rus"
	@./font_hash_nodraw font_data/font.fnt font_data/russian.txt "Drawing disabled rus"

test_font_hash_en: font_hash $(FONT_ADD_TARGETS)
	@./font_hash font_data/font.fnt font_data/english.txt "Cheetah hash eng, no kern"
	@./font_hash_noptr font_data/font.fnt font_data/english.txt "Hash without ptrs eng"
	@./font_hash_1251 font_data/font_1251.fnt font_data/english.txt "1251 encoding eng"
	@./font_hash_nodraw font_data/font.fnt font_data/english.txt "Drawing disabled eng"

test_font_hash: test_font_hash_ru test_font_hash_en

test_random: random
	@./random

test: test_font_hash test_random

clean :
	rm -f $(TARGETS)
