default: ci

ci:
	PLATFORMIO_CI_SRC=. platformio ci --board=uno

run:
	platformio run

upload:
	platformio run -v -t upload

monitor:
	platformio serialports monitor
