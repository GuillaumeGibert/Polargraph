#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <iostream>

#include <QObject>
#include <QImage>
#include <QDebug>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "mat_and_qimage.hpp"

#include "Config.h"

class ImageProcessor : public QObject
{
	Q_OBJECT

	public:
		ImageProcessor();
		virtual ~ImageProcessor();
		

	public slots:
		void setImage(QImage);
		void process();
	
	signals:
		void sendContours(std::vector<std::vector<cv::Point>>);
		void sendImageProcessed();
	
	private :
		QImage m_oImage;
		QImage m_oOriginalImage;
		cv::Mat m_oMatImage;
	
};
#endif
