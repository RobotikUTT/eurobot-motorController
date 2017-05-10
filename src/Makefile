PROJECT_DIR      = src

PROJECT_DIR      = .

BOARD_TAG        = nano
BOARD_SUB        = atmega328

MONITOR_PORT     = /dev/cu.wchusb*
ARDUINO_DIR      = /Applications/Arduino.app/Contents/Java
MONITOR_BAUDRATE = 115200
AVR_TOOLS_DIR    = /Applications/Arduino.app/Contents/Java/hardware/tools/avr
AVRDDUDE         = /Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avrdude
CFLAGS_STD       = -std=gnu11
CXXFLAGS_STD     = -std=gnu++11
CXXFLAGS         += -pedantic -Wall -Wextra
CURRENT_DIR      = $(shell basename $(CURDIR))
OBJDIR           = $(PROJECT_DIR)/../bin/$(BOARD_TAG)/$(CURRENT_DIR)
ARDUINO_LIBS     = Wire

include /usr/local/opt/arduino-mk/Arduino.mk