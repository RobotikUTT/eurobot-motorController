all: clampController unit

clampController:
	make -C ./src

unit:
	make -C ./tests && ./bin/tests/tests

deploy:
	make -C ./src upload
netdeploy:clampController
	scp src/bin/nano/src/src.hex user@bot:.
	ssh user@bot "avrdude -c arduino -b 57600 -P /dev/ttyUSB* -p atmega328p -vv -U flash:w:./src.hex" 
netwatch:
	ssh user@bot "picocom -b 115200 -l /dev/ttyUSB*"
clean:
	make -C ./src clean && make -C ./tests clean
