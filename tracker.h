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
#include <vector>
#include "imageinfo.h"

using namespace std;
using namespace cv;

struct exData
{
    float timeStamp;
    int playerID;
    Point2f currPt;
    float precision;
    bool isCorrect;
    bool occlusion;
};

struct traj
{
    int playerID;
    Point2f currPt;
    Point2f prevPt;
    float derivX;
    float derivY;
    float precision;
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
    void setAutomatic(bool automatic);
    bool getAutomatic();
    void setObjectNumber(int num);
    int getObjectNumber();
    void setCurrentId(int currentId);
    int getCurrentId();
    void setExFileName(string fileName);
    string getExFileName();
    void setIsFirst(bool isFirst);
    bool getIsFirst();
    void setTrained(bool isTrained);
    bool getTrained();
    void setTimeStamp(float timeStamp);
    float getTimeStamp();
    void setFrameCount(int frameCount);
    int getFrameCount();
    void setExData(vector<struct exData*> *exData);
    vector<struct exData*> *getExData();

    void next();
    void init();
    void prepareTraj(int trajCount);
    void exportData(struct exData* data);
    void exportData();
    void incrementTime();
    void match();
    void track(Point2f pt);
    bool validate();
    void nextPlayer();
private:\
    bool automatic;
    bool isTrained;
    bool isFirst;
    int objectNumber;
    int currentId;
    string exFileName;
    ImageInfo *imgInfo;
    std::vector<struct traj*> trj;
    float timeStamp;
    int frameCount;
    vector<struct exData*> *exData;
    void prepareTemplates();

};

#endif // TRACKER_H
