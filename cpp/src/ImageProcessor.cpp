#include "ImageProcessor.h"

ImageProcessor::ImageProcessor()
{}

ImageProcessor::~ImageProcessor()
{}

void ImageProcessor::setImage(QImage oImage)
{
	m_oImage = oImage;
	m_oOriginalImage=oImage;

	// convert to opencv format
	m_oMatImage = qimage_to_mat_cpy(m_oImage, false);

	// save the image
	cv::imwrite("original.jpg", m_oMatImage);

}

void ImageProcessor::process()
{
	qDebug() << "Image Processing in progress...";

	if (m_oMatImage.empty())
	{
		qDebug() << "[ERROR](ImageProcessor::process): the cv::Mat image is empty!";
		return;
	}
    
	// convert to grayscale
	qDebug() << "--> Color To Grayscale ";
	cv::cvtColor(m_oMatImage, m_oMatImage, cv::COLOR_BGR2GRAY);

	// save the image
	cv::imwrite("gray.jpg", m_oMatImage);

	// threshold the image
	qDebug() << "--> Thresholding";
	cv::threshold(m_oMatImage, m_oMatImage, thresholdValue, maxValue, cv::THRESH_BINARY);

	// save the image
	cv::imwrite("threshold.jpg", m_oMatImage);

	//find the contours
	qDebug() << "--> Contour detection";
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours( m_oMatImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE );

	cv::Mat drawing = cv::Mat::zeros( m_oMatImage.size(), CV_8UC3 );
	cv::RNG rng(12345);
	for( size_t i = 0; i< contours.size(); i++ )
	{
		cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
		drawContours( drawing, contours, (int)i, color, 2, cv::LINE_8, hierarchy, 0 );
	}
	
	// save the image
	cv::imwrite("contour.jpg", drawing);
	
	emit sendContours(contours);
	emit sendImageProcessed();
	
	qDebug() << "Image Processing done...";
	
}
