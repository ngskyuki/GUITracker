#include "imageinfo.h"

ImageInfo::ImageInfo()
{

}

ImageInfo::ImageInfo(string fileNames[])
{
    this->setCapture(fileNames);
    this->setup(true);
}

ImageInfo::~ImageInfo()
{

}

void ImageInfo::setCapture(string fileNames[])
{
    this->capture[0] = VideoCapture(fileNames[0]);
    this->capture[1] = VideoCapture(fileNames[1]);
}
VideoCapture *ImageInfo::getCapture()
{
    /*Validation*/
    if(this->capture == NULL) { cout << "Captures are not set yet." << endl; return NULL; }
    return this->capture;
}

void ImageInfo::setCaptureNumber(double num)
{
    this->capture[0].set(CAP_PROP_FRAME_COUNT, num);
    this->capture[1].set(CAP_PROP_FRAME_COUNT, num);
}

double ImageInfo::getCaptureNumber() { return this->capture[0].get(CAP_PROP_FRAME_COUNT); }

void ImageInfo::setTmpLeftImg(Mat img) { this->tmpLeftImg = img; }

Mat ImageInfo::getTmpLeftImg() { return this->tmpLeftImg; }

void ImageInfo::setTmpRightImg(Mat img) { this->tmpRightImg = img; }

Mat ImageInfo::getTmpRightImg() { return this->tmpRightImg; }

void ImageInfo::setTmpImg(Mat img) { this->tmpImg = img; }

Mat ImageInfo::getTmpImg() { return this->tmpImg; }

void ImageInfo::setDispImg(Mat img) { this->dispImg = img; }

Mat ImageInfo::getDispImg() { return this->dispImg; }

void ImageInfo::setSrcPtLeft(Point2f pt[])
{
    for(int i = 0; i < 4; i++) {
        this->srcPtLeft[i] = Point(pt[i].x, pt[i].y);
    }
}

Point2f *ImageInfo::getSrcPtLeft() { return this->srcPtLeft; }

void ImageInfo::setSrcPtRight(Point2f pt[])
{
    for(int i = 0; i < 4; i++) {
        this->srcPtRight[i] = Point2f(pt[i].x, pt[i].y);
    }
}

Point2f *ImageInfo::getSrcPtRight() { return this->srcPtRight; }

void ImageInfo::setRoiRect(Rect roi) { this->roiRect = roi; }

Rect ImageInfo::getRoiRect() { return this->roiRect; }

void ImageInfo::setDstSize(Size size) { this->dstSize = size; }

Size ImageInfo::getDstSize() { return this->dstSize; }

void ImageInfo::setInitialized(bool intialized) { this->initialized = initialized; }

bool ImageInfo::getInitialized() { return this->initialized; }

void ImageInfo::setup(bool forInit)
{
    this->next();
    if(!forInit)
    {
        this->transHomography();
        this->mergeImg();
    }
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

bool ImageInfo::validate()
{
    if(this->tmpLeftImg.empty()  ||
            this->tmpRightImg.empty())
        return false;
    return true;
}
