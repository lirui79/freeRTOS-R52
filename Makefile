#-----------------------------------------------------------------------------
# The confidential and proprietary information contained in this file may
# only be used by a person authorised under and to the extent permitted
# by a subsisting licensing agreement from ARM Limited.
#
#            (C) COPYRIGHT 2004-2011 ARM Limited.
#                ALL RIGHTS RESERVED
#
# This entire notice must be reproduced on all copies of this file
# and copies of this file may only be made by a person if such person is
# permitted to do so under the terms of a subsisting license agreement
# from ARM Limited.
#
#      SVN Information
#
#      Checked In          : $Date: 2010-10-12 11:59:41 +0100 (Tue, 12 Oct 2010) $
#
#      Revision            : $Revision: 150631 $
#
#      Release Information : Cortex-R5-AT570-r1p1-00rel0
#
#-------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Purpose : Makefile for the Cortex-R5 Integration Kit
#
#-----------------------------------------------------------------------------

# Clear suffix list for implicit rules
.SUFFIXES :

#include cfg/Makefile.cfg

ROOT    = .
TARGET  = Freertos

INCLUDES := \
  . \
  ./include \
  freertos/include \
  freertos/portable/GCC/UVMM \
  vcodec  \
  mailbox


ENDIAN = --littleend

CORE=cortex-r52

COMPILER := gcc

GCC_TOOLCHAIN           := arm-none-eabi
GCC_ASM                 := $(GCC_TOOLCHAIN)-as
GCC_CC                  := $(GCC_TOOLCHAIN)-gcc
GCC_LINKER              := $(GCC_TOOLCHAIN)-ld
GCC_ELF2BIN             := $(GCC_TOOLCHAIN)-objcopy
GCC_DISASS              := $(GCC_TOOLCHAIN)-objdump

CPPFLAGS := $(addprefix -I,$(INCLUDES)) -MMD -MP -D__FREERTOS__  -DSUPPORT_DBGFS  -DVCMD_DEBUG_INTERNAL -DSUPPORT_WATCHDOG -DIRQ_SIMULATION
#CPPFLAGS := $(addprefix -I,$(INCLUDES)) -MMD -MP -D__FREERTOS__

#-mfloat-abi=soft  -mfpu=none -marm -mno-thumb-interwork -mabi=aapcs-linux -mno-unaligned-access
GCC_CC_FLAGS            :=  -v -Og -g -std=gnu99 -mthumb -mcpu=$(CORE) -mfloat-abi=hard -mfpu=vfpv3-d16 \
							-fno-builtin-printf -fomit-frame-pointer -fno-strict-aliasing -fno-builtin-memcpy \
							-Wall -Wextra
GCC_AS_FLAGS            :=  -v -g -mcpu=$(CORE) -mfpu=vfpv3-d16 -x assembler-with-cpp
GCC_CC_LDFLAGS          :=  -verbose -nostdlib -static --no-warn-rwx-segments
GCC_LINKER_OPTS_ASM     :=
GCC_LINKER_FILE         :=  -T $(ROOT)/link.ld
GCC_DISASS_OPTS         :=  --disassemble-all --disassemble-zeroes
GCC_MICRO               :=

ARMASM                  = armasm 
ARMLINK                 = armlink
ARMCC                   = armcc
FROMELF                 = fromelf

# Define the path to look for included files, i.e. Macros.s
ASINC = -I include

#convert bin to hex
BIN2HEX = ./bin2hex

# Build directories
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

#A78_BIN = ./firmware/a78_code.o

# Find all source directories (max depth 5)
SRCDIRS = $(shell find $(ROOT) -maxdepth 5 -type d)

# Find all source files
SRCCS = $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
SRCSS = $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.s))

# Generate object file list
OBJS = $(patsubst $(ROOT)/%,$(OBJ_DIR)/%.o,$(basename $(SRCCS) $(SRCSS)))
#OBJS += ./firmware/a78.o

# Final targets
ELF_TARGET = $(BUILD_DIR)/$(TARGET).elf
MAPFILE    = $(BUILD_DIR)/$(TARGET).map
BIN_TARGET = $(BIN_DIR)/$(TARGET).bin
HEX_TARGET = $(BIN_DIR)/$(TARGET).hex

