#include "SerialPortManager.h"



SerialPortManager::SerialPortManager(bool bVerboseMode) :
	m_bVerboseMode(bVerboseMode),
	m_bIsBufferSizeCorrect(false), m_bIsBaudRateCorrect(false), m_bIsDataBitsCorrect(false), m_bIsParityCorrect(false), m_bIsStopBitsCorrect(false), m_bIsFlowControlCorrect(false),
	m_bIsBeginKeywordSet(false), m_bIsEndKeywordSet(false), m_bIsValueSeparatorSet(false),
	m_bIsPortFound(false), m_bIsPortConnected(false),
	m_bFirstPortFound(true),
    m_pSerialPort(nullptr),
    m_pElapsedTimer(new QElapsedTimer)
{
	if (nullptr == m_pSerialPort)
	{
		m_pSerialPort = new QSerialPort(this);
		if (m_bVerboseMode)
			qInfo() << "[INFO] <SerialPortManager> QSerialPort created";
	}

	// handles errors
	connect(m_pSerialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
	
	// reads new data as available
	connect(m_pSerialPort, SIGNAL(readyRead()), this, SLOT(readData()));
		
	if (m_bVerboseMode)
		qInfo() << "[INFO] <SerialPortManager> signals/slots connected";
}

SerialPortManager::SerialPortManager() : SerialPortManager(true)
{

}

SerialPortManager::~SerialPortManager()
{
	closeSerialPort();
	
	if (nullptr != m_pSerialPort)
	{
		delete m_pSerialPort;
		m_pSerialPort = nullptr;
	}

    if(nullptr != m_pElapsedTimer)
        delete m_pElapsedTimer;
	
    if (m_bVerboseMode)
		qInfo() << "[INFO] <SerialPortManager> QSerialPort destroyed";
}

void SerialPortManager::setBufferSize(int i32BufferSize)
{
	if (i32BufferSize != 0)
	{
		m_bIsBufferSizeCorrect = true;
		m_pSerialPort->setReadBufferSize(i32BufferSize);
	
		m_i32BufferSize = i32BufferSize;

		if (m_bVerboseMode)
			qInfo() << "[INFO] <SerialPortManager> setBufferSize successfull : " << i32BufferSize;
	}
	else
	{
		m_bIsBufferSizeCorrect = false;
		qCritical() << "[ERROR] <SerialPortManager> Wrong Buffer Size value : " << i32BufferSize;   
	}
}

void SerialPortManager::setBaudRate(int i32BaudRate)
{
	if (	i32BaudRate == 1200 ||
			i32BaudRate == 2400 ||	
			i32BaudRate == 4800 ||
			i32BaudRate == 9600 ||
			i32BaudRate == 19200 ||
			i32BaudRate == 38400 ||
			i32BaudRate == 57600 ||
			i32BaudRate == 115200
	)
	{	
		m_bIsBaudRateCorrect = true;
		m_pSerialPort->setBaudRate(i32BaudRate);
		
		if (m_bVerboseMode)
			qInfo() << "[INFO] <SerialPortManager> setBaudRate successfull : " << i32BaudRate;
	}
	else
	{
		m_bIsBaudRateCorrect = false;
		qCritical() << "[ERROR] <SerialPortManager> Wrong Baud Rate value : " << i32BaudRate;   
	}
	
}

void SerialPortManager::setDataBits(int i32DataBits)
{
	if (	i32DataBits == 5 ||	//QSerialPort::Data5
			i32DataBits == 6 ||	//QSerialPort::Data6
			i32DataBits == 7 ||	//QSerialPort::Data7
			i32DataBits == 8	//QSerialPort::Data8
	)
	{
		m_bIsDataBitsCorrect = true;
		
		switch (i32DataBits)
		{
			case 5:
				m_pSerialPort->setDataBits(QSerialPort::Data5);
				break;
			case 6:
				m_pSerialPort->setDataBits(QSerialPort::Data6);
				break;
			case 7:
				m_pSerialPort->setDataBits(QSerialPort::Data7);
				break;
			case 8:
				m_pSerialPort->setDataBits(QSerialPort::Data8);
				break;
		}
		
		if (m_bVerboseMode)
			qInfo() << "[INFO] <SerialPortManager> setDataBits successfull : " << i32DataBits;
	}
	else
	{
		m_bIsDataBitsCorrect = false;
		qCritical() << "[ERROR] <SerialPortManager> Wrong Data Bits value : " << i32DataBits;   
	}	

}

void SerialPortManager::setParity(int i32Parity)
{
	if (	i32Parity == 0 || 	//QSerialPort::NoParity
			i32Parity == 2 || 	//QSerialPort::EvenParity
			i32Parity == 3 || 	//QSerialPort::OddParity
			i32Parity == 4 || 	//QSerialPort::SpaceParity
			i32Parity == 5		//QSerialPort::MarkParity
	)
	{
		m_bIsParityCorrect = true;

		switch (i32Parity)
		{
		case 0:
			m_pSerialPort->setParity(QSerialPort::NoParity);
			break;
		case 2:
			m_pSerialPort->setParity(QSerialPort::EvenParity);
			break;
		case 3:
			m_pSerialPort->setParity(QSerialPort::OddParity);
			break;
		case 4:
			m_pSerialPort->setParity(QSerialPort::SpaceParity);
			break;
		case 5:
			m_pSerialPort->setParity(QSerialPort::MarkParity);
			break;
		}
			
		if (m_bVerboseMode)
			qInfo() << "[INFO] <SerialPortManager> setParity successfull : " << i32Parity;
	}
	else
	{
		m_bIsParityCorrect = false;
		qCritical() << "[ERROR] <SerialPortManager> Wrong Parity value : " << i32Parity;   
	}
	
}

void SerialPortManager::setStopBits(int i32StopBits)
{
	if (	i32StopBits == 1 || 	//QSerialPort::OneStop
			i32StopBits == 3 || 	//QSerialPort::OneAndHalfStop
			i32StopBits == 2	 	//QSerialPort::TwoStop
	)
	{
		m_bIsStopBitsCorrect = true;
		
		switch (i32StopBits)
		{
		case 1:
			m_pSerialPort->setStopBits(QSerialPort::OneStop);
			break;
		case 2:
			m_pSerialPort->setStopBits(QSerialPort::TwoStop);
			break;
		case 3:
			m_pSerialPort->setStopBits(QSerialPort::OneAndHalfStop);
			break;
		}
				
		if (m_bVerboseMode)
			qInfo() << "[INFO] <SerialPortManager> setStopBits successfull : " << i32StopBits;
	}
	else
	{
		m_bIsStopBitsCorrect = false;
		qCritical() << "[ERROR] <SerialPortManager> Wrong Stop Bits value : " << i32StopBits;  
	}
	
}	
	
void SerialPortManager::setFlowControl(int i32FlowControl)
{
	if (	i32FlowControl == 0 || 	//QSerialPort::NoFlowControl
			i32FlowControl == 1 || 	//QSerialPort::HardwareControl
			i32FlowControl == 2  	//QSerialPort::SoftwareControl
	)
	{
		m_bIsFlowControlCorrect = true;

		switch (i32FlowControl)
		{
		case 0:
			m_pSerialPort->setFlowControl(QSerialPort::NoFlowControl);
			break;
		case 1:
			m_pSerialPort->setFlowControl(QSerialPort::HardwareControl);
			break;
		case 2:
			m_pSerialPort->setFlowControl(QSerialPort::SoftwareControl);
			break;
		}
		
		if (m_bVerboseMode)
			qInfo() << "[INFO] <SerialPortManager> setFlowControl successfull : " << i32FlowControl;
	}
	else
	{
		m_bIsFlowControlCorrect = false;
		qCritical() << "[ERROR] <SerialPortManager> Wrong Flow Control Bits value : " << i32FlowControl;  
	}
	
}		
	

void SerialPortManager::setBufferBeginKeyword(QString sBufferBeginKeyword)
{
	m_sBufferBeginKeyword = sBufferBeginKeyword;
	m_bIsBeginKeywordSet = true;
}

void SerialPortManager::setBufferEndKeyword(QString sBufferEndKeyword)
{
	m_sBufferEndKeyword = sBufferEndKeyword;
	m_bIsEndKeywordSet = true;
}

void SerialPortManager::setValueSeparator(char cValueSeparator)
{
	m_cValueSeparator = cValueSeparator;
	m_bIsValueSeparatorSet = true;
}

void SerialPortManager::setFirstPortFound(bool bFirstPortFound)
{
    m_bFirstPortFound = bFirstPortFound;
}

std::vector<float> SerialPortManager::getDataBuffer() 
{
	std::vector<float> l_vDataBuffer = m_vDataBuffer;
	m_vDataBuffer.clear();
	return l_vDataBuffer;
}

void SerialPortManager::handleError(QSerialPort::SerialPortError error)
{
	if (error == QSerialPort::ResourceError) 
	{
		qCritical() << "[ERROR] <SerialPortManager> Serial Port error : " << m_pSerialPort->errorString();
		closeSerialPort();
	}
}

bool SerialPortManager::openSerialPort()
{
	if (m_bIsPortFound && m_bIsBufferSizeCorrect && m_bIsBaudRateCorrect && m_bIsDataBitsCorrect && m_bIsParityCorrect && m_bIsStopBitsCorrect && m_bIsFlowControlCorrect)
	{
		if (m_pSerialPort->open(QIODevice::ReadWrite))
		{
			m_bIsPortConnected = true;
            m_pElapsedTimer->start();

			if (m_bVerboseMode)
				qInfo() << "[INFO] <SerialPortManager> QSerialPort opened";
		}
		else
		{
			qCritical() << "[ERROR] <SerialPortManager> Serial Port error:" << m_pSerialPort->errorString();
		}
	}
	else
	{
		qCritical() << "[ERROR] <SerialPortManager> Serial Port setup failed: ";
		qCritical() << "--> Port found : " << m_bIsPortFound << ", name :  " << m_pSerialPort->portName() ;
		qCritical() << "--> Buffer size : " << m_bIsBufferSizeCorrect;
		qCritical() << "--> Baud rate : " << m_bIsBaudRateCorrect;
		qCritical() << "--> Data bits : " << m_bIsDataBitsCorrect;
		qCritical() << "--> Parity : " << m_bIsParityCorrect;
		qCritical() << "--> Stop bits : " << m_bIsStopBitsCorrect;
		qCritical() << "--> Flow control : " << m_bIsFlowControlCorrect;

	}

	return m_bIsPortConnected;
}


void SerialPortManager::findSerialPort(QString portName)
{
	QSerialPortInfo portToUse;
	const auto infos = QSerialPortInfo::availablePorts();
	
	for (const QSerialPortInfo &info : infos) 
	{
        if (m_bVerboseMode)
        {
            qInfo() << "##################################################";
            qInfo() << "port found :" << info.description() << " : " << info.manufacturer();
        }


        if (m_bFirstPortFound && (info.description().contains(portName) || info.manufacturer().contains(portName)))
		{
			portToUse = info;
            m_bFirstPortFound = false;
			m_bIsPortFound = true; 
			
            //if (m_bVerboseMode)
			{
				qInfo() << "[INFO] <SerialPortManager> QSerialPort found : ";
				qInfo() << "--> Port: " + info.portName();
				qInfo() << "--> Location: " + info.systemLocation();
				qInfo() << "--> Description: " + info.description();
				qInfo() << "--> Manufacturer: " + info.manufacturer();
				qInfo() << "--> Serial number: " + info.serialNumber();
				qInfo() << "--> Vendor Identifier: " + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString());
				qInfo() << "--> Product Identifier: " + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString());
                // pay attention: info.isBusy() does not work for Bluetooth serial port!
				//qInfo() << QObject::tr("Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";
			}
		}
        else
        {
            m_bFirstPortFound = true;
        }
	}
	
	m_pSerialPort->setPortName(portToUse.portName());
}



