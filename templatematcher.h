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
    void setSrcFileName(string fileName);
    string getSrcFileName();
    void setBgFileName(string fileName);
    string getBgFileName();
    void setExFileName(string fileName);
    string getExFileName();
    void setCapture(VideoCapture videoCapture);
    VideoCapture getCapture();
    void setSrcPt(Point2f *srcPt[]);
    Point2f* getSrcPt();
    void setDstPt(Point2f *dstPt[]);
    Point2f* getDstPt();
    void setDstSize(Size size);
    Size getDstSize();
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

    void Next();
    vector<struct traj> prepareTraj();
    void exportData();
    void incrementTime();

    void match();

private:
    string srcFileName;
    string bgFileName;
    string exFileName;
    VideoCapture capture;
    Point2f srcPt[4];
    Point2f dstPt[4];
    Size dstSize;
    Mat tmpMat;
    Mat bg;
    Mat tmpDispMat;
    Rect tmpRoiRect;
    vector<struct traj> trj;
    double timeStamp;
    int frameCount;
    void transHomography();
    void prepareTemplates();
};

#endif // TEMPLATEMATCHER_H
