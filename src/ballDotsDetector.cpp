//
// Created by 八木賢太郎 on 2018/10/11.
//

#include "ballDotsDetector.h"

using namespace cv;
using namespace std;

void ballDotsDetector::ballDotsDetection() {
    int im_num = 0;
    for (ImageInfo im: this->image_list){

            cv::Mat bin_img;
            threshold(im.gray_image, bin_img, 15, 255, THRESH_BINARY); //閾値160で2値画像に変換

            vector<vector<cv::Point> > contours;
            vector<cv::Vec4i> hierarchy;
            cv::findContours(bin_img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

            int max_area = 0;
            int maxID = 0;
            for (int i = 0; i < contours.size(); i++) {
                double area = cv::contourArea(contours.at(i));
                if (max_area < area) {
                    max_area = area;
                    maxID = i;
                }
            }

            cv::Point minP = minPoint(contours.at(maxID));
            cv::Point maxP = maxPoint(contours.at(maxID));
            cv::Rect rect(minP, maxP);
            //矩形を描く
            cv::rectangle(im.image, rect, cv::Scalar(0, 255, 0), 2, 8);

            //gray scaleからrectをクロップ
            cv::Mat subImg = im.gray_image(rect); // 切り出し画像
            image_list[im_num].crop_gray_image = subImg;
            cv::Mat colorRectImg = im.image(rect); // 切り出し画像

            if(im_num > 1110) {
                featureDetection(image_list[im_num - 1].crop_gray_image, image_list[im_num].crop_gray_image);
            }else {

                cv::Mat bin_sub;
                threshold(subImg, bin_sub, 50, 255, THRESH_BINARY); //閾値160で2値画像に変換
//
//
//                int range = 6;
//                for (int i = 0 + range / 2; i < subImg.rows - range / 2; i++) {
//                    for (int j = 0 + range / 2; j < subImg.cols - range / 2; j++) {
//                        float pix_value = subImg.at<unsigned char>(j,i);
//                        bool fail = false;
//                        for (int a = i - range / 2; a < i + range / 2; a++) {
//                            for (int b = j - range / 2; b < j + range / 2; b++) {
////                                pix_value += subImg.at<unsigned char>(a, b);
//                                if(pix_value > subImg.at<unsigned char>(b,a))
//                                    fail = true;
//                            }
//                        }
//                        if(!fail){
//                            colorRectImg.at<cv::Vec3b>(j,i) = cv::Vec3b(0,0,255);
//                        }
//                        pix_value /= range * range;
////                cout << i << " " << j << " " << pix_value << endl;
//                    }
//                }

//
//                cv::imshow("aaaaa", colorRectImg );
//                cv::imwrite("../data/image" + to_string(im_num) + ".jpg", colorRectImg);
//
//                cv::waitKey();
//
//                // 線形フィルタ
//                double filter[][3] =
//                        {
//                                {0, 1,  0},
//                                {1, -4, 1},
//                                {0, 1,  0}
//                        };
//
//                // 入力画像と同じサイズの画像を0で初期化
//                cv::Mat dst = cv::Mat::zeros(bin_sub.rows, bin_sub.cols, CV_8U);
//                // 配列外アクセスが起きないように
//                for (int j = 1; j < subImg.rows - 1; j++) {
//                    for (int i = 1; i < subImg.cols - 1; i++) {
//                        int tmp = 0;
//                        for (int jj = -1; jj < 1; jj++) {
//                            for (int ii = -1; ii < 1; ii++) {
//                                tmp += subImg.at<uchar>(j + jj, i + ii) * filter[jj + 1][ii + 1];
//                            }
//                        }
//                        if (tmp < 0) tmp = -tmp;  // 負の場合は絶対値をとる
//                        if (tmp > 255) tmp = 255;  // 255を超えないようにする
//                        dst.at<uchar>(j, i) = tmp;
//                    }
//                }
////        threshold(dst, dst,200,255,THRESH_BINARY); //閾値160で2値画像に変換
//                cv::imshow("saaa", subImg);
//                cv::imshow("sassaa", dst);
//
                cv::Mat canny;
                cv::Canny(subImg, canny, 30, 110);
                cv::Mat cannyRect = canny.clone();

                cv::dilate(cannyRect, cannyRect, cv::Mat(), cv::Point(-1, -1), 2);
                cv::erode(cannyRect, cannyRect, cv::Mat(), cv::Point(-1, -1), 1);
                cv::erode(cannyRect, cannyRect, cv::Mat(), cv::Point(-1, -1), 1);
                cv::erode(cannyRect, cannyRect, cv::Mat(), cv::Point(-1, -1), 1);

                //画像はんてん
                cv::Mat destination;
                bitwise_not(cannyRect, destination);

                vector<vector<cv::Point> > cannyContours;
                vector<cv::Vec4i> cannyHierarchy;
                cv::findContours(destination, cannyContours, cannyHierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_TC89_L1);

                for (int i = 0; i < cannyContours.size(); i++) {
                    cv::Point minP = minPoint(contours.at(i));
                    cv::Point maxP = maxPoint(contours.at(i));
                    cv::Rect rect(minP, maxP);
                    //矩形を描く
                    cv::rectangle(colorRectImg, rect, cv::Scalar(0, 255, 0), 2, 8);
                }

                cv::imshow("color", im.image);
//        cv::imshow("binary", subImg);
//        cv::imshow("binaryaa", subImg);
//        cv::imshow("cambby", canny);
//        cv::imshow("camnny rect", cannyRect);
//        cv::imshow("color rect", colorRectImg);


                cv::waitKey();
            }
        im_num++;
    }

}


void featureDetection(cv::Mat scene1, cv::Mat scene2){
//    //読み込む画像のパス
//    cv::String scene1_path = "imgPath1"
//    cv::String scene2_path = "imgPath2";
//

    //書き出す画像のパス
    cv::String scene_12_path = "imgPath12";


    //比較用画像を読み込む (アルファチャンネル非対応のため、IMREAD_COLORで強制する)


    //AKAZE
//    auto algorithm = cv::AKAZE::create();

    //KAZE
    //auto algorithm = cv::KAZE::create();

    //ORB
    //auto algorithm = cv::ORB::create();

    //BRISK
    auto algorithm = cv::BRISK::create();

    //FAST
    //auto algorithm = cv::FastFeatureDetector::create();


    // 特徴点抽出
    std::vector<cv::KeyPoint> keypoint1, keypoint2;
    algorithm->detect(scene1, keypoint1);
    algorithm->detect(scene2, keypoint2);


    // 特徴記述
    cv::Mat descriptor1, descriptor2;
    algorithm->compute(scene1, keypoint1, descriptor1);
    algorithm->compute(scene2, keypoint2, descriptor2);

    // マッチング (アルゴリズムにはBruteForceを使用)
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce");
    std::vector<cv::DMatch> match, match12, match21;
    matcher->match(descriptor1, descriptor2, match12);
    matcher->match(descriptor2, descriptor1, match21);
    //クロスチェック(1→2と2→1の両方でマッチしたものだけを残して精度を高める)
    for (size_t i = 0; i < match12.size(); i++)
    {
        cv::DMatch forward = match12[i];
        cv::DMatch backward = match21[forward.trainIdx];
        if (backward.trainIdx == forward.queryIdx)
        {
            match.push_back(forward);
        }
    }

    // マッチング結果の描画
    cv::Mat dest;
    cv::drawMatches(scene1, keypoint1, scene2, keypoint2, match, dest);
    cv::imshow("a", dest);
    cv::waitKey();
    //マッチング結果の書き出し
//    cv::imwrite(scene_12_path, dest);

//    return 0;
}

//最小座標を求める
cv::Point minPoint(vector<cv::Point> contours){
    double minx = contours.at(0).x;
    double miny = contours.at(0).y;
    for(int i=1;i<contours.size(); i++){
        if(minx > contours.at(i).x){
            minx = contours.at(i).x;
        }
        if(miny > contours.at(i).y){
            miny = contours.at(i).y;
        }
    }
    return cv::Point(minx, miny);
}
//最大座標を求める
cv::Point maxPoint(vector<cv::Point> contours){
    double maxx = contours.at(0).x;
    double maxy = contours.at(0).y;
    for(int i=1;i<contours.size(); i++){
        if(maxx < contours.at(i).x){
            maxx = contours.at(i).x;
        }
        if(maxy < contours.at(i).y){
            maxy = contours.at(i).y;
        }
    }
    return cv::Point(maxx, maxy);
}

void ballDotsDetector::loadVideo(string video_path){
    Mat img;
    VideoCapture cap(video_path); //Windowsの場合　パス中の¥は重ねて¥¥とする
    int max_frame=cap.get(CV_CAP_PROP_FRAME_COUNT); //フレーム数
    for(int i=0; i<max_frame;i++){
        ImageInfo im;
        cap >> im.image;
        cvtColor(im.image, im.gray_image, CV_BGR2GRAY);
        this->image_list.push_back(im);

//        cv::imshow("test", im.image);
//        cv::waitKey();
    }
}