void SerialPortManager::closeSerialPort()
{
	m_pSerialPort->close();
	m_pSerialPort->deleteLater();
	
	m_bIsPortConnected = false;

	if (m_bVerboseMode)
		qInfo() << "[INFO] <SerialPortManager> QSerialPort closed and deleted";
}

void SerialPortManager::writeData(const QByteArray data)
{
	if (!m_bIsPortConnected)
	{
		qCritical() << "[ERROR] <SerialPortManager> writeData failure -> port not connected!";
		return;
	}
		
	m_pSerialPort->write(data);
	qDebug() << "[INFO] <SerialPortManager> writing some data : "<< data;



	if (m_bVerboseMode)
		qInfo() << "[INFO] <SerialPortManager> writing some data";
}



void SerialPortManager::readData()
{
    // checks if the serial port is connected before reading new data...
	if (!m_bIsPortConnected)
	{
		qCritical() << "[ERROR] <SerialPortManager> readData failure -> port not connected!";
		return;
	}

	// checks if keywords were set
	if (!m_bIsBeginKeywordSet && !m_bIsEndKeywordSet && !m_bIsValueSeparatorSet)
	{
		qCritical() << "[ERROR] <SerialPortManager> readData failure -> keywords and/or separator not set!";
		return;
	}
	
	// reads all availbale data on the serial port
    QByteArray dataChunk = m_pSerialPort->readAll();
	
    // appends the new chunk to the buffer
	m_oDataBuffer.append(dataChunk);
	
	// looks for starting and ending keywords
	int dataBufferBegin = m_oDataBuffer.indexOf(m_sBufferBeginKeyword, 0);
	int dataBufferEnd = m_oDataBuffer.indexOf(m_sBufferEndKeyword, dataBufferBegin);

	// processes buffer if complete
	if ((dataBufferBegin != -1) && (dataBufferEnd != -1))
	{
		// cuts the buffer if necessary
		QByteArray cutBuffer = m_oDataBuffer.mid(dataBufferBegin, dataBufferEnd - dataBufferBegin);
		
		// splits it into useful values
		QByteArrayList listData = cutBuffer.split(m_cValueSeparator);

		// checks if at least 2 separators were found
		if (listData.count() < 2)
		{
			qCritical() << "[WARNING] <SerialPortManager> readData failure -> not enough data available --> skipped!";
			return;
		}

		// stores values in a vector of floats
		// skips the first and last ones that contain buffer separators
		m_vDataBuffer.clear();
		for (int i = 1; i < listData.count() - 1; i++)
		{
			m_vDataBuffer.push_back(listData[i].toFloat());
		}
		
        // sends the data through a signal
        emit sigBroadcastSignalValues((float)m_pElapsedTimer->elapsed() / 1000.0, m_vDataBuffer);

		// prints values 
        if (m_bVerboseMode)
		{
			qInfo() << "[INFO] <SerialPortManager> data buffer : ";
			for (int i = 0; i < m_vDataBuffer.size(); i++)
				qInfo() << "(" << i << ", " << m_vDataBuffer[i] << " )";
		}
		// removes used data	
		m_oDataBuffer.remove(0, dataBufferEnd + 1);
	}
}

