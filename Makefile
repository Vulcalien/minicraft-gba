# Vulcalien's GBA Makefile
#
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

SRC_SUBDIRS := scene images entity

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

ASFLAGS  := -mcpu=arm7tdmi

LDFLAGS := -Tlnkscript -nostartfiles
LDLIBS  :=
# =============================

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
SRC := $(wildcard $(SRC_DIR)/*.c)\
       $(foreach DIR,$(SRC_SUBDIRS),$(wildcard $(SRC_DIR)/$(DIR)/*.c))
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

OUT_ELF := $(BIN_DIR)/$(OUT_FILENAME).elf
OUT     := $(BIN_DIR)/$(OUT_FILENAME).gba

OBJ_DIRECTORIES := $(OBJ_DIR) $(foreach DIR,$(SRC_SUBDIRS),$(OBJ_DIR)/$(DIR))

# === TARGETS ===
.PHONY: all run build res clean release

all: build

run:
	$(EMULATOR) ./$(OUT)

build: $(OUT)

clean:
	@$(RM) $(RMFLAGS) $(BIN_DIR) $(OBJ_DIR) src/res release

$(OUT): $(OUT_ELF)
	$(OBJCOPY) -O binary $^ $@

$(OUT_ELF): $(OBJ_DIR)/crt0.o $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/crt0.o: $(SRC_DIR)/crt0.s | $(OBJ_DIRECTORIES)
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIRECTORIES)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIRECTORIES) src/res src/res/sounds:
	$(MKDIR) $(MKDIRFLAGS) "$@"

res: src/res src/res/sounds
	scripts/convert-resources.py res/resources.json

release:
	make clean && make res && make
	truncate -s %128K $(OUT) && truncate -s -42 $(OUT)
	echo -n "Jeanne, Seigneur, est ton oeuvre splendide" >> $(OUT)
	mkdir release release/checksum
	cp $(OUT) COPYING README.md release/
	git clone `git remote get-url origin` release/source
	git -C release/source gc --aggressive --prune=now
	cd release && md5sum $(OUT_FILENAME).gba > checksum/$(OUT_FILENAME).gba.md5
	cd release && md5sum-recursive source > checksum/source.md5
	cd release && zip -r release.zip\
        $(OUT_FILENAME).gba COPYING README.md source checksum

-include $(OBJ:.o=.d)
