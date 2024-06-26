#include "ImageDisplay.h"
#include <QDebug>

ImageDisplay::ImageDisplay()   
{
}

ImageDisplay::~ImageDisplay()   
{
}

void ImageDisplay::openDialog()    
{
	QMessageBox::information(this,"Dialog box","Open the image to process");
	QString fileName = QFileDialog::getOpenFileName(this, "Open...", QString(), "Images (*.png *.jpg *.jpeg)");
	bool bImageloaded = m_oImage.load(fileName); 
	
	if (!bImageloaded)
	{
		QMessageBox::critical(this, "Error", "The image could not be loaded!");
	}
	else
	{
		setFixedSize(m_oImage.width(),m_oImage.height());
	}
	
	m_oOriginalImage=m_oImage;
	
	emit sendOriginalImage(m_oOriginalImage);
	emit sendImageSize(m_oImage.width(),m_oImage.height());
	emit sendImageLoaded();
	
	update();
}

void ImageDisplay::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawImage(0,0,m_oImage);
	update();
}

void ImageDisplay::clearImage()
{
	m_oImage=m_oOriginalImage; 
	
	emit sendOriginalImage(m_oOriginalImage);
	emit sendImageSize(m_oImage.width(),m_oImage.height());
	emit sendImageCleared();
	
	update();
}

void ImageDisplay::resetToProcessedImage()
{
	m_oImage=m_oProcessedImage; 
	
	update();
}

void ImageDisplay::setImage(QImage oImage)
{
	m_oImage = oImage;
	
	setFixedSize(m_oImage.width(),m_oImage.height());
	emit sendImageSize(m_oImage.width(),m_oImage.height());
	
	update();
}

void ImageDisplay::setContours(std::vector<std::vector<cv::Point>> vContours)
{
	if (vContours.size() == 0)
	{
		qDebug() << "[WARNING](ImageDisplay::setContours) Contour vector is empty!";
		return;
	}
	
	QPainter painter(&m_oImage);

	QPen pen;
	pen.setColor(Qt::red);
	pen.setWidth(5);
	painter.setPen(pen);

	for (int l_contour = 0; l_contour < vContours.size(); l_contour++)
	{
		for (int l_point = 0; l_point < vContours[l_contour].size(); l_point++)
		{
			QPoint currentPoint;
			currentPoint.setX(vContours[l_contour][l_point].x);
			currentPoint.setY(vContours[l_contour][l_point].y);
			painter.drawPoint(currentPoint);
		}
	}
	
	// saves the current processed image
	m_oProcessedImage = m_oImage;
	
	update();

}

void ImageDisplay::setPoint(cv::Point oPoint)
{
	QPainter painter(&m_oImage);

	QPen pen;
	pen.setColor(Qt::green);
	pen.setWidth(5);
	painter.setPen(pen);
	
	QPoint currentPoint;
	currentPoint.setX(oPoint.x);
	currentPoint.setY(oPoint.y);
	painter.drawPoint(currentPoint);
	
	update();

}
