#include "imageinfo.h"

ImageInfo::ImageInfo()
{

}

ImageInfo::ImageInfo(string fileNames[])
{
    for(int i = 0; i < 2; i++) { cout << "Please input 2 video files" << endl; return; }
    this->setCapture(fileNames);
    this->setup();
}

ImageInfo::~ImageInfo()
{

}

void ImageInfo::setCapture(string fileNames[])
{
    this->capture[0] = VideoCapture(fileNames[0]);
    this->capture[1] = VideoCapture(fileNames[1]);
}
VideoCapture *ImageInfo::getCaputre()
{
    /*Validation*/
    if(this->capture == NULL) { cout << "Captures are not set yet." << endl; return NULL; }
    return this->capture;
}

void ImageInfo::setTmpLeftImg(Mat img) { this->tmpLeftImg = img; }

Mat ImageInfo::getTmpLeftImg() { return this->tmpLeftImg; }

void ImageInfo::setTmpRightImg(Mat img) { this->tmpRightImg = img; }

Mat ImageInfo::getTmpRightImg() { return this->tmpRightImg; }

void ImageInfo::setTmpImg(Mat img) { this->tmpImg = img; }

Mat ImageInfo::getTmpImg() { return this->tmpImg; }

void ImageInfo::setDispImg(Mat img) { this->dispImg = img; }

Mat ImageInfo::getDispImg() { return this->dispImg; }

void ImageInfo::setSrcPtLeft(Point2f *pt[])
{
    /*Validation*/
    for(int i = 0; i < 4; i++) { if(pt[i] == NULL) { cout << "Please set 4 points." << endl; return; } }
    Point tmp;
    for(int i = 0; i < 4; i++) {
        tmp  = Point(pt[i]->x, pt[i]->y);
        this->srcPtLeft[i] = tmp;
    }
}

Point2f *ImageInfo::getSrcPtLeft() { return this->srcPtLeft; }

void ImageInfo::setSrcPtRight(Point2f *pt[])
{
    /*Validation*/
    for(int i = 0; i < 4; i++) {if(pt[i] == NULL) { cout << "Please set 4 points." << endl; return; } }
    Point tmp;
    for(int i = 0; i < 4; i++) {
        tmp = Point(pt[i]->x, pt[i]->y);
        this->srcPtRight[i] = tmp;
    }
}

Point2f *ImageInfo::getSrcPtRight() { return this->srcPtRight; }

void ImageInfo::setRoiRect(Rect roi) { this->roiRect = roi; }

Rect ImageInfo::getRoiRect() { return this->roiRect; }

void ImageInfo::setDstSize(Size size) { this->dstSize = size; }

Size ImageInfo::getDstSize() { return this->dstSize; }

void ImageInfo::setup()
{
    this->next();
    this->transHomography();
    this->mergeImg();
}

void ImageInfo::next()
{
    this->capture[0] >> this->tmpLeftImg;
    this->capture[1] >> this->tmpRightImg;
}

void ImageInfo::mergeImg()
{
    Size s = Size(this->tmpLeftImg.cols + this->tmpRightImg.cols,
                  max(this->tmpLeftImg.rows, this->tmpRightImg.rows));
    this->tmpImg = Mat(s, CV_8UC3);
    Rect roi = Rect(0, 0, this->tmpLeftImg.cols, this->tmpLeftImg.rows);
    add(this->tmpLeftImg, this->tmpImg(roi), this->tmpImg);
    roi = Rect(this->tmpLeftImg.cols, 0, this->tmpRightImg.cols, this->tmpRightImg.rows);
    add(this->tmpRightImg, this->tmpImg(roi), this->tmpImg);
}

void ImageInfo::transHomography()
{
    Mat homographyMtx = getPerspectiveTransform(this->srcPtLeft, this->dstPtLeft);
    warpPerspective(this->tmpLeftImg, this->tmpLeftImg, homographyMtx, this->dstSize, INTER_LANCZOS4 + WARP_FILL_OUTLIERS);
    homographyMtx = getPerspectiveTransform(this->srcPtRight, this->dstPtRight);
    warpPerspective(this->tmpRightImg, this->tmpRightImg, homographyMtx, this->dstSize, INTER_LANCZOS4, WARP_FILL_OUTLIERS);
}
