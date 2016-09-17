.PHONY: all build main link clean sources server client

CC = clang

COMMON_DIR := "src/common"
OUTPUT_DIR := build

COMMON_SOURCES := $(shell find $(COMMON_DIR) -name *.c)
COMMON_DEPS := $(subst .c,.o, $(COMMON_SOURCES))
COMMON_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(COMMON_DEPS))

GUILE_CFLAGS := `guile-config compile`

WARNINGS := -Wall -Wextra -pedantic

CFLAGS ?= -std=gnu99 -g $(WARNINGS) $(GUILE_CFLAGS)

GUILE_LIBS = `guile-config link`
LIBS = $(GUILE_LIBS)

INCLUDES := -Iincludes/common

COMPILE.c = $(CC) $(CFLAGS) $(INCLUDES) -c
LINK.o    = $(CC) $(CFLAGS) $(INCLUDES) $(LIBS)

OUTPUT_OPTION = -o $(OUTPUT_DIR)/$@

EXEC := server

SOURCES = $(shell find $(LIB_DIR) -name *.c)
DEPS = $(subst .c,.o, $(SOURCES))
OBJECTS = $(addprefix $(OUTPUT_DIR)/, $(DEPS))

ifeq ($(VERBOSE),1)
	SILENCER :=
else
	SILENCER := @
endif

build: src/$(EXEC).o $(COMMON_DEPS)
	@echo "Linking bin/$(EXEC)"
	$(SILENCER)$(LINK.o) build/$< $(COMMON_OBJECTS) -o bin/$(EXEC)

%.o: %.c
	@echo "Compiling $<"
	$(SILENCER)mkdir -p $(shell dirname build/$<)
	$(SILENCER)$(COMPILE.c) $< $(OUTPUT_OPTION)

clean:
	@$(RM) bin/
	@$(RM) build/

sources:
	$(info $(SOURCES))

leaks: src/leaks.o

valgrind:
	valgrind --leak-check=yes ./bin/$(EXEC)

main:
	@make EXEC=main build

server:
	@make EXEC=server build

client:
	@make EXEC=client build
