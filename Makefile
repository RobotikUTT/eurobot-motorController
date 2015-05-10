all: motorController unit

motorController:
	make -C ./src

unit:
	make -C ./tests && ./bin/tests/tests

deploy:
	make -C ./src upload

netdeploy: motorController
	scp src/bin/nano/src/src.hex user@bot:.
	ssh user@bot "avrdude -c arduino -b 57600 -P /dev/ttyUSB* -p atmega328p -vv -U flash:w:./src.hex"

clean:
	make -C ./src clean && make -C ./tests clean