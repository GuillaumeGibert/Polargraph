#include <QApplication>

#include "SerialPortManager.h"
#include "MainWindow.h"
#include "Spy.h"
#include "Config.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	//=======SERIAL PORT=========
	// creates a serial port manager object
	SerialPortManager spm(false);

	// sets serial port features
	spm.setBufferSize(serialPortBufferSize);
	spm.setBaudRate(serialPortBaudRate);
	spm.setDataBits(serialPortDataBits);
	spm.setParity(serialPortParity);
	spm.setStopBits(serialPortStopBits);
	spm.setFlowControl(serialPortFlowControl);
	spm.setBufferBeginKeyword(serialPortBeginKeyword);
	spm.setBufferEndKeyword(serialPortEndKeyword);
	spm.setValueSeparator(serialPortValueSeparator);

	// finds the serial port
	spm.findSerialPort("Arduino");

	// opens the serial port
	spm.openSerialPort();

	// at this point, the spm object is sending data through its sigBroadcastSignalValues signal
	
	//=======MainWindow=========
	// creates the Main Window
	MainWindow window;
	// shows the main window
	window.show();
	
	//=======SPY=========
	// creates a spy object
	Spy spy;
			
	//=======Qt CONNECTIONS=========
	QObject::connect(&window, SIGNAL(sigSendArduinoCommand(const QByteArray)), &spm,    SLOT(writeData(const QByteArray)));
	
	QObject::connect(&spm, SIGNAL(sigBroadcastSignalValues(float, std::vector<float>)), &spy,    SLOT(setSignalValues(float, std::vector<float>)));
       
       
	return app.exec();
	
}