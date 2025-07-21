CC              = gcc
RB              = ruby

BUILD_DIR       = build
LIBC_DIR        = libc
LIBC_LIBSTATIC  = $(LIBC_DIR)/build/libc.a
LIBC_BUILDSCRIPT= build.rb

SASM_ELF        = sasm.elf
SASM_ELF_DIR    = $(BUILD_DIR)/$(SASM_ELF)
SASM_TEMP       = $(HOME)/temp/c/sasm

SASM_FLAGS      = -nostdlib -nodefaultlibs -fno-builtin -Iinclude/ -I$(LIBC_DIR)/include/
LINK_FLAGS      = -nostartfiles

MAIN_SRC        = src/main.c
LEXER_SRC       = src/lexer.c
LEXER_O         = $(BUILD_DIR)/lexer.o
OBJS            = $(LEXER_O)

.PHONY: all setup clean run runt debug

all: setup $(LIBC_LIBSTATIC) $(SASM_ELF_DIR)

setup:
	mkdir -p $(BUILD_DIR)

$(SASM_ELF_DIR): $(MAIN_SRC) $(OBJS)
	$(CC) $(MAIN_SRC) -o $@ $(OBJS) $(SASM_FLAGS) $(LIBC_LIBSTATIC) $(LINK_FLAGS)

$(LEXER_O): $(LEXER_SRC)
	$(CC) -c $< -o $@ $(SASM_FLAGS)

$(LIBC_LIBSTATIC): $(LIBC_DIR)/$(LIBC_BUILDSCRIPT)
	cd $(LIBC_DIR) && $(RB) $(LIBC_BUILDSCRIPT)

run:
	$(SASM_ELF_DIR)

runt:
	mkdir -p $(SASM_TEMP)
	cp $(SASM_ELF_DIR) $(SASM_TEMP)/$(SASM_ELF)
	chmod +x $(SASM_TEMP)/$(SASM_ELF)
	$(SASM_TEMP)/$(SASM_ELF) main.sasm

clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(LIBC_DIR)/build/*

debug: CFLAGS += -g
debug: clean all
