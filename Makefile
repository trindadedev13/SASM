CC                  = gcc
RB                  = ruby

BUILD_DIR           = build
LIBC_DIR            = libc
LIBC_LIBSTATIC      = $(LIBC_DIR)/build/libc.a
LIBC_BUILDSCRIPT    = build.rb

SASM_EXECUTABLE     = sasm
SASM_EXECUTABLE_DIR = $(BUILD_DIR)/$(SASM_EXECUTABLE)
SASM_TEMP           = $(HOME)/temp/c/sasm

SASM_FLAGS          = -nostdlib -nodefaultlibs -fno-builtin -Iinclude/ -I$(LIBC_DIR)/include/ -fsanitize=address -g
LINK_FLAGS          = -nostartfiles

MAIN_SRC            = src/main.c

LEXER_SRC           = src/lexer.c
LEXER_O             = $(BUILD_DIR)/lexer.o

PARSER_SRC          = src/parser.c
PARSER_O            = $(BUILD_DIR)/parser.o

TOKEN_SRC           = src/token.c
TOKEN_O             = $(BUILD_DIR)/token.o

REGS_SRC            = src/regs.c
REGS_O              = $(BUILD_DIR)/regs.o

INTERPRETER_SRC     = src/interpreter.c
INTERPRETER_O       = $(BUILD_DIR)/interpreter.o

NODE_SRC            = src/node.c
NODE_O              = $(BUILD_DIR)/node.o

OBJS                = $(LEXER_O) $(PARSER_O) $(TOKEN_O) $(REGS_O) $(INTERPRETER_O) $(NODE_O)

.PHONY: all setup clean run runt debug

all: setup $(LIBC_LIBSTATIC) $(SASM_EXECUTABLE_DIR)

setup:
	mkdir -p $(BUILD_DIR)

$(SASM_EXECUTABLE_DIR): $(MAIN_SRC) $(OBJS)
	$(CC) $(MAIN_SRC) -o $@ $(OBJS) $(SASM_FLAGS) $(LIBC_LIBSTATIC) $(LINK_FLAGS)

$(LEXER_O): $(LEXER_SRC)
	$(CC) -c $< -o $@ $(SASM_FLAGS)

$(PARSER_O): $(PARSER_SRC)
	$(CC) -c $< -o $@ $(SASM_FLAGS)

$(TOKEN_O): $(TOKEN_SRC)
	$(CC) -c $< -o $@ $(SASM_FLAGS)

$(REGS_O): $(REGS_SRC)
	$(CC) -c $< -o $@ $(SASM_FLAGS)

$(NODE_O): $(NODE_SRC)
	$(CC) -c $< -o $@ $(SASM_FLAGS)

$(INTERPRETER_O): $(INTERPRETER_SRC)
	$(CC) -c $< -o $@ $(SASM_FLAGS)

$(LIBC_LIBSTATIC): $(LIBC_DIR)/$(LIBC_BUILDSCRIPT)
	cd $(LIBC_DIR) && $(RB) $(LIBC_BUILDSCRIPT)

run:
	$(SASM_EXECUTABLE_DIR)

runt:
	mkdir -p $(SASM_TEMP)
	cp $(SASM_EXECUTABLE_DIR) $(SASM_TEMP)/$(SASM_EXECUTABLE)
	chmod +x $(SASM_TEMP)/$(SASM_EXECUTABLE)
	$(SASM_TEMP)/$(SASM_EXECUTABLE) main.sasm

clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(LIBC_DIR)/build/*

debug: CFLAGS += -g
debug: clean all
