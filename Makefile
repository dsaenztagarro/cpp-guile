.PHONY: all build main link clean sources

CC = clang

OUTPUT_DIR := build

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

COMPILE.c = $(CC) $(CFLAGS) -c
LINK.o    = $(CC) $(CFLAGS) $(LIBS)

OUTPUT_OPTION = -o $(OUTPUT_DIR)/$@


all: build link

build: main
	$(call log-action, "Linking", "bin/bfs")

main: src/main.o

link:
	@echo "Linking"
	@mkdir -p bin
	@$(LINK.o) build/src/main.o $(OBJECTS) -o bin/main

%.o: %.c
	@echo "Compiling $<"
	@mkdir -p $(shell dirname build/$<)
	@$(COMPILE.c) $< $(OUTPUT_OPTION)

clean:
	@$(RM) bin/
	@$(RM) build/
	$(call log-action, "Cleaning")

sources:
	$(info $(SOURCES))
