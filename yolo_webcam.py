import cv2
import torch
import serial
from ultralytics import YOLO

ser = serial.Serial('/dev/ttyACM0',9600,timeout=1)
model = YOLO('best.pt')

cap = cv2.VideoCapture(2)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 256)
frame_counter = 0
frame_interval = 8

while cap.isOpened():
    if frame_counter % frame_interval == 0:
        ret, frame = cap.read()
        if not ret:
            break
        frame = cv2.resize(frame,(320,256))
        result = model(frame, imgsz=320)
        classes = result[0].boxes.cls.to("cpu").tolist()
        boxes = result[0].boxes.xywh.to("cpu").tolist()
        
        for box, cls in zip(boxes, classes):
            if int(cls) == 0:
                print(f"{box[0]},{box[1]}")
                send_str = str(box[0])+','+str(box[1])
                ser.write((send_str + '\n').encode())
                print('sent:',send_str)
                #ser.close()
                #exit()
    frame_counter += 1