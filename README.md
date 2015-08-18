# Build

I built this without using the Arduino IDE on Linux Mint, so the build
instructions reflect that. This only works on the Arduino Uno.

You need
[Arduino Makefile](https://github.com/sudar/Arduino-Makefile), the
Arduino toolchain, and pySerial.

```sh
sudo apt-get install arduino arduino-mk
pip install pyserial
```

Connect your Arduino to a USB port and get ownership of the port:

```sh
sudo chown $USER /dev/ttyACM0
```

To build and upload the code to your Arduino, all you need to do is:

```sh
make -k upload
```
