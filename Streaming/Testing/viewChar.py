import zmq
import base64
import numpy as np

context = zmq.Context()
footage_socket = context.socket(zmq.SUB)
footage_socket.setsockopt(zmq.CONFLATE, 1)
#footage_socket.connect('tcp://127.0.0.1:5555')
footage_socket.connect('tcp://192.168.1.110:55556')
#footage_socket.setsockopt_string(zmq.SUBSCRIBE, ''.decode('ascii'))
footage_socket.setsockopt_string(zmq.SUBSCRIBE, '')

while True:
    try:
        frame = footage_socket.recv_string()
        print(frame)

    except KeyboardInterrupt:
        break


