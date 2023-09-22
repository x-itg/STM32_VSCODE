##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.18.0-B7] date: [Thu Mar 02 20:37:47 CST 2023]
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = EC20103RB
PROJECT= EC20103RB

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -O0
COMMIT_COUNT := $(shell git rev-list --count HEAD)
BUILDTIME := $(shell date +'-%Y%m%d_%H%M%S') 
BUILDTIME := $(shell git describe --dirty --long --always)
BUILDTIME := $(shell echo "$(BUILDTIME)" | tr -d ' ')_$(COMMIT_COUNT)
COMMIT_TIME := $(shell git show -s --format=%ct HEAD)
COMMIT_ID := $(shell git rev-parse HEAD)
COMMIT_ID_ABBREV := $(shell git rev-parse --short HEAD)
READABLE_TIME := $(shell date -d @$(COMMIT_TIME) +"%Y%m%d_%H%M%S")
COMMIT_INFO := $(READABLE_TIME)_$(COMMIT_ID_ABBREV)_$(COMMIT_COUNT)
COMMIT_INFO := $(shell echo "$(COMMIT_INFO)" | tr -d ' ')
BUILDTIME := $(COMMIT_INFO)

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
Src/main.c \
Src/oled.c \
Src/radarandec20.c \
Src/u1rs232.c \
Src/u2ec20.c \
Src/u3radar.c \
Src/stm32f1xx_it.c \
Src/stm32f1xx_hal_msp.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
Src/system_stm32f1xx.c  

# ASM sources
ASM_SOURCES =  \
startup_stm32f103xb.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F103xB \
-DBUILDCOMMITID=\"$(BUILDTIME)\"


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-IInc \
-IDrivers/STM32F1xx_HAL_Driver/Inc \
-IDrivers/STM32F1xx_HAL_Driver/Inc/Legacy \
-IDrivers/CMSIS/Device/ST/STM32F1xx/Include \
-IDrivers/CMSIS/Include


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F103RBTx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
default_language:=zh-CN
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin
dif:
	diff $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET)_backup.bin -a
git: 
	@if [ -n "$(findstring dirty,$(shell git describe --dirty --long --always))" ]; then \
		echo -e "$(YELLOW)code update, building$(NC)"; \
		cp $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET)_backup.bin; \
		make -s; \
		echo -e "$(YELLOW)code update, builded$(NC)"; \
		if diff -q $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET)_backup.bin >/dev/null; then \
			echo -e "$(RED)bin no change,code changed,keep dirty$(NC)"; \
			echo -e "$(RED)current commit:$$(git log -1 --pretty=%B)$(NC)"; \
		else \
			echo -e "$(GREEN)bin changed created:$(COMMIT_INFO).bin$(NC)"; \
			cp $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(COMMIT_INFO).bin; \
			git add .; \
			git commit -am $(BUILDTIME); \
			echo -e "$(YELLOW)new commit:$$(git log -1 --pretty=%B)$(NC)"; \
			git push -q origin master; \
			echo -e "$(GREEN)bin changed,code changed,commit and push success$(NC)"; \
		fi; \
	else \
		echo -e "$(GREEN)code no change commit:$$(git log -1 --pretty=%B)$(NC)"; \
	fi;
#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# Program
upload: build/$(PROJECT).bin
	openocd -f board/stm32f1discovery.cfg -c "reset_config trst_only combined" -c "program build/$(PROJECT).elf verify reset exit"

debug-start:
	openocd -f stm32-bv_openocd.cfg

update:
	openocd -f openocd.cfg -c init -c halt -c "program $(BUILD_DIR)/$(TARGET).hex verify reset exit"
reset:
	openocd -f openocd.cfg -c init -c halt -reset -c shutdown
# *** EOF ***