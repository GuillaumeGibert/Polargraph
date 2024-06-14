import sys, getopt
import numpy as np
import cv2
import serial
import time

def openSerialPort(arduino, comPort, baudRate):
	arduino.baudrate = baudRate
	arduino.port = comPort
	
	print("--> COM Port = ", arduino.port)
	print("--> Baud rate = ", arduino.baudrate)
	#print("--> bytesize = ", arduino.bytesize)
	#print("--> parity = ", arduino.parity)
	#print("--> stopbits = ", arduino.stopbits)
	#print("--> timeout = ", arduino.timeout)
	#print("--> xonxoff = ", arduino.xonxoff)
	#print("--> rtscts = ", arduino.rtscts)
	#print("--> dsrdtr = ", arduino.dsrdtr)
	#print("--> write_timeout = ", arduino.write_timeout)
	#print("--> inter_byte_timeout = ", arduino.inter_byte_timeout)
	try:
		arduino.open()
	except serial.SerialException as e:
		print("[ERROR] ", e)
		return	
	
	time.sleep(2) # gives the connection 2s to settle (Arduino board resets)
	
	
def processImage(filename, threshold):
	# loads the original image
	img_orig = cv2.imread(filename)
	# checks if the image is not empty
	if img_orig is None:
		print("[ERROR] Cannot read the input image: ", filename)
		return [], 0, 0
	# gets image characteristics	
	img_width = np.shape(img_orig)[1]
	img_height = np.shape(img_orig)[0]
	print("--> Image size = (", img_width, ", ", img_height, ")")
	cv2.imshow('Original image', img_orig)
	#cv2.imwrite('output/img_orig.jpg', img_orig)
	# converts it to grayscale
	print("--> RGB to Gray")
	img_gray = cv2.cvtColor(img_orig, cv2.COLOR_BGR2GRAY)
	cv2.imshow('Grayscale image', img_gray)
	#cv2.imwrite('output/img_gray.jpg', img_gray)
	# applies thresholding
	print("--> Thresholding")
	ret, img_thresh = cv2.threshold(img_gray, int(threshold), 255, cv2.THRESH_BINARY)
	cv2.imshow('Binary image', img_thresh)
	#cv2.imwrite('output/img_thresh.jpg', img_thresh)
	# detects contours
	print("--> Contour detection")
	contours, hierarchy = cv2.findContours(image=img_thresh, mode=cv2.RETR_TREE, method=cv2.CHAIN_APPROX_NONE)
	# draws contours
	print("--> Drawing contours")
	img_orig_copy = img_orig.copy()
	cv2.drawContours(image=img_orig_copy, contours=contours, contourIdx=-1, color=(0, 255, 0), thickness=2, lineType=cv2.LINE_AA)
	cv2.imshow('Contour detection', img_orig_copy)
	#cv2.imwrite('output/img_contour.jpg', img_orig_copy)
	
	print("--> Press any key to start sending the contour infos...")
	cv2.waitKey(0)
		
	# return the contours and the image size
	return contours, img_width, img_height
	
	
def convertContoursPixel2mm(contoursInPixel, img_width, img_height, scalingFactor):
	# checks if the contour is not empty
	if len(contoursInPixel) == 0:
		print("[ERROR] Contour is empty!")
		return []
	
	# prints contour infos 
	print("-->There is/are ",  len(contoursInPixel), " contour(s) detected in this image")
	
	# converts contour from tuple to list if necessary
	# see https://github.com/opencv/opencv/issues/21266
	if type(contoursInPixel) is tuple:
		contoursInPixel = list(contoursInPixel)
		
	# creates an array to store the contours in mm
	contoursInMm = contoursInPixel.copy()
	
	# creates the transformation matrix
	transformationMatrix = np.array([[0., 1., -img_width/2.0], [-1., 0., img_height/2.0], [0., 0., 1.]])
	
	
	# retrieves the point coordinates in pixel and convert them in mm
	for i, contour in enumerate(contoursInPixel): # loop over one contour area
		print("-->There are ",  len(contour), " points in the contour #", i+1)
		for j, contour_point in enumerate(contour): # loop over the points
			currentPointInPixel = np.array([ [contour_point[0][1]], [contour_point[0][0]], [1.0] ])
			currentPointInMm = np.matmul(transformationMatrix, currentPointInPixel);
			
			print("-->pixel: (" , currentPointInPixel[0], ",", currentPointInPixel[1], ") -->mm: (", currentPointInMm[0], ",", currentPointInMm[1], ")")
			
			# stores coordinates in mm into a list
			contoursInMm[i][j] = np.transpose([currentPointInMm[0]/float(scalingFactor) , currentPointInMm[1]/float(scalingFactor)]);
		
	return contoursInMm
	
