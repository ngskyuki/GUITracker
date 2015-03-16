#ifndef TRACKER_H
#define TRACKER_H

#include <sstream>
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <QFile>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "imageinfo.h"

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

class Tracker
{
public:
    Tracker(int objectNumber = 0, bool automatic = false, bool isTrained = false);
    Tracker(ImageInfo *imgInfo, int objectNumber = 0, bool automatic = false, bool isTrained = false);
    ~Tracker();
    void setup();
    void setImageInfo(ImageInfo *info);
    ImageInfo *getImageInfo();
    void setObjectNumber(int num);
    int getObjectNumber();
    void setCurrentPlayerId(int id);
    int getCurrentPlayerId();
    void setExFileName(string fileName);
    string getExFileName();
    void setIsFirst(bool isFirst);
    bool getIsFirst();
    void setTrained(bool isTrained);
    bool getTrained();
    void setTimeStamp(double timeStamp);
    double getTimeStamp();
    void setFrameCount(int frameCount);
    int getFrameCount();
    void next();
    void init();
    void prepareTraj(int trajCount);
    void exportData();
    void incrementTime();
    void match();
private:\
    bool automatic;
    bool isTrained;
    bool isFirst;
    int objectNumber;
    int currentPlayerId;
    string exFileName;
    ImageInfo *imgInfo;
    std::vector<struct traj*> trj;
    double timeStamp;
    int frameCount;
    void prepareTemplates();

};

#endif // TRACKER_H
