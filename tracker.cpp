#include "tracker.h"

Tracker::Tracker(int objectNumber, bool automatic, bool isTrained)
{ 
    this->objectNumber = objectNumber;
    this->automatic = automatic;
    this->isTrained = isTrained;
    this->timeStamp = 0.0;
    this->frameCount = 0;
    this->currentId = 0;
}

Tracker::Tracker(ImageInfo *imgInfo, int objectNumber, bool automatic, bool isTrained)
{
    this->imgInfo = imgInfo;
    this->objectNumber = objectNumber;
    this->isTrained = isTrained;
    this->automatic = automatic;
    this->prepareTraj(this->objectNumber);

    this->timeStamp = 0.0;
    this->frameCount = 0;
    this->currentId = 0;
}

Tracker::~Tracker()
{

}

void Tracker::setup() {}

void Tracker::setImageInfo(ImageInfo *info) { this->imgInfo = info; }

ImageInfo *Tracker::getImageInfo() { return this->imgInfo; }

void Tracker::setCurrentId(int currentId) { this->currentId = currentId; }

int Tracker::getCurrentId() { return this->currentId; }

void Tracker::setObjectNumber(int num) { this->objectNumber = num; }

int Tracker::getObjectNumber() { return this->objectNumber; }

void Tracker::setExFileName(string fileName) { this->exFileName = fileName; }

string Tracker::getExFileName() { return this->exFileName; }

void Tracker::setIsFirst(bool isFirst) { this->isFirst = isFirst; }

bool Tracker::getIsFirst() { return this->isFirst; }

void Tracker::setTrained(bool isTrained) { this->isTrained = isTrained; }

bool Tracker::getTrained() { return this->isTrained; }

void Tracker::setTimeStamp(double timeStamp) { this->timeStamp = timeStamp; }

double Tracker::getTimeStamp() { return this->timeStamp; }

int Tracker::getFrameCount() { return this->frameCount; }

void Tracker::setFrameCount(int frameCount) { this->frameCount = frameCount; }

void Tracker::nextPlayer() { this->currentId++; this->incrementTime(); }

void Tracker::next()
{
    this->imgInfo->setup();
    if(this->imgInfo->getTmpImg().empty()) cout << "Video is empty!" << endl;
    this->frameCount++;
}

void Tracker::incrementTime()
{
    int sec;
    int count;
    if(frameCount % 33 == 0)
    {
        sec = frameCount / 33 + 1;
        count = 0;
    }
    else
    {
        sec = frameCount / 33;
        count = frameCount % 33;
    }
    this->timeStamp = sec + count * 0.03;
}

void Tracker::exportData()
{
    ofstream ofs;
    QFile f(QString(this->exFileName.c_str()));
    ofs.open((f.fileName().toStdString().c_str()));
    vector<struct traj*>::iterator begin, end;
    begin = this->trj.begin();
    end = this->trj.end();

    for(; begin != end; begin++)
    {
        ofs << this->timeStamp << ",";
        ofs << (*begin)->playerID << ",";
        ofs << (*begin)->currPt.x << ",";
        ofs << (*begin)->currPt.y << ",";
        ofs << (*begin)->precision << ",";
        ofs << (*begin)->isCorrect << ",";
        ofs << (*begin)->occlusion << endl;
    }
    ofs.close();
    return;
}

void Tracker::prepareTraj(int trajCount)
{
    struct traj *tmp;
    for(int i = 0; i < trajCount; i++)
    {
        tmp = new struct traj;
        tmp->playerID = i;
        this->trj.push_back(tmp);
    }
}

void Tracker::match()
{
    if(this->imgInfo->tmpImg.empty()) { cout << "Please initialize!" << endl; return; }

    this->imgInfo->getTmpImg().copyTo(this->imgInfo->dispImg);
    Mat tmp;
    this->imgInfo->getTmpImg().copyTo(tmp);
    double maxVal;
    Point maxPt;
    struct traj *tmpTraj = this->trj[this->currentId];

    Rect roiRect = Rect(0, 0, tmpTraj->tmpl.cols, tmpTraj->tmpl.rows);
    Rect searchRect;
    Mat result;

    int roiX = 0, roiY = 0, roiW = 0, roiH = 0;

    if(this->isFirst)
    {
        matchTemplate(tmp, tmpTraj->tmpl, result, TM_CCOEFF_NORMED);
        normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
    }
    else
    {
        roiX = tmpTraj->derivX > 0 ? tmpTraj->currPt.x : tmpTraj->currPt.x - 2;
        roiY = tmpTraj->derivY > 0 ? tmpTraj->currPt.y : tmpTraj->currPt.y - 2;
        roiW = tmpTraj->occlusion ? tmpTraj->tmpl.cols + 10 : tmpTraj->tmpl.cols;
        roiH = tmpTraj->occlusion ? tmpTraj->tmpl.rows + 10 : tmpTraj->tmpl.rows;

        if(roiY < 0) roiY = 0;
        else if(roiY + roiH > tmp.rows) roiY = tmp.rows - roiH;
        if(roiX < 0) roiX = 0;
        else if(roiX + roiW > tmp.cols) roiX = tmp.cols - roiW;

        searchRect = Rect(roiX, roiY, roiW, roiH);

        matchTemplate(tmp(searchRect), tmpTraj->tmpl, result, TM_CCOEFF_NORMED);
        normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
    }
    minMaxLoc(result, NULL, &maxVal, NULL, &maxPt);

    if(this->isFirst)
    {
        roiRect.x = maxPt.x;
        roiRect.y = maxPt.y;
    }
    else
    {
        roiRect.x = maxPt.x + roiX;
        roiRect.y = maxPt.y + roiY;
    }

    /*
     * Display Rectangle and ID
     */
    rectangle(this->imgInfo->tmpImg, roiRect, Scalar(0, 255, 255), -1);
    stringstream pid;
    pid << this->currentId;
    putText(this->imgInfo->dispImg, pid.str(), Point(roiRect.x + 1, roiRect.y + 8), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
    rectangle(this->imgInfo->dispImg, roiRect, Scalar(255, 0, 0));

    if(this->isFirst)
    {
        tmpTraj->currPt = Point2d(roiRect.x, roiRect.y);
        tmpTraj->prevPt = Point2d(roiRect.x, roiRect.y);
    }
    else
    {
        swap(tmpTraj->currPt, tmpTraj->prevPt);
        tmpTraj->currPt = Point2d(roiRect.x, roiRect.y);
    }
    tmpTraj->derivX = tmpTraj->currPt.x - tmpTraj->prevPt.x;
    tmpTraj->derivY = tmpTraj->currPt.y - tmpTraj->prevPt.y;

    tmpTraj->precision = maxVal;
    tmpTraj->isCorrect = (maxVal > 0);
}

bool Tracker::validate()
{
    bool result = true;
    if(this->exFileName == NULL) result = false;
    if(this->imgInfo == NULL) result = false;
    if(this->trj.size() == 0) result = false;
    return result;
}