def sendContoursToArduinoBoard(arduino, contoursInMm, fps, samplingFactor):
	if len(contoursInMm) == 0 :
		print("[ERROR] Contour is empty!")
		return
		
	if arduino.is_open == False:
		print("[ERROR] Arduino board is not connected, nothing will be sent!")
	else:
		stringToSend = "Start\n";
		try:
			arduino.write(stringToSend.encode())
		except serial.SerialException as e:
			print("[ERROR] ", e)
			return	
	print(contoursInMm)
	
	servoFlag = 0
	
	for i, contour in enumerate(contoursInMm):# loop over one contour area
		servoFlag = 0
		for j, contour_point in enumerate(contour):# loop over the points
			if j%int(samplingFactor) == 0:
				stringToSend = "Data:" + str(contour_point[0][0]) + ";" + str(contour_point[0][1]) + "," + str(servoFlag) + "\n"
				print("-->Sending: ", stringToSend, end ="")
				servoFlag = 1
				if arduino.is_open:
					try:
						arduino.write(stringToSend.encode('ascii'))
					except serial.SerialException as e:
						print("[ERROR] ", e)
						return
				time.sleep(1./float(fps))
			

def usage():
	print("polargraph.py -i <inputfile> -f <fps> -p <comPort> -b <baudRate> -s <scaling_factor> -c <contour_sampling> -t <threshold>")
	

def main(argv):
	# sets default values (input image filename, fps, comPort, baudRate, threshold)
	inputFilename = '../images/python-logo.png'
	fps = 5.0
	comPort = 'COM8'
	baudRate = 115200
	scalingFactor = 1
	samplingFactor = 1
	threshold = 80
	
	# parses the command line to look for options
	try:
		opts, args = getopt.getopt(argv,"hi:f:p:b:s:c:t:")
	except getopt.GetoptError:
		usage()
		sys.exit(2)
	
	for opt, arg in opts:
		if opt == '-h':
			usage()
			sys.exit()
		elif opt in ("-i"):
			inputFilename = arg
		elif opt in ("-f"):
			fps = arg
		elif opt in ("-p"):
			comPort = arg
		elif opt in ("-b"):
			baudRate = arg
		elif opt in ("-s"):
			scalingFactor = arg
		elif opt in ("-c"):
			samplingFactor = arg
		elif opt in ("-t"):
			threshold = arg
	
	print("=== CONFIG ===")
	print("--> Input file is ", inputFilename)
	print("--> FPS is ", fps)
	print("--> COM port is ", comPort)
	print("--> Baud rate is ", baudRate)
	print("--> Scaling factor is ", scalingFactor)
	print("--> Sampling factor is ", samplingFactor)
	print("--> Threshold factor is ", threshold)
	
	# opens the serial port
	print("=== OPENING SERIAL PORT ===")
	arduino = serial.Serial(bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=0.01, xonxoff=False, rtscts=False, dsrdtr=False, write_timeout=0.01, inter_byte_timeout=0.01)
	#https://pyserial.readthedocs.io/en/latest/pyserial_api.html#serial.Serial
	openSerialPort(arduino, comPort, baudRate)
		
	# performs image processing
	print("=== IMAGE PROCESSING ===")
	contours, img_width, img_height = processImage(inputFilename, threshold)
    
	# converts pixel to mm
	print("=== CONTOUR CONVERSION ===")
	contoursInMm = convertContoursPixel2mm(contours, img_width, img_height, scalingFactor)
	
	# sends contours to the Arduino board
	print("=== SENDING CONTOURS ===")
	sendContoursToArduinoBoard(arduino, contoursInMm, fps, samplingFactor)
	
	# closes serial communication
	print("=== CLOSING SERIAL PORT ===")
	time.sleep(2) # give the connection 2s to finish drawing
	
	if arduino.is_open:
		stringToSend = "Stop\n";
		try:
			arduino.write(stringToSend.encode())
		except serial.SerialException as e:
			print("[ERROR] ", e)
			return
		print("--> Stop sending data")
		
		arduino.close()
		print("--> Serial port is closed")
	else:
		print("[WARNING] Serial port was  already closed")
		
	# closes opencv windows
	cv2.destroyAllWindows()
	

if __name__ == "__main__":
    main(sys.argv[1:])




