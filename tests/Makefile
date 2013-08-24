CFLAGS += -Ofast -pipe -fomit-frame-pointer -mtune=native

LDFLAGS += -lm

FONT_ADD_TARGETS = bin/font_hash_1251 bin/font_hash_nodraw bin/font_hash_noptr

OUT_DIR=bin

SOURCES = $(wildcard *.c)
TARGETS := $(addprefix bin/,$(SOURCES:.c=)) $(FONT_ADD_TARGETS)
HEADERS := $(wildcard ../src/*.h)

all : $(TARGETS)

bin/%: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o "$@" "$<"

bin/font_hash_1251: font_hash.c $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -DTEST_NOUNICODE "$<" -o "$@"
bin/font_hash_nodraw: font_hash.c $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -DTEST_NODRAW "$<" -o "$@"
bin/font_hash_noptr: font_hash.c $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -DTEST_NOHASHPTR "$<" -o "$@"

test_font_hash_ru: bin/font_hash $(FONT_ADD_TARGETS)
	@./$(OUT_DIR)/font_hash font_data/font.fnt font_data/russian.txt "Cheetah hash rus, no kern"
	@./$(OUT_DIR)/font_hash font_data/font_kerning.fnt font_data/russian.txt "Cheetah hash rus, kerning"
	@./$(OUT_DIR)/font_hash_noptr font_data/font.fnt font_data/russian.txt "Hash without ptrs rus"
	@./$(OUT_DIR)/font_hash_1251 font_data/font_1251.fnt font_data/russian_1251.txt "1251 encoding rus"
	@./$(OUT_DIR)/font_hash_nodraw font_data/font.fnt font_data/russian.txt "Drawing disabled rus"

test_font_hash_en: bin/font_hash $(FONT_ADD_TARGETS)
	@./$(OUT_DIR)/font_hash font_data/font.fnt font_data/english.txt "Cheetah hash eng, no kern"
	@./$(OUT_DIR)/font_hash_noptr font_data/font.fnt font_data/english.txt "Hash without ptrs eng"
	@./$(OUT_DIR)/font_hash_1251 font_data/font_1251.fnt font_data/english.txt "1251 encoding eng"
	@./$(OUT_DIR)/font_hash_nodraw font_data/font.fnt font_data/english.txt "Drawing disabled eng"

test_color test_random: test_%: bin/%
	./"$<"

test_font_hash: test_font_hash_ru test_font_hash_en

test: $(addprefix test_,$(SOURCES:.c=))

clean :
	rm -f $(TARGETS)
