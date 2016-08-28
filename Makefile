.PHONY: all build main link clean sources server client

CC = clang

COMMON_DIR := "src/common"
OUTPUT_DIR := build

COMMON_SOURCES = $(shell find $(COMMON_DIR) -name *.c)

COMMON_DEPS = $(subst .c,.o, $(COMMON_SOURCES))
COMMON_OBJECTS = $(addprefix $(OUTPUT_DIR)/, $(COMMON_DEPS))

# GCC COMPILER FLAGS
#
# -Wall
#  tells the compiler to implement 'all' Warning options. Warnings are
#  diagnostic messages that report constructions which are not inherently
#  erroneous but which are risky or suggest there may have been an error. Very
#  useful for debugging code.
#
# -ansi
#  tells the compiler to implement the ANSI language option. This turns off
#  certain "features" of GCC which are incompatible with the ANSI standard.
#
# -pedantic
#  used in conjunction with -ansi, this tells the compiler to be adhere
#  strictly to the ANSI standard, rejecting any code which is not compliant.
#
# -o
# tells the compiler to save the compiled program under the name . So, typing
# "gcc myfile.c -o myfile.x" will take the source code of file "myfile.c" and
# create program "myfile.x" rather than the default program "a.out".
#
# -
GUILE_CFLAGS = `guile-config compile`
CFLAGS = -std=c11 \
				 -pedantic \
				 -Wall \
				 -Wextra \
				 -g \
				 $(GUILE_CFLAGS)

GUILE_LIBS = `guile-config link`
LIBS = $(GUILE_LIBS)

INCLUDES = -Iincludes/common

COMPILE.c = $(CC) $(CFLAGS) $(INCLUDES) -c
LINK.o    = $(CC) $(CFLAGS) $(INCLUDES) $(LIBS)

OUTPUT_OPTION = -o $(OUTPUT_DIR)/$@

EXEC := server

SOURCES = $(shell find $(LIB_DIR) -name *.c)
DEPS = $(subst .c,.o, $(SOURCES))
OBJECTS = $(addprefix $(OUTPUT_DIR)/, $(DEPS))


build: src/$(EXEC).o $(COMMON_DEPS)
	@echo "Linking bin/$(EXEC)"
	@$(LINK.o) build/$< $(COMMON_OBJECTS) -o bin/$(EXEC)

%.o: %.c
	@echo "Compiling $<"
	@mkdir -p $(shell dirname build/$<)
	@$(COMPILE.c) $< $(OUTPUT_OPTION)

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
