// code from from https://github.com/stereomatchingkiss/ocv_libs

#ifndef OPENCVTOQT_HPP
#define OPENCVTOQT_HPP

/*! \file mat_and_qimage.h
    \brief conversion between qimage and mat
*/

#include <QImage>
#include <QDebug>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>


QImage mat_to_qimage_cpy(cv::Mat const &mat, bool swap = true);

QImage mat_to_qimage_ref(cv::Mat &mat, bool swap = true);

cv::Mat qimage_to_mat_cpy(QImage const &img, bool swap = true);

cv::Mat qimage_to_mat_ref(QImage &img, bool swap = true);


#endif // OPENCVTOQT_HPP
