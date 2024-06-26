#ifndef IMAGE_DISPLAY_H
#define IMAGE_DISPLAY_H

#include <QWidget>
#include <QFileDialog>
#include <QPaintEvent>
#include <QPainter>
#include <QMessageBox>
#include <QImage>
#include "opencv2/core/core.hpp"

class ImageDisplay: public QWidget
{
	Q_OBJECT

	public:
		ImageDisplay();
		virtual ~ImageDisplay();
		void paintEvent(QPaintEvent *event)override;

	public slots:
		void openDialog();
		void clearImage();
		void setImage(QImage);
		void setContours(std::vector<std::vector<cv::Point>>);
		void setPoint(cv::Point);
		void resetToProcessedImage();
	
	signals:
		void sendOriginalImage(QImage);
		void sendImageSize(int, int);
		void sendImageLoaded();
		void sendImageCleared();
			
	private :
		QImage m_oImage;
		QImage m_oOriginalImage;
		QImage m_oProcessedImage;
	
};
#endif
