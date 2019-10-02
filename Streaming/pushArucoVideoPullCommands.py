import base64
import cv2
import cv2.aruco as aruco
import zmq
import serial
import numpy as np

''' For mouse purposes only
# Aruco Markers visual capture
def getMouseLocation(event, x, y, flage, params):
    global globalX, globalY
    globalX = x
    globalY = y

globalX = 0
globalY = 0
'''

aruco_dict = aruco.Dictionary_get(aruco.DICT_6X6_250)
parameters =  aruco.DetectorParameters_create()


# ZeroMQ Setup
context = zmq.Context()
footage_socket = context.socket(zmq.DEALER)
footage_socket.setsockopt(zmq.CONFLATE, 1)
footage_socket.bind('tcp://*:55555')
camera = cv2.VideoCapture(0)  # init the camera

#camera.set(3, 640)
#camera.set(4, 480)

# Serial Connection to arduino
port = "/dev/arduino_0"
rate = 115200
s1 = serial.Serial(port, rate)
s1.baudrate = rate

msg = None

encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 85]

def rescale_frame(frame, percent=75):
    width = int(frame.shape[1] * percent/ 100)
    height = int(frame.shape[0] * percent/ 100)
    dim = (width, height)
    return cv2.resize(frame, dim, interpolation =cv2.INTER_AREA)

while True:
    try:
        # Grab the current frame
        grabbed, frame = camera.read()
        
        # See if there are any markers in it and draw them
        corners, ids, rejectedImgPoints = aruco.detectMarkers(frame, aruco_dict, parameters=parameters)
        frame = aruco.drawDetectedMarkers(frame, corners, ids)
        
        # Encode frame into buffer
        #frame = rescale_frame(frame, 80)
        encoded, buffer = cv2.imencode('.jpg', frame)
        #encoded, buffer = cv2.imencode('.jpg', frame, encode_param)
        jpg_as_text = base64.b64encode(buffer)
        
        # Send the image out over ZMQ
        footage_socket.send(jpg_as_text)

        # If we receive a message, pipe it thought serial 
        msg = footage_socket.recv().decode("utf-8")
        if(not msg):
            print("Garbage")
        else:
            s1.write(str.encode(msg))

    except KeyboardInterrupt:
        camera.release()
        cv2.destroyAllWindows()
        break
