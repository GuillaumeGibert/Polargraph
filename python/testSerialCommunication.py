import sys, getopt
import numpy as np
import cv2
import serial
import time

def openSerialPort(arduino, comPort, baudRate):
	print("--> COM Port = ", comPort)
	print("--> Baud rate = ", baudRate)
	arduino.baudrate = baudRate
	arduino.port = comPort
	try:
		arduino.open()
	except serial.SerialException as e:
		print("[ERROR] ", e)
		return	
	
	time.sleep(2) # give the connection 2s to settle (Arduino board resets)
	
	
def sendContoursToArduinoBoard(arduino, contoursInMm, fps):
	if len(contoursInMm) == 0 :
		print("[ERROR] Contour is empty!")
		return
		
	if arduino.is_open == False:
		print("[ERROR] Arduino board is not connected, nothing will be sent!")
	else:
		stringToSend = "Start\n";
		arduino.write(stringToSend.encode())
	
	for i, contour in enumerate(contoursInMm):# loop over one contour area
		for j, contour_point in enumerate(contour):# loop over the points
			stringToSend = "CurrentPosition:" + str(contour_point[0]) + ";" + str(contour_point[1]) + "\n"
			print("-->Sending: ", stringToSend, end ="")
			if arduino.is_open:
				arduino.write(stringToSend.encode('ascii'))
			
			time.sleep(1./float(fps))
			

def usage():
	print("testSerialCommunication.py -f <fps> -p <comPort> -b <baudRate>")
	

def main(argv):
	# default values (input image filename, fps, comPort, baudRate)
	fps = 0.2
	comPort = 'COM8'
	baudRate = 9600
	
	# parse the command line to look for options
	try:
		opts, args = getopt.getopt(argv,"hf:p:b:")
	except getopt.GetoptError:
		usage()
		sys.exit(2)
	
	for opt, arg in opts:
		if opt == '-h':
			usage()
			sys.exit()
		elif opt in ("-f"):
			fps = arg
		elif opt in ("-p"):
			comPort = arg
		elif opt in ("-b"):
			baudRate = arg
	
	print("=== CONFIG ===")
	print("--> FPS is ", fps)
	print("--> COM port is ", comPort)
	print("--> Baud rate is ", baudRate)
	
	
	# open the serial port
	print("=== OPENING SERIAL PORT ===")
	arduino = serial.Serial()
	openSerialPort(arduino, comPort, baudRate)
	
	#contoursInMm = convertContoursPixel2mm(contours, img_width, img_height)
	contoursInMm = [[[-100, 100],[-100, -100],[100, -100],[100, 100],[-100, 100]]]
	
	# send contours to the Arduino board
	print("=== SENDING CONTOURS ===")
	sendContoursToArduinoBoard(arduino, contoursInMm, fps)
	
	# close serial communication
	print("=== CLOSING SERIAL PORT ===")
	time.sleep(2) # give the connection 2s to finish drawing
	
	if arduino.is_open:
		stringToSend = "Stop\n";
		arduino.write(stringToSend.encode())
		print("--> Stop sending data")
		
		arduino.close()
		print("--> Serial port is closed")
	else:
		print("[WARNING] Serial port was  already closed")
		
	
if __name__ == "__main__":
    main(sys.argv[1:])




