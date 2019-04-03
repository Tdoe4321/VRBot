import base64
import cv2
import zmq
import zlib

context = zmq.Context()
footage_socket = context.socket(zmq.PUB)
footage_socket.setsockopt(zmq.CONFLATE, 1)
#footage_socket.connect('tcp://192.168.1.141:5555')
footage_socket.bind('tcp://*:5555')
camera = cv2.VideoCapture(0)  # init the camera

#camera.set(3, 640)
#camera.set(4, 480)

def rescale_frame(frame, percent=75):
    width = int(frame.shape[1] * percent/ 100)
    height = int(frame.shape[0] * percent/ 100)
    dim = (width, height)
    return cv2.resize(frame, dim, interpolation =cv2.INTER_AREA)

while True:
    try:
        grabbed, frame = camera.read()  # grab the current frame
        #frame = rescale_frame(frame, 1000)
        encoded, buffer = cv2.imencode('.jpg', frame)
        jpg_as_text = base64.b64encode(zlib.compress(buffer, 6))
        footage_socket.send(jpg_as_text)

    except KeyboardInterrupt:
        camera.release()
        cv2.destroyAllWindows()
        break
