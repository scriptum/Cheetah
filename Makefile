# Makefile autogenerated by premake
# Don't edit this file! Instead edit `premake.lua` then rerun `make`
# Options:
#   CONFIG=[Release]

ifndef CONFIG
  CONFIG=Release
endif

export CONFIG

.PHONY: all clean cheetah

all: cheetah

Makefile: premake.lua
	@echo ==== Regenerating Makefiles ====
	@premake --file $^ --target gnu

cheetah:
	@echo ==== Building cheetah ====
	@$(MAKE) --no-print-directory -C . -f cheetah.make

clean:
	@$(MAKE) --no-print-directory -C . -f cheetah.make clean
