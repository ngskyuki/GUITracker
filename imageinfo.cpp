#include "imageinfo.h"

ImageInfo::ImageInfo()
{
    this->dstSize = Size(537, 269);
    this->dstPtLeft[0] = Point(0, 0);
    this->dstPtLeft[1] = Point(0, 269);
    this->dstPtLeft[2] = Point(537, 269);
    this->dstPtLeft[3] = Point(537, 0);

    this->dstPtRight[0] = Point(0, 0);
    this->dstPtRight[1] = Point(0, 269);
    this->dstPtRight[2] = Point(537, 269);
    this->dstPtRight[3] = Point(537, 0);
}

ImageInfo::ImageInfo(string fileNames[])
{
    this->setCapture(fileNames);
    this->setup(true);
    this->dstSize = Size(537, 269);
    this->dstPtLeft[0] = Point(0, 0);
    this->dstPtLeft[1] = Point(0, 269);
    this->dstPtLeft[2] = Point(537, 269);
    this->dstPtLeft[3] = Point(537, 0);

    this->dstPtRight[0] = Point(0, 0);
    this->dstPtRight[1] = Point(0, 269);
    this->dstPtRight[2] = Point(537, 269);
    this->dstPtRight[3] = Point(537, 0);
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
    this->capture[0].set(CAP_PROP_POS_FRAMES, num);
    this->capture[1].set(CAP_PROP_POS_FRAMES, num);
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
        this->srcPtRight[i] = Point(pt[i].x, pt[i].y);
    }
}

Point2f *ImageInfo::getSrcPtRight() { return this->srcPtRight; }

void ImageInfo::setRoiRect(Rect roi) { this->roiRect = roi; }

Rect ImageInfo::getRoiRect() { return this->roiRect; }

void ImageInfo::setDstSize(Size size) { this->dstSize = size; }

Size ImageInfo::getDstSize() { return this->dstSize; }

void ImageInfo::setInitialized(bool isInitialized) { this->initialized = isInitialized; }

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

void ImageInfo::paintCircle(Point2f pt)
{
    circle(this->dispImg, pt, 2, Scalar(255, 0, 0));
}

void ImageInfo::paintId(Point2f pt, int id)
{
    putText(this->dispImg, to_string(id), pt, FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 0, 255));
}

void ImageInfo::next()
{ 
    this->capture[0] >> this->tmpLeftImg;
    this->capture[1] >> this->tmpRightImg;
}

void ImageInfo::mergeImg()
{
    Size s = Size(this->tmpLeftImg.cols * 2,
                  max(this->tmpLeftImg.rows, this->tmpRightImg.rows));
    this->tmpImg = Mat::zeros(s, CV_8UC3);
    Rect roi = Rect(0, 0, this->tmpLeftImg.cols, this->tmpLeftImg.rows);
    this->tmpLeftImg.copyTo(this->tmpImg(roi));
    roi = Rect(this->tmpLeftImg.cols, 0, this->tmpRightImg.cols, this->tmpRightImg.rows);
    this->tmpRightImg.copyTo(this->tmpImg(roi));
}

void ImageInfo::transHomography()
{
    Mat homographyMtx = getPerspectiveTransform(this->srcPtLeft, this->dstPtLeft);
    warpPerspective(this->tmpLeftImg, this->tmpLeftImg, homographyMtx, this->dstSize, INTER_LANCZOS4 + WARP_FILL_OUTLIERS);
    Mat homographyMtx2 = getPerspectiveTransform(this->srcPtRight, this->dstPtRight);
    warpPerspective(this->tmpRightImg, this->tmpRightImg, homographyMtx2, this->dstSize, INTER_LANCZOS4 + WARP_FILL_OUTLIERS);
}

bool ImageInfo::validate()
{
    if(this->tmpLeftImg.empty()  ||
            this->tmpRightImg.empty())
        return false;
    return true;
}
