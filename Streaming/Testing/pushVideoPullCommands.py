import base64
import cv2
import zmq
import serial

context = zmq.Context()
footage_socket = context.socket(zmq.DEALER)
footage_socket.setsockopt(zmq.CONFLATE, 1)
#footage_socket.connect('tcp://192.168.1.141:5555')
footage_socket.bind('tcp://*:55555')
camera = cv2.VideoCapture(0)  # init the camera

#camera.set(3, 640)
#camera.set(4, 480)

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
        grabbed, frame = camera.read()  # grab the current frame
        #frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        #frame = rescale_frame(frame, 80)
        encoded, buffer = cv2.imencode('.jpg', frame)
        #encoded, buffer = cv2.imencode('.jpg', frame, encode_param)
        jpg_as_text = base64.b64encode(buffer)
        footage_socket.send(jpg_as_text)
        msg = footage_socket.recv().decode("utf-8")
        if(not msg):
            print("Garbage")
        else:
            s1.write(str.encode(msg))

    except KeyboardInterrupt:
        camera.release()
        cv2.destroyAllWindows()
        break
