#ifndef SERIAL_PORT_MANAGER_H
#define SERIAL_PORT_MANAGER_H

#include <QtCore/QtGlobal>
#include <QObject>
#include <QDebug>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QElapsedTimer>

class SerialPortManager : public QObject
{
	Q_OBJECT

	public:
		SerialPortManager();
		SerialPortManager(bool bVerboseMode);
		~SerialPortManager();

	public:
        void findSerialPort(QString);
		bool openSerialPort();
		void closeSerialPort();
	
		void setBufferSize(int);
		void setBaudRate(int);
		void setDataBits(int);
		void setParity(int);
		void setStopBits(int);
		void setFlowControl(int);
		void setBufferBeginKeyword(QString);
		void setBufferEndKeyword(QString);
		void setValueSeparator(char);
        void setFirstPortFound(bool);

		std::vector<float> getDataBuffer();

    public slots:
        void writeData(const QByteArray data);
		void readData();
		void handleError(QSerialPort::SerialPortError error);
	
	signals:
    void sigBroadcastSignalValues(float, std::vector<float>);
		
	private:
        QSerialPort* m_pSerialPort;
		bool m_bFirstPortFound;
		QByteArray m_oDataBuffer;
		bool m_bVerboseMode;
	
		bool m_bIsBufferSizeCorrect;
		bool m_bIsBaudRateCorrect;
		bool m_bIsDataBitsCorrect;
		bool m_bIsParityCorrect;
		bool m_bIsStopBitsCorrect;
		bool m_bIsFlowControlCorrect;

		bool m_bIsPortFound;
		bool m_bIsPortConnected;

		bool m_bIsBeginKeywordSet;
		bool m_bIsEndKeywordSet;
		bool m_bIsValueSeparatorSet;

		int m_i32BufferSize;
		QString m_sBufferBeginKeyword;
		QString m_sBufferEndKeyword;
		char m_cValueSeparator;

		std::vector<float> m_vDataBuffer;

        QElapsedTimer* m_pElapsedTimer;
	
};

#endif // SERIALPORTMANAGER_H
