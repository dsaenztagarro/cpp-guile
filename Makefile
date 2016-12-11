.PHONY: all build main link clean sources server client

# OPTIONS

ifeq ($(VERBOSE),'yes')
	$(info testA)
	SILENCER :=@
else
	$(info testB)
	SILENCER :=@
endif

SILENCER :=@

.DEFAULT_GOAL := mazingerz

CC ?= cc

EXEC := mazingerz

COMMON_DIR := "src/common"
EXEC_DIR := "src/$(EXEC)"
OUTPUT_DIR := build

COMMON_SOURCES := $(shell find $(COMMON_DIR) -name *.c)
COMMON_DEPS := $(subst .c,.o, $(COMMON_SOURCES))
COMMON_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(COMMON_DEPS))

ROBOT_DIR := "src/$(EXEC)"

ROBOT_SOURCES := $(shell find $(ROBOT_DIR) -name *.c)
ROBOT_DEPS := $(subst .c,.o, $(ROBOT_SOURCES))
ROBOT_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(ROBOT_DEPS))

EXEC_SOURCES := $(shell find $(EXEC_DIR) -name *.c)
EXEC_DEPS := $(subst .c,.o, $(EXEC_SOURCES))
EXEC_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(EXEC_DEPS))

DEPS := $(COMMON_DEPS) $(ROBOT_DEPS) $(EXEC_DEPS)
OBJECTS := $(COMMON_OBJECTS) $(ROBOT_OBJECTS)

# CFLAGS

# GUILE_CFLAGS := `guile-config compile`

# WARNINGS := -Wall -Wextra -pedantic
WARNINGS := -Wall -Wextra

CFLAGS ?= -g $(WARNINGS) # $(GUILE_CFLAGS)
# CFLAGS ?= -std=gnu99 -g $(WARNINGS) # $(GUILE_CFLAGS)



# LIBS

# GUILE_LIBS = `guile-config link`
# LIBS = $(GUILE_LIBS)

INCLUDES := -Iinclude/ -Ibuild/include

# COMMANDS

COMPILE.c = $(CC) $(MACROS) $(CFLAGS) $(INCLUDES) -c
LINK.o    = $(CC) $(MACROS) $(CFLAGS) $(INCLUDES) $(LIBS)

OUTPUT_OPTION = -o $(OUTPUT_DIR)/$@


build: src/$(EXEC).o ename.c.inc $(DEPS)
	@echo "Linking bin/$(EXEC)"
	$(SILENCER)$(LINK.o) build/$< $(OBJECTS) -o bin/$(EXEC)

%.o: %.c
	@echo "Compiling $<"
	$(SILENCER)mkdir -p $(shell dirname build/$<)
	$(SILENCER)$(COMPILE.c) $< $(OUTPUT_OPTION)

ename.c.inc:
	@echo "Generating ename.c.inc header"
	$(SILENCER)mkdir -p build/include
	$(SILENCER)./scripts/build_ename.sh > ./build/include/ename.c.inc

clean:
	$(SILENCER)$(RM) bin/
	$(SILENCER)$(RM) build/

leaks: src/leaks.o

valgrind:
	valgrind --leak-check=yes ./bin/$(EXEC)

main:
	@make EXEC=main build

mazingerz:
	@make EXEC=mazingerz build

test:
	rm -f /tmp/mazingerz.socket
	@make MACROS="-D TEST" EXEC=mazingerz build
	./bin/mazingerz


test_message:
	@mkdir -p bin/mazingerz
	@make MACROS="-D TEST" EXEC=mazingerz/message build
	./bin/mazingerz/message
	# @valgrind --leak-check=yes ./bin/mazingerz/message

test2:
	$(CC) $(MACROS) $(CFLAGS) $(INCLUDES) -D TEST -o ./bin/test src/common/list.c src/common/test.c include/common/test.h
	./bin/test

cl_dgram:
	@make EXEC=cl_dgram build
