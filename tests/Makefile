CFLAGS += -Ofast -pipe -fomit-frame-pointer -mtune=generic

LDFLAGS += -lm

FONT_ADD_TARGETS = font_hash_1251 font_hash_nodraw font_hash_noptr

OUT_DIR=bin

SOURCES = $(wildcard *.c)
TARGETS := $(SOURCES:.c=) $(FONT_ADD_TARGETS)
HEADERS := $(wildcard ../src/*.h)

all : $(TARGETS)

% : %.c $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(OUT_DIR)/"$@" "$<"

font_hash_1251: font_hash.c $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -DTEST_NOUNICODE font_hash.c -o $(OUT_DIR)/font_hash_1251
font_hash_nodraw: font_hash.c $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -DTEST_NODRAW font_hash.c -o $(OUT_DIR)/font_hash_nodraw
font_hash_noptr: font_hash.c $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -DTEST_NOHASHPTR font_hash.c -o $(OUT_DIR)/font_hash_noptr

test_font_hash_ru: font_hash $(FONT_ADD_TARGETS)
	@./$(OUT_DIR)/font_hash font_data/font.fnt font_data/russian.txt "Cheetah hash rus, no kern"
	./$(OUT_DIR)/font_hash font_data/font_kerning.fnt font_data/russian.txt "Cheetah hash rus, kerning"
	@./$(OUT_DIR)/font_hash_noptr font_data/font.fnt font_data/russian.txt "Hash without ptrs rus"
	@./$(OUT_DIR)/font_hash_1251 font_data/font_1251.fnt font_data/russian_1251.txt "1251 encoding rus"
	@./$(OUT_DIR)/font_hash_nodraw font_data/font.fnt font_data/russian.txt "Drawing disabled rus"

test_font_hash_en: font_hash $(FONT_ADD_TARGETS)
	@./$(OUT_DIR)/font_hash font_data/font.fnt font_data/english.txt "Cheetah hash eng, no kern"
	@./$(OUT_DIR)/font_hash_noptr font_data/font.fnt font_data/english.txt "Hash without ptrs eng"
	@./$(OUT_DIR)/font_hash_1251 font_data/font_1251.fnt font_data/english.txt "1251 encoding eng"
	@./$(OUT_DIR)/font_hash_nodraw font_data/font.fnt font_data/english.txt "Drawing disabled eng"

test_font_hash: test_font_hash_ru test_font_hash_en

test_random: random
	@./$(OUT_DIR)/random

test: test_font_hash test_random

clean :
	rm -f $(OUT_DIR)/*
