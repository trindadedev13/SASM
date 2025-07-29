CC                  = gcc
RB                  = ruby
GDB                 = gdb

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

STACK_SRC           = src/stack.c
STACK_O             = $(BUILD_DIR)/stack.o

CPU_SRC             = src/cpu.c
CPU_O               = $(BUILD_DIR)/cpu.o

MEMORY_SRC          = src/memory.c
MEMORY_O            = $(BUILD_DIR)/memory.o

OBJS                = $(LEXER_O) $(PARSER_O) $(TOKEN_O) $(REGS_O) $(INTERPRETER_O) $(STACK_O) $(CPU_O) $(MEMORY_O)

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

$(INTERPRETER_O): $(INTERPRETER_SRC)
	$(CC) -c $< -o $@ $(SASM_FLAGS)

$(STACK_O): $(STACK_SRC)
	$(CC) -c $< -o $@ $(SASM_FLAGS)

$(CPU_O): $(CPU_SRC)
	$(CC) -c $< -o $@ $(SASM_FLAGS)

$(MEMORY_O): $(MEMORY_SRC)
	$(CC) -c $< -o $@ $(SASM_FLAGS)

$(LIBC_LIBSTATIC): $(LIBC_DIR)/$(LIBC_BUILDSCRIPT)
	cd $(LIBC_DIR) && $(RB) $(LIBC_BUILDSCRIPT)

run:
	$(SASM_EXECUTABLE_DIR)

# just a alias
runt: termux_run

termux_setup:
	mkdir -p $(SASM_TEMP)
	cp $(SASM_EXECUTABLE_DIR) $(SASM_TEMP)/$(SASM_EXECUTABLE)
	cp main.sasm $(SASM_TEMP)/main.sasm
	cd $(SASM_TEMP) && chmod +x $(SASM_EXECUTABLE)

termux_run: termux_setup
	cd $(SASM_TEMP) && ./$(SASM_EXECUTABLE) main.sasm

clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(LIBC_DIR)/build/*

gdbt: termux_setup
	cd $(SASM_TEMP) && $(GDB) $(SASM_EXECUTABLE) --args ./$(SASM_EXECUTABLE) main.sasm

debug: CFLAGS += -g
debug: clean all