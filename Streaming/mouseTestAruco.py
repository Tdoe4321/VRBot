import numpy as np
import cv2
import cv2.aruco as aruco

def printHere(event, x, y, flags, params):
    global globalX, globalY
    globalX = x
    globalY = y

cv2.namedWindow("test")
cv2.setMouseCallback("test", printHere)

cap = cv2.VideoCapture(0)

globalX = 0
globalY = 0

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()
    aruco_dict = aruco.Dictionary_get(aruco.DICT_6X6_250)
    parameters =  aruco.DetectorParameters_create()

    '''    detectMarkers(...)
        detectMarkers(image, dictionary[, corners[, ids[, parameters[, rejectedI
        mgPoints]]]]) -> corners, ids, rejectedImgPoints
        '''
        #lists of ids and the corners beloning to each id
    corners, ids, rejectedImgPoints = aruco.detectMarkers(frame, aruco_dict, parameters=parameters)

    frame = aruco.drawDetectedMarkers(frame, corners, ids)

    if(corners):
        polyPoints = np.array(corners[0], 'int32')
        if ((cv2.pointPolygonTest(polyPoints, (globalX,globalY), False)) > 0):
            print("X=" + str(globalX) + " Y=" + str(globalY) )
            cv2.fillConvexPoly(frame, polyPoints, 255)

    # Display the resulting frame
    cv2.imshow("test",frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
