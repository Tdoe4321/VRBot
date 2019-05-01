# VRBot
A VR Controlled Robot Project

### Dependencies
```
pip install numpy
pip install opencv-python
pip install pyzmq
```

### UDEV
```
udevadm info -a -n /dev/ttyUSB1 | grep '{serial}' | head -n1
vim /etc/udev/rules.d/99-usb-serial.rules
SUBSYSTEM=="tty", ATTRS{serial}=="VALUE_FROM_ABOVE", SYMLINK+="arduino_0"
```
