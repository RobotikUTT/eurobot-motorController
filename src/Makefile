### This is the path to where you have created/cloned your project
PROJECT_DIR       = $(realpath ..)

# # PROJECT_DIR       = $(HOME)/dev/arduino/bare-arduino-project

# ### AVR_GCC_VERSION
# ### Check if the version is equal or higher than 4.9
# AVR_GCC_VERSION  := $(shell expr `avr-gcc -dumpversion | cut -f1` \>= 4.9)

# ### ARDMK_DIR
# ### Path to the Arduino-Makefile directory.
ARDMK_DIR         = /usr/local/opt/arduino-mk/

### ARDUINO_DIR
### Path to the Arduino application and ressources directory.
### For Arduino IDE 1.6.x
ARDUINO_DIR       = /Applications/Arduino.app/Contents/Java

# ### USER_LIB_PATH
# ### Path to where the your project's libraries are stored.
USER_LIB_PATH    :=  $(realpath $(PROJECT_DIR)/lib)

### BOARD_TAG & BOARD_SUB
### For Arduino IDE 1.6.x
### Both BOARD_TAG and BOARD_SUB are needed. They must be set to the board you are currently using. (i.e BOARD_TAG = uno, mega, etc. & BOARD_SUB = atmega2560, etc.)
### Note: for the Arduino Uno, only BOARD_TAG is mandatory and BOARD_SUB can be equal to anything
BOARD_TAG        = nano
BOARD_SUB        = atmega328

### MONITOR_BAUDRATE
### It must be set to Serial baudrate value you are using.
MONITOR_BAUDRATE  = 115200

### AVR_TOOLS_DIR
### Path to the AVR tools directory such as avr-gcc, avr-g++, etc.
AVR_TOOLS_DIR     = $(ARDUINO_DIR)/hardware/tools/avr

### AVRDDUDE
### Path to avrdude directory.
AVRDDUDE          = $(ARDUINO_DIR)/hardware/tools/avr/bin/avrdude

### CFLAGS_STD
CFLAGS_STD        = -std=gnu11

### CXXFLAGS_STD
CXXFLAGS_STD      = -std=gnu++11

### CPPFLAGS
### Flags you might want to set for debugging purpose. Comment to stop.
CXXFLAGS          = -pedantic -Wall -Wextra -fdiagnostics-color

### MONITOR_PORT
### The port your board is connected to. Using an '*' tries all the ports and finds the right one.
MONITOR_PORT      = /dev/cu.wchusb*

### OBJDIR
### This is were you put the binaries you just compile using 'make'
CURRENT_DIR       = $(shell basename $(CURDIR))
OBJDIR            = $(PROJECT_DIR)/bin/$(BOARD_TAG)

### path to Arduino.mk, inside the ARDMK_DIR, don't touch.
include $(ARDMK_DIR)/Arduino.mk
