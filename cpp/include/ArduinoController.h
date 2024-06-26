#ifndef ARDUINO_CONTROLLER_H
#define ARDUINO_CONTROLLER_H



#include <QObject>
#include <QDebug>
#include <QBasicTimer>

#include "opencv2/core/core.hpp"
#include <iostream>

#include "Config.h"

class ArduinoController : public QObject
{
	Q_OBJECT

	public:
		ArduinoController();
		virtual ~ArduinoController();
		

	public slots:
		void setContours(std::vector<std::vector<cv::Point>>);
		void setStartButton();
		void setPauseButton();
		void setStopButton();
		void startWork();
		void stopWork();
		void setFps(float);
		void setImageSize(int, int);
			
	private:
		void timerEvent(QTimerEvent *e);
		void doWork();
		void convertContourPixel2Mm();
	
	signals:
		void sigSendArduinoCommand(const QByteArray);
		void sigSendCurrentPixel(cv::Point);
	
	private :
		std::vector<std::vector<cv::Point>> m_vContoursInPixel;
		std::vector<std::vector<float>> m_vContoursInMm;
		QBasicTimer* m_pTimer;                  /**< qt timer */
		float m_fFps;
		bool m_bWorkStarted;
		int m_i32CurrentPointIndex; 
		int m_i32CurrentContourIndex; 
		int m_i32ImageWidth;
		int m_i32ImageHeigth;
};
#endif
