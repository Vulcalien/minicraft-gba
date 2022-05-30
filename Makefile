# Vulcalien's Executable Makefile
# version 0.1.5
#
# modified to compile GBA ROMs
#
# One Makefile for Unix and Windows
# Made for the 'gcc' compiler

# === DETECT OS ===
ifeq ($(OS),Windows_NT)
	CURRENT_OS := WINDOWS
else
	CURRENT_OS := UNIX
endif

# ========= EDIT HERE =========
OUT_FILENAME := minicraft

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

ifeq ($(CURRENT_OS),UNIX)
	CC      := arm-none-eabi-gcc
	AS      := arm-none-eabi-as
	OBJCOPY := arm-none-eabi-objcopy

	EMULATOR := mgba-qt
else ifeq ($(CURRENT_OS),WINDOWS)
	CC      :=
	AS      :=
	OBJCOPY :=

	EMULATOR :=
endif

CPPFLAGS := -Iinclude -MMD -MP
CFLAGS   := -O3 -fomit-frame-pointer -marm -mcpu=arm7tdmi\
            -Wall -pedantic

LDFLAGS := -Llib -Tlnkscript -nostartfiles
LDLIBS  := -lm
# =============================

OBJ_EXT := .o

ifeq ($(CURRENT_OS),UNIX)
	MKDIR      := mkdir
	MKDIRFLAGS := -p

	RM      := rm
	RMFLAGS := -rfv
else ifeq ($(CURRENT_OS),WINDOWS)
	MKDIR      := mkdir
	MKDIRFLAGS :=

	RM      := rmdir
	RMFLAGS := /Q /S
endif

# === OTHER ===
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%$(OBJ_EXT))

OUT_ELF := $(BIN_DIR)/$(OUT_FILENAME).elf
OUT     := $(BIN_DIR)/$(OUT_FILENAME).gba

# === TARGETS ===
.PHONY: all run build clean

all: build

run:
	$(EMULATOR) ./$(OUT)

build: $(OUT)

clean:
	@$(RM) $(RMFLAGS) $(BIN_DIR) $(OBJ_DIR)

$(OUT): $(OUT_ELF)
	$(OBJCOPY) -O binary $^ $@

$(OUT_ELF): $(OBJ_DIR)/crt0$(OBJ_EXT) $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/crt0$(OBJ_EXT): $(SRC_DIR)/crt0.s | $(OBJ_DIR)
	$(AS) -o $@ $<

$(OBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	$(MKDIR) $(MKDIRFLAGS) "$@"

-include $(OBJ:$(OBJ_EXT)=.d)
