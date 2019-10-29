# VRBot
A VR Controlled Remote-Control Car Project

For a link to the Instructables for a How-To on how to build this, [Click Here]()
To take a closer look at some of the thought behind how I built this, [check out my website](https://tylergragg.com/vrbot-fall-2019/)

### Dependencies
There are two main components here. (1) A 'ground station' computer that needs to have a VR Headset with SteamVR hooked up. (2) An embedded system with an arduino and a usb camera hooked up to it. The following dependencies must be installed on the embedded system.

```bash
pip install numpy
pip install opencv-python
pip install pyzmq
```

### UDEV
To make sure that, no matter what order you plug devices into the embedded system, you can still talk to the arduino, I write some UDEV rules. This basically takes the serial number from the arduino that you have plugged in, and gives it an alias of `arduino_0`. When you do this, just check which `/dev/` port your arduino is on before running the first command. For example, mine was `/dev/ttyUSB1`. You can check this by typing `ls /dev` before and after plugging in your arduino and seeing which `/dev/tty___` comes up.

```
udevadm info -a -n /dev/ttyUSB1 | grep '{serial}' | head -n1
vim /etc/udev/rules.d/99-usb-serial.rules
SUBSYSTEM=="tty", ATTRS{serial}=="VALUE_FROM_ABOVE", SYMLINK+="arduino_0"
```
