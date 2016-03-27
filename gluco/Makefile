default: ci

ci:
	PLATFORMIO_CI_SRC=. platformio ci --board=uno

run:
	platformio run -vvv

upload:
	platformio run -vvv -t upload

monitor:
	platformio serialports monitor -b 115200
