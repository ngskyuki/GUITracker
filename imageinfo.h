#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;
using namespace cv;

class ImageInfo
{
public:
    ImageInfo();
    ImageInfo(string fileNames[]);
    ~ImageInfo();

    void setCapture(string fileNames[]);
    VideoCapture *getCaputre();
    void setTmpLeftImg(Mat img);
    Mat getTmpLeftImg();
    void setTmpRightImg(Mat img);
    Mat getTmpRightImg();
    void setTmpImg(Mat img);
    Mat getTmpImg();
    void setDispImg(Mat img);
    Mat getDispImg();
    void setSrcPtLeft(Point2f pt[]);
    Point2f* getSrcPtLeft();
    void setSrcPtRight(Point2f pt[]);
    Point2f* getSrcPtRight();
    void setRoiRect(Rect roi);
    Rect getRoiRect();
    void setDstSize(Size size);
    Size getDstSize();

    void next();
    void setup(bool forInit = false);
    void mergeImg();
    void transHomography();
    bool validate();

    Mat tmpImg;
    Mat dispImg;

private:

    VideoCapture capture[2];

    Mat tmpLeftImg;
    Mat tmpRightImg;
    Rect roiRect;
    Size dstSize;
    Point2f srcPtLeft[4];
    Point2f dstPtLeft[4];
    Point2f srcPtRight[4];
    Point2f dstPtRight[4];
};

#endif // IMAGEINFO_H
