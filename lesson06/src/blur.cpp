#include "blur.h"

#include <libutils/rasserts.h>

cv::Mat blur(cv::Mat img, double sigma) {
    int width = img.cols;
    int height = img.rows;
    cv::Mat blur(height, width, CV_32FC3);
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            cv::Vec3f dxyz = img.at<cv::Vec3f>(j, i);
            float x = 0;
            float y = 0;
            float z = 0;
            blur.at<cv::Vec3f>(j,i) = cv::Vec3f(x,y,z);
        }
    }
    // TODO формулу весов можно найти тут:
    // https://ru.wikipedia.org/wiki/%D0%A0%D0%B0%D0%B7%D0%BC%D1%8B%D1%82%D0%B8%D0%B5_%D0%BF%D0%BE_%D0%93%D0%B0%D1%83%D1%81%D1%81%D1%83
    return blur;
}
