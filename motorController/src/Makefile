################# EDIT BELOW #######################
PROJECT_DIR       = /home/pyms/Documents/Code/eurobot-2015/motorController/src

################# DONT TOUCH #######################
ARDMK_DIR         = $(PROJECT_DIR)/../make
ARDUINO_DIR       = /usr/share/arduino
BOARD_TAG         = mega2560
MONITOR_BAUDRATE  = 9600
AVR_TOOLS_DIR     = /usr/local
AVR_TOOLS_DIR     = /usr
AVRDDUDE          = /usr/bin/avrdude
CFLAGS_STD        = -std=gnu11
CXXFLAGS_STD      = -std=gnu++11
CXXFLAGS         += -pedantic -Wall -Wextra -DTEST_VIRTUAL=""
MONITOR_PORT      = /dev/ttyACM*
CURRENT_DIR       = $(shell basename $(CURDIR))
OBJDIR            = $(PROJECT_DIR)/../bin/$(BOARD_TAG)/$(CURRENT_DIR)

include $(ARDMK_DIR)/Arduino.mk