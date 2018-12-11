//
// Created by 八木賢太郎 on 2018/10/11.
//

#ifndef BALLDOTSDETECTION_BALLDOTSDETECTOR_H
#define BALLDOTSDETECTION_BALLDOTSDETECTOR_H
#include "iostream"
#include "opencv2/opencv.hpp"
#include "./ImageInfo.h"


class ballDotsDetector {
public:
    std::vector<ImageInfo> image_list;


    void ballDotsDetection();
    void ballDotsDetectionFromVideo(std::string videoFolderPath);
    void loadVideo(std::string videoPath);

    int VIDEO_NUMBER;
};

//最小座標を求める
cv::Point minPoint(std::vector<cv::Point> contours);
//最大座標を求める
cv::Point maxPoint(std::vector<cv::Point> contours);

void featureDetection(cv::Mat scene1, cv::Mat scene2);


#endif //BALLDOTSDETECTION_BALLDOTSDETECTOR_H
