export PATH=$HOME/rpi-tools/arm-bcm2708/arm-linux-gnueabihf/bin:$PATH
arm-linux-gnueabihf-gcc -march=armv6 -mfpu=vfp -mfloat-abi=hard hello.c -o hello