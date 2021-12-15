#include <opencv2/imgproc.hpp>
#include "parseSymbols.h"

std::vector<cv::Mat> splitSymbols(cv::Mat img)
{
    std::vector<cv::Mat> symbols;
    std::vector<std::vector<cv::Point>> contoursPoints;
    cv::findContours(img, contoursPoints, cv::RETR_EXTERNAL  ,cv::CHAIN_APPROX_NONE  );
    for (int contourI = 0; contourI < contoursPoints.size(); ++contourI) {
        std::vector<cv::Point> points = contoursPoints[contourI];
        int minx=1000;
        int maxx=0;
        int miny=1000;
        int maxy=0;
        for (int i = 0; i < points.size();i++) {
            if(points[i].x<minx){
                minx=points[i].x;
            }
            if(points[i].x>maxx){
                maxx=points[i].x;
            }
            if(points[i].y<miny){
                miny=points[i].y;
            }
            if(points[i].y>maxy){
                maxy=points[i].y;
            }
        }
        cv::Mat subImage(img, cv::Rect(minx, miny, maxx-minx+1, maxy-miny+1));
        symbols.push_back(subImage);
    }
    // TODO 101: чтобы извлечь кусок картинки (для каждого прямоугольника cv::Rect вокруг символа) - загуглите "opencv how to extract subimage"
    return symbols;
}
