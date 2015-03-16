#ifndef TEMPLATEMATCHER_H
#define TEMPLATEMATCHER_H

#include <sstream>
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <QFile>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

struct traj
{
    int playerID;
    Point2d currPt;
    Point2d prevPt;
    double derivX;
    double derivY;
    double precision;
    bool isCorrect;
    bool occlusion;
    Mat tmpl;
};

class TemplateMatcher
{
public:
    TemplateMatcher();
    ~TemplateMatcher();
    void setup();
    void setTrained(bool isTrained);
    bool getTrained();
    void setLeftSrcFileName(string leftSrcFileName);
    string getLeftSrcFileName();
    void setRightSrcFileName(string rightSrcFileName);
    string getRightSrcFileName();
    void setBgFileName(string fileName);
    string getBgFileName();
    void setExFileName(string fileName);
    string getExFileName();
    void setCapture(VideoCapture videoCapture);
    VideoCapture getCapture();
    void initCapture();
    void setLeftSrcPt(Point2f *srcPt[]);
    Point2f* getLeftSrcPt();
    void setLeftDstPt(Point2f *dstPt[]);
    Point2f* getLeftDstPt();
    void setRightSrcPt(Point2f *srcPt[]);
    Point2f* getRightSrcPt();
    void setRightDstPt(Point2f *dstPt[]);
    Point2f* getRightDstPt();

    void setDstSize(Size size);
    Size getDstSize();

    Mat getTmpLeftMat();
    Mat getTmpRightMat();

    void setTmpMat(Mat tmpMat);
    Mat getTmpMat();

    void setBg(Mat bg);
    Mat getBg();
    void setTmpDispMat(Mat tmpDispMat);
    Mat getTmpDispMat();
    void setTmpRoiRect(Rect tmpRoiRect);
    Rect getTmpRoiRect();
    void setTrj(vector<traj> trj);
    vector<struct traj> getTrj();
    void setTimeStamp(double timeStamp);
    double getTimeStamp();
    void setFrameCount(int frameCount);
    int getFrameCount();
    Mat transHomography();
    void Next();
    vector<struct traj> prepareTraj();
    void exportData();
    void incrementTime();
    void match();

private:
    bool automatic;
    void judgeInit();
    bool traind;
    string leftSrcFileName;
    string rightSrcFileName;
    string bgFileName;
    string exFileName;
    VideoCapture capture;
    Point2f leftSrcPt[4];
    Point2f leftDstPt[4];
    Point2f rightSrcPt[4];
    Point2f rightDstPt[4];
    Size dstSize;
    Mat tmpMat;
    Mat tmpLeftMat;
    Mat tmpRightMat;
    Mat bg;
    Mat tmpDispMat;
    Rect tmpRoiRect;
    vector<struct traj> trj;
    struct traj judge;
    double timeStamp;
    int frameCount;
    bool isFirst;
    void prepareTemplates();
};

#endif // TEMPLATEMATCHER_H
