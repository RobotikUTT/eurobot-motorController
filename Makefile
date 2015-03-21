all: motorController unit

motorController:
	make -C ./src

unit:
	make -C ./tests && ./bin/tests/tests

deploy:
	make -C ./src upload

clean:
	make -C ./src clean && make -C ./tests clean