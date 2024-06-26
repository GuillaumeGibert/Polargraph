QT +=  widgets serialport

INCLUDEPATH += include
INCLUDEPATH += <path_to_opencv>\build\install\include
LIBS += -L<path_to_opencv>\build\install\x64\vc16\lib \
        -lopencv_core440 -lopencv_imgproc440 -lopencv_highgui440 -lopencv_imgcodecs440
 
HEADERS = 	include/SerialPortManager.h \
			include/MainWindow.h \
                        include/ImageDisplay.h \
			include/ImageProcessor.h \
                        include/Spy.h \
                        include/ArduinoController.h \
                        include/Config.h \
                        include/mat_and_qimage.hpp
			
		
SOURCES = 	src/SerialPortManager.cpp \
			src/MainWindow.cpp \
                        src/ImageDisplay.cpp \
			src/ImageProcessor.cpp \
                        src/Spy.cpp \
                        src/ArduinoController.cpp \
                        src/mat_and_qimage.cpp \
		        src/main.cpp
   
                
