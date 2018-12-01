//
// Created by 八木賢太郎 on 2018/10/11.
//

#ifndef BALLDOTSDETECTION_IMAGEINFO_H
#define BALLDOTSDETECTION_IMAGEINFO_H
#include "opencv2/opencv.hpp"
#include "iostream"
#include "vector"

class ImageInfo {
public:
    cv::Mat image;
    cv::Mat gray_image;
    cv::Mat crop_gray_image;
};


#endif //BALLDOTSDETECTION_IMAGEINFO_H
