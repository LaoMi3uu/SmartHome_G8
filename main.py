import cv2
import numpy as np
import dlib
import time
from imutils import face_utils
from imutils.video import FPS

cap = cv2.VideoCapture(0)

detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor("shape_predictor_68_face_landmarks.dat")
fps = FPS().start()
while True:
    _, frame = cap.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    faces = detector(gray)
    for face in faces:

        shape = predictor(gray, face)
        shape = face_utils.shape_to_np(shape)

        for (x, y) in shape[37:47]:
            cv2.circle(frame, (x, y), 4, (255, 0, 0), -1)

    cv2.imshow("Frame", frame)

    key = cv2.waitKey(1)
    if key == 27:
        break

    fps.update()

fps.stop()
print("[INFO] elasped time: {:.2f}".format(fps.elapsed()))
print("[INFO] approx. FPS: {:.2f}".format(fps.fps()))

cv2.destroyAllWindows()