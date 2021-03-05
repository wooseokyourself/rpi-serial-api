# Description
Serial Read/Write API for Raspberry Pi with C++. Below is additional offered API using this serial read/write API.
+ BG96 modem (AT command)   

Credit to WiringPi: http://wiringpi.com

# Build
```console
pi@raspberrypi:~/rpi-serial-api $ mkdir build && cd build
pi@raspberrypi:~/rpi-serial-api/build $ cmake ..
pi@raspberrypi:~/rpi-serial-api/build $ make
```

# Run
Run BG96 test code from ```main/test-BG96.cpp```
```console
pi@raspberrypi:~/rpi-serial-api $ ./build/BG96
```
