#include "ArduinoController.h"

ArduinoController::ArduinoController(): 
	m_pTimer(new QBasicTimer),
	m_fFps(1.0),
	m_bWorkStarted(false),
	m_i32CurrentPointIndex(0),
	m_i32CurrentContourIndex(0),
	m_i32ImageWidth(0),
	m_i32ImageHeigth(0)
{
	
}

ArduinoController::~ArduinoController()
{
	if (nullptr != m_pTimer)
	{
		delete m_pTimer;
		m_pTimer = nullptr;
	}
}

void ArduinoController::setContours(std::vector<std::vector<cv::Point>> vContours)
{
	if (vContours.size() == 0)
	{
		qDebug() << "[WARNING](ArduinoController::setContours) Contour vector is empty!";
		return;
	}
		
	m_vContoursInPixel = vContours;
	convertContourPixel2Mm();
}

void ArduinoController::setImageSize(int i32ImageWidth, int i32ImageHeigth)
{
	m_i32ImageWidth = i32ImageWidth;
	m_i32ImageHeigth = i32ImageHeigth;
}

void ArduinoController::setStartButton()
{
	qDebug() << "Start sending commands to the Arduino board ...";
	QByteArray l_oArduinoCommand = "Start\n";
	emit sigSendArduinoCommand(l_oArduinoCommand);
	startWork();
}

void ArduinoController::setPauseButton()
{
	qDebug() << "Pause sending commands to the Arduino board ...";
	stopWork();
	
	QByteArray l_oArduinoCommand = "Pause\n";
	emit sigSendArduinoCommand(l_oArduinoCommand);
	
}

void ArduinoController::setStopButton()
{
	qDebug() << "Stop sending commands to the Arduino board ...";
	stopWork();
	
	QByteArray l_oArduinoCommand = "Stop\n";
	emit sigSendArduinoCommand(l_oArduinoCommand);
	
	// initialize the indices
	m_i32CurrentPointIndex = 0;
	m_i32CurrentContourIndex = 0;
}

void ArduinoController::convertContourPixel2Mm()
{
	m_vContoursInMm.clear();
	
	float theta = -90.0 / 180.0 * CV_PI;
	float tx = 0.0;
	float ty = m_i32ImageHeigth;
	
	// pixel / mm ratios
	float widthRatio = (float)m_i32ImageWidth / drawingAreaWidthInMm;
	float heigthRatio = (float)m_i32ImageHeigth / drawingAreaHeigthInMm;
	float appliedRatio = ceil((widthRatio>=heigthRatio) ? widthRatio : heigthRatio);
	//qDebug() << "widthRatio= " << widthRatio << ", heigthRatio= " << heigthRatio << ", appliedRatio= " << appliedRatio;

	// image to real world reference frame transformation
	cv::Mat transformationMatrix(3, 3, CV_32FC1);
	transformationMatrix.at<float>(0,0) = cos(theta); 	transformationMatrix.at<float>(0,1) = -sin(theta);	transformationMatrix.at<float>(0,2) = tx;
	transformationMatrix.at<float>(1,0) = sin(theta); 	transformationMatrix.at<float>(1,1) = cos(theta);	transformationMatrix.at<float>(1,2) = ty;
	transformationMatrix.at<float>(2,0) = 0.0;		transformationMatrix.at<float>(2,1) = 0.0;		transformationMatrix.at<float>(2,2) = 1.0;
	
	// size the output vector with the same number of contours as the input one
	m_vContoursInMm.resize(m_vContoursInPixel.size());
	
	// fill the output vector by transforming the data
	for (int l_contour = 0; l_contour < m_vContoursInPixel.size(); l_contour++)
	{
		for (int l_point = 0; l_point < m_vContoursInPixel[l_contour].size(); l_point++)
		{
			// get the coordinate of the current point in the pixel reference frame
			cv::Mat pointCoordinateInPixel(3, 1, CV_32FC1);
			pointCoordinateInPixel.at<float>(0,0) = m_vContoursInPixel[l_contour][l_point].y; //!! WATCH OUT Points go (x,y); (width,height)
			pointCoordinateInPixel.at<float>(1,0) = m_vContoursInPixel[l_contour][l_point].x;
			pointCoordinateInPixel.at<float>(2,0) = 1.0;
			
			// transform it in the real world reference frame
			cv::Mat pointCoordinateInCm(3, 1, CV_32FC1);
			pointCoordinateInCm = transformationMatrix * pointCoordinateInPixel;
			
			// store the data in the contour in cm
			m_vContoursInMm[l_contour].push_back(pointCoordinateInCm.at<float>(0,0) / appliedRatio);
			m_vContoursInMm[l_contour].push_back(pointCoordinateInCm.at<float>(1,0) / appliedRatio);
		}
	}
	
}

void ArduinoController::timerEvent(QTimerEvent *e)
{
	Q_UNUSED(e);

	doWork();
}

void ArduinoController::doWork()
{
	if (m_vContoursInPixel.size() == 0)
	{
		qDebug() << "[WARNING](ArduinoController::doWork) Contour vector is empty!";
		stopWork();
		return;
	}
	
	float targetPointX = m_vContoursInMm[m_i32CurrentContourIndex][2*m_i32CurrentPointIndex];
	float targetPointY = m_vContoursInMm[m_i32CurrentContourIndex][2*m_i32CurrentPointIndex+1];
	
	float targetPointI = m_vContoursInPixel[m_i32CurrentContourIndex][m_i32CurrentPointIndex].x;
	float targetPointJ = m_vContoursInPixel[m_i32CurrentContourIndex][m_i32CurrentPointIndex].y;
	
	std::cout<<"CurrentPoint = (" << targetPointJ << "," << targetPointI << ") --> (" << targetPointX << "," << targetPointY << ")" << std::endl;
		
	QString l_sCmd = "CurrentPosition:" + QString::number(targetPointX) + ";" + QString::number(targetPointY) +"\n";
	QByteArray l_oArduinoCommand = l_sCmd.toLatin1();
	emit sigSendArduinoCommand(l_oArduinoCommand);
	
	cv::Point oPoint;
	oPoint.x = m_vContoursInPixel[m_i32CurrentContourIndex][m_i32CurrentPointIndex].x;
	oPoint.y = m_vContoursInPixel[m_i32CurrentContourIndex][m_i32CurrentPointIndex].y;
	emit sigSendCurrentPixel(oPoint);
	
	m_i32CurrentPointIndex+=10;
	
	if (m_i32CurrentPointIndex >= m_vContoursInPixel[m_i32CurrentContourIndex].size())
	{
		m_i32CurrentPointIndex = 0;
		m_i32CurrentContourIndex++;
		
		if (m_i32CurrentContourIndex >= m_vContoursInPixel.size())
		{
			m_i32CurrentContourIndex = 0;
			stopWork();
		}
	}
}

void ArduinoController::setFps(float fFps)
{
	m_fFps = fFps;
}

void ArduinoController::startWork()
{
	if (m_fFps == 0.0)
	{
		qDebug() << "[ERROR] (ArduinoController::startWork): Frame rate is set to 0!";
		return;
	}

	if (!m_bWorkStarted)
	{
		m_bWorkStarted = true;
		m_pTimer->start(1000.0 / m_fFps, this);
	}
}

void ArduinoController::stopWork()
{
	if (m_bWorkStarted)
	{
		m_bWorkStarted = false;
		m_pTimer->stop();
	}	
}
