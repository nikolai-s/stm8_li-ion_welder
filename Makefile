#######
# makefile for STM8*_StdPeriph_Lib and SDCC compiler
#
# note: paths in this Makefile assume unmodified SPL folder structure
#
# usage:
#   1. if SDCC not in PATH set path -> CC_ROOT
#   2. set correct STM8 device -> DEVICE
#   3. set project paths -> PRJ_ROOT, PRJ_SRC_DIR, PRJ_INC_DIR
#   4. set SPL paths -> SPL_ROOT
#   5. add required SPL modules -> SPL_SOURCE
#   6. add requirmodules  EVAL_128K_SOURCE, EVAL_COMM_SOURCE
#
#######

# STM8 device (for supported devices see stm8s.h)
DEVICE=STM8S103
DEVICE_FLASH=stm8s103f3

# set compiler path & parameters 
CC_ROOT =
CC      = sdcc
CFLAGS  = -mstm8 -lstm8 --opt-code-size

# set output folder and target name
OUTPUT_DIR = ./$(DEVICE)
TARGET     = $(OUTPUT_DIR)/$(DEVICE).hex

# set project folder and files (all *.c)
PRJ_ROOT    = .
PRJ_SRC_DIR = $(PRJ_ROOT)
PRJ_INC_DIR = $(PRJ_ROOT)
PRJ_SOURCE  = $(notdir $(wildcard $(PRJ_SRC_DIR)/*.c))
PRJ_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(PRJ_SOURCE:.c=.rel))

LIB_INC_DIR = /usr/share/sdcc/include/

# set SPL paths
SPL_ROOT    = STM8-SPL-SDCC
SPL_MAKE_DIR = $(SPL_ROOT)/Libraries/STM8S_StdPeriph_Driver
SPL_INC_DIR = $(SPL_ROOT)/Libraries/STM8S_StdPeriph_Driver/inc
SPL_LIB_DIR = $(SPL_ROOT)/Libraries/STM8S_StdPeriph_Driver/$(DEVICE)
SPL_LIB = spl.lib


# collect all include folders
INCLUDE = -I$(PRJ_SRC_DIR) -I$(SPL_INC_DIR) -I$(LIB_INC_DIR)

# collect all source directories
VPATH=$(PRJ_SRC_DIR):$(SPL_SRC_DIR)

.PHONY: clean

all: $(OUTPUT_DIR) $(TARGET) $(SPL_LIB)

$(SPL_LIB):
	$(MAKE) -C $(SPL_MAKE_DIR) DEVICE=$(DEVICE)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

$(OUTPUT_DIR)/%.rel: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -D$(DEVICE) -c $?

$(OUTPUT_DIR)/%.rel: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -D$(DEVICE) -c $? -o $@

$(TARGET): $(PRJ_OBJECTS) $(SPL_LIB)
	$(CC) $(CFLAGS) -o $(TARGET) -L$(SPL_LIB_DIR) -l$(SPL_LIB) $^

clean: 
	rm -fr $(OUTPUT_DIR)
	$(MAKE) -C $(SPL_MAKE_DIR) clean

flash: $(TARGET)
	stm8flash -c stlinkv2 -p $(DEVICE_FLASH) -s flash -w $(TARGET)