# Run ARM assembler
#@echo "TESTFLAGS: $(TESTFLAGS)"
#@echo "ENDIAN: $(ENDIAN)"
#@echo "DHRYASFLAGS: $(DHRYASFLAGS)"
$(info TESTFLAGS=$(TESTFLAGS) DHRYASFLAGS=$(DHRYASFLAGS) ENDIAN=$(ENDIAN))

COMPILER_INT := $(strip $(COMPILER))
ifeq ($(COMPILER_INT), gcc)
  #ENDIAN = -EL
  AS 				:= $(GCC_ASM)
  CC                := $(GCC_CC)
  LD				:= $(GCC_LINKER)
  OBJCOPY 			:= $(GCC_ELF2BIN) -O binary -R .comment -R .debug*
  ASFLAGS           := $(GCC_AS_FLAGS)
  CFLAGS			:= $(GCC_CC_FLAGS)
  LDFLAGS			:= $(GCC_CC_LDFLAGS) -Map=$(MAPFILE)
  LINKER_OPTS_ASM 		:= $(GCC_LINKER_OPTS_ASM)
  LINKER_FILE     		:= $(GCC_LINKER_FILE)
  DISSAMBLER      		:= $(GCC_DISASS)
  DISSAMBLER_OPTS 		:= $(GCC_DISASS_OPTS)
else
  AS = $(ARMASM) $(ASFLAGS) $(DHRYASFLAGS) $(TESTFLAGS) $(ASINC) --unsafe --apcs /inter --width 132 --length 0 $(ENDIAN)
endif

$(info AS=$(AS) CC=$(CC))

# Generate memory image
all : $(HEX_TARGET)

# Create build directories
$(OBJ_DIR) $(BIN_DIR):
	@mkdir -p $@
	
# Function to get target directory for object file
obj_dir = $(OBJ_DIR)/$(dir $(patsubst $(ROOT)/%,%,$1))

# Compile C files
$(OBJ_DIR)/%.o: $(ROOT)/%.c | $(OBJ_DIR)
	@mkdir -p $(call obj_dir,$<)
	@echo "CC  $<"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Compile assembly files  
$(OBJ_DIR)/%.o: $(ROOT)/%.s | $(OBJ_DIR)
	@mkdir -p $(call obj_dir,$<)
	@echo "AS  $<"	
	$(CC) -c $(CPPFLAGS) $(ASFLAGS) $< -o $@
	#@$(AS) $(ASFLAGS) $< -o $@

# Link ELF file
$(ELF_TARGET): $(OBJS) | $(BUILD_DIR)
	@echo "LD  $@"
	#@$(LD) $(LDFLAGS) $(LINKER_FILE) $(OBJS) -o $@
	#@$(LD) $(LDFLAGS) $(LINKER_FILE) ./firmware/a78_code.o $(OBJS) -o $@
	@$(CC) $(CFLAGS) -nostdlib $(LINKER_FILE) -static -Wl,-Map=$@.map $(OBJS) -o $@
	@$(DISSAMBLER) $(DISSAMBLER_OPTS) $@ > $(BUILD_DIR)/$(TARGET).disass	
	
# Generate BIN file from ELF
$(BIN_TARGET): $(ELF_TARGET) | $(BIN_DIR)
	@echo "OBJCOPY  $@"
	@$(OBJCOPY) -O binary $< $@
	
# Generate HEX file from BIN
$(HEX_TARGET): $(BIN_TARGET)
	@echo "BIN2HEX  $@"
	@$(BIN2HEX) $< $(basename $@)36.hex 0 32
	@$(BIN2HEX) $< $(basename $@).hex 0 64

# Clean build artifacts
clean:
	@rm -rf $(BUILD_DIR)

# Print debug information
print-%:
	@echo '$*=$($*)'
	
# Show source files info
info:
	@echo "Source directories:"
	@echo "$(SRCDIRS)"
	@echo "C source files:"
	@echo "$(SRCCS)"
	@echo "Assembly source files:"
	@echo "$(SRCSS)"
	@echo "Object files:"
	@echo "$(OBJS)"
