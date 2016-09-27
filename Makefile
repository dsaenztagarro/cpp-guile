.PHONY: all build main link clean sources server client

CC ?= clang

COMMON_DIR := "src/common"
OUTPUT_DIR := build

COMMON_SOURCES := $(shell find $(COMMON_DIR) -name *.c)
COMMON_DEPS := $(subst .c,.o, $(COMMON_SOURCES))
COMMON_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(COMMON_DEPS))

# CFLAGS

GUILE_CFLAGS := `guile-config compile`

WARNINGS := -Wall -Wextra -pedantic

CFLAGS ?= -std=gnu99 -g $(WARNINGS) $(GUILE_CFLAGS)

# LIBS

GUILE_LIBS = `guile-config link`
LIBS = $(GUILE_LIBS)

INCLUDES := -Iincludes/common -Ibuild/includes

# COMMANDS

COMPILE.c = $(CC) $(CFLAGS) $(INCLUDES) -c
LINK.o    = $(CC) $(CFLAGS) $(INCLUDES) $(LIBS)

OUTPUT_OPTION = -o $(OUTPUT_DIR)/$@

EXEC := server

# OPTIONS

ifeq ($(VERBOSE),1)
	SILENCER :=
else
	SILENCER := @
endif

build: src/$(EXEC).o ename.c.inc $(COMMON_DEPS)
	@echo "Linking bin/$(EXEC)"
	$(SILENCER)$(LINK.o) build/$< $(COMMON_OBJECTS) -o bin/$(EXEC)

%.o: %.c
	@echo "Compiling $<"
	$(SILENCER)mkdir -p $(shell dirname build/$<)
	$(SILENCER)$(COMPILE.c) $< $(OUTPUT_OPTION)

ename.c.inc:
	@echo "Generating ename.c.inc header"
	$(SILENCER)mkdir -p build/includes
	$(SILENCER)./scripts/build_ename.sh > ./build/includes/ename.c.inc

clean:
	$(SILENCER)$(RM) bin/
	$(SILENCER)$(RM) build/

leaks: src/leaks.o

valgrind:
	valgrind --leak-check=yes ./bin/$(EXEC)

main:
	@make EXEC=main build

sv_dgram:
	@make EXEC=sv_dgram build

cl_dgram:
	@make EXEC=cl_dgram build
