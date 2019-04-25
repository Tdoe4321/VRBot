import base64
import zmq
import time

context = zmq.Context()
footage_socket = context.socket(zmq.PUB)
footage_socket.setsockopt(zmq.CONFLATE, 1)
#footage_socket.connect('tcp://192.168.1.141:5555')
footage_socket.bind('tcp://*:55556')
#camera = cv2.VideoCapture(0)  # init the camera

#camera.set(3, 640)
#camera.set(4, 480)

while True:
    try:
        #text = base64.b64encode("hello world")
        footage_socket.send_string("Hello World")
        time.sleep(1)

    except KeyboardInterrupt:
        break
