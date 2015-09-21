# Directories
CURRENT_DIR       = $(shell basename $(CURDIR))
OBJDIR            = $(PROJECT_DIR)/bin/$(CURRENT_DIR)/$(BOARD_TAG)
PROJECT_DIR       = $(CURDIR)
ARDMK_DIR         = $(PROJECT_DIR)/Arduino-Makefile
ARDUINO_DIR       = /usr/share/arduino
USER_LIB_PATH    :=  $(realpath $(PROJECT_DIR)/lib)
AVR_TOOLS_DIR     = /usr
AVRDDUDE          = /usr/bin/avrdude


# Board
BOARD_TAG         = uno
#BOARD_SUB         = atmega2560
MONITOR_BAUDRATE  = 115200
MONITOR_PORT      = /dev/ttyACM0 # Using an '*' tries all the ports and finds the right one.


# Compiler
AVR_GCC_VERSION  := $(shell expr `avr-gcc -dumpversion | cut -f1` \>= 4.9)
CFLAGS_STD        = -std=gnu11
CXXFLAGS_STD      = -std=gnu++11
CXXFLAGS          = -pedantic -Wall -Wextra


### If avr-gcc -v is higher than 4.9, activate coloring of the output
ifeq "$(AVR_GCC_VERSION)" "1"
    CXXFLAGS += -fdiagnostics-color
endif

### path to Arduino.mk, inside the ARDMK_DIR
include $(ARDMK_DIR)/Arduino.mk
