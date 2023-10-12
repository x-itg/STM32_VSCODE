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
-D_BUILDCOMMITID=\"$(BUILDTIME)\"


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
# make 
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
NC = \033[0m
HFILES := $(shell find Inc Drivers/STM32F1xx_HAL_Driver/Inc Drivers/STM32F1xx_HAL_Driver/Inc/Legacy Drivers/CMSIS/Device/ST/STM32F1xx/Include Drivers/CMSIS/Include -type f -name "*.h")
md5current := $(shell md5sum $(BUILD_DIR)/$(TARGET).bin | cut -d ' ' -f 1)
md5lasttim := $(shell md5sum $(BUILD_DIR)/$(TARGET)_backup.bin | cut -d ' ' -f 1)

# git describe --dirty --long --always
readdirty :
	@echo -e "$(YELLOW)The repository dirty has been cleaned up:$(shell git describe --dirty --long --always)$(NC)";
# make first make and cp a backup.bin
 
# make g 先判断_backup.bin是否存在，不存在就先make再cp一个_backup.bin
# 文件，判断.c .h 文件是否发生改变，如果改变就make，make前cp一个
# _backup.bin文件再在make后比对bin文件和_backup.bin文件；bin文件发
# 生变化就打tag 并往远程推送，未改变就不打tag也不推送；
# 判断仓库是否dirty如果dirty则进行推送（此时不打tag）

g:
	@if [ ! -f $(BUILD_DIR)/$(TARGET)_backup.bin ]; then \
	    make -s;\
		cp $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET)_backup.bin; \
		cp $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(COMMIT_INFO).bin; \
		echo -e "$(RED)$(TARGET)_backup.bin absent,make..................$(NC)"; \
		echo -e "$(RED)$(TARGET)_backup.bin absent,NOW CREATING & COPYING$(NC)"; \
	fi; \
	if [ ! -f $(BUILD_DIR)/$(TARGET).bin ]; then \
	    make -s;\
		cp $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET)_backup.bin; \
		cp $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(COMMIT_INFO).bin; \
		echo -e "$(RED)$(TARGET).bin absent,make..................$(NC)"; \
		echo -e "$(RED)$(TARGET).bin absent,NOW CREATING & COPYING$(NC)"; \
	fi; \
	if git diff --quiet --exit-code $(HFILES) && git diff --quiet --exit-code $(C_SOURCES); then \
		echo -e "$(GREEN)No changes in .H and .C files We choose whether to automatically$(NC)"; \
		echo -e "$(GREEN)submit updates to other files based on the repository's status.$(NC)"; \
		echo -e "$(GREEN)code no change LastCommit: $$(git log -1 --pretty=%B)$(NC)"; \
		echo -e "$(GREEN)code no Last Time  MD5SUM: $(md5lasttim)$(NC)"; \
		echo -e "$(GREEN)code no Current    MD5SUM: $(md5current)$(NC)"; \
	else \
		if ! git diff --quiet --exit-code $(HFILES); then \
			echo "$(YELLOW).H files changed$(NC)"; \
		fi; \
		if ! git diff --quiet --exit-code $(C_SOURCES); then \
			echo "$(YELLOW).C files changed$(NC)"; \
		fi; \
		echo -e "$(YELLOW)code update, building$(NC)"; \
		cp $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET)_backup.bin; \
		make -s; \
		echo -e "$(YELLOW)code update, builded$(NC)"; \
		if diff -q $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET)_backup.bin >/dev/null; then \
			echo -e "$(RED)bin no change,Invalid code modification$(NC),$(GREEN)keep dirty$(NC)"; \
			echo -e "$(RED)bin no change,Invalid code modification$(NC),$(GREEN)Current   Commit:$$(git log -1 --pretty=%B)$(NC)"; \
			echo -e "$(RED)bin no change,Invalid code modification$(NC),$(GREEN)Last Time MD5SUM: $(md5lasttim)$(NC)"; \
			echo -e "$(RED)bin no change,Invalid code modification$(NC),$(GREEN)Current   MD5SUM: $(md5current)$(NC)"; \
		else \
		    cp $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(COMMIT_INFO).bin; \
			git add -A; \
			git rm --cached build/*.elf build/*.hex build/*.d build/*.map build/*.o build/*.d build/*.lst; \
			git commit -am "TV"$(BUILDTIME); \
			git tag v2.0.2c_"$(BUILDTIME)"; \
			git push -q origin main; \
			git push origin --tags; \
			echo -e "$(YELLOW)bin changed,code changed$(NC),$(GREEN)  COMMIT PUSH SUCCESSFULL$(NC)"; \
			echo -e "$(YELLOW)bin changed,code changed$(NC),$(GREEN)  BIN   Created: $(COMMIT_INFO).bin$(NC)"; \
			echo -e "$(YELLOW)bin changed,code changed$(NC),$(GREEN)  NEW    Commit: $$(git log -1 --pretty=%B)$(NC)"; \
			echo -e "$(YELLOW)bin changed,code changed$(NC),$(GREEN)LastTime MD5SUM: $(md5lasttim)$(NC)"; \
			echo -e "$(YELLOW)bin changed,code changed$(NC),$(GREEN)Current  MD5SUM: $(md5current)$(NC)"; \
		fi; \
	fi; \
	if [ -n "$(findstring dirty,$(shell git describe --dirty --long --always))" ]; then \
		echo -e "$(YELLOW)The repository is dirty, have to clean:$(shell git describe --dirty --long --always)! $(NC)"; \
		echo -e "$(YELLOW)Now clearing this dirty..............................$(NC)"; \
		echo -e "$(YELLOW)Now Add COMMIT and PUSH to origin and origin.............$(NC)"; \
		git add -A; \
		git rm --cached build/*.elf build/*.hex build/*.d build/*.map build/*.o build/*.d build/*.lst; \
		git commit -am "FV"$(BUILDTIME); \
		echo -e "$(YELLOW)git push -q origin main$(NC)"; \
		git push -q origin main; \
		make readdirty; \
	else \
		echo -e "$(YELLOW)The repository itself is clean:$(shell git describe --dirty --long --always)$(NC)"; \
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