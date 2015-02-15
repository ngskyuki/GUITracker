#include "templatematcher.h"

TemplateMatcher::TemplateMatcher()
{
    this->srcPt[0] = Point2f(202.0, 137.0);
    this->srcPt[1] = Point2f(38.0, 279.0);
    this->srcPt[2] = Point2f(687.0, 279.0);
    this->srcPt[3] = Point2f(522.0, 137.0);
    this->dstPt[0] = Point2f(0.0, 0.0);
    this->dstPt[1] = Point2f(0, 375);
    this->dstPt[2] = Point2f(550, 375);
    this->dstPt[3] = Point2f(550, 0);
    this->dstSize = Size(550, 375);

    this->trj = this->prepareTraj();
    this->prepareTemplates();
    this->timeStamp = 0.0;
    this->frameCount = 0;
}
TemplateMatcher::~TemplateMatcher(){}

void TemplateMatcher::setup()
{
    this->capture = VideoCapture(this->srcFileName);
}
void TemplateMatcher::setSrcFileName(string fileName)
{
    this->srcFileName = fileName;
}
string TemplateMatcher::getSrcFileName()
{
    return this->srcFileName;
}
void TemplateMatcher::setBgFileName(string fileName)
{
    this->bgFileName = fileName;
}
string TemplateMatcher::getBgFileName()
{
    return this->bgFileName;
}
void TemplateMatcher::setExFileName(string fileName)
{
    this->exFileName = fileName;
}
string TemplateMatcher::getExFileName()
{
    return this->exFileName;
}
void TemplateMatcher::setCapture(VideoCapture videoCapture)
{
    this->capture = videoCapture;
}
VideoCapture TemplateMatcher::getCapture()
{
    return this->capture;
}
void TemplateMatcher::setSrcPt(Point2f *srcPt[])
{
    for(int i = 0; i < 4; i++)
    {
        this->srcPt[i] = *srcPt[i];
    }
}
Point2f* TemplateMatcher::getSrcPt()
{
    return this->srcPt;
}
void TemplateMatcher::setDstPt(Point2f **dstPt)
{
    for(int i = 0; i < 4; i++)
    {
        this->dstPt[i] = *dstPt[i];
    }
}
Point2f* TemplateMatcher::getDstPt()
{
    return this->dstPt;
}
void TemplateMatcher::setDstSize(Size size)
{
    this->dstSize = size;
}
Size TemplateMatcher::getDstSize()
{
    return this->dstSize;
}
void TemplateMatcher::setTmpMat(Mat tmpMat)
{
    this->tmpMat = tmpMat;
}
Mat TemplateMatcher::getTmpMat()
{
    return this->tmpMat;
}
void TemplateMatcher::setBg(Mat bg)
{
    this->bg = bg;
}
Mat TemplateMatcher::getBg()
{
    return this->bg;
}
void TemplateMatcher::setTmpDispMat(Mat tmpDispMat)
{
    this->tmpDispMat = tmpDispMat;
}
Mat TemplateMatcher::getTmpDispMat()
{
    return this->tmpDispMat;
}
void TemplateMatcher::setTmpRoiRect(Rect tmpRoiRect)
{
    this->tmpRoiRect = tmpRoiRect;
}
Rect TemplateMatcher::getTmpRoiRect()
{
    return this->tmpRoiRect;
}
void TemplateMatcher::setTrj(vector<struct traj> trj)
{
    this->trj = trj;
}
vector<struct traj> TemplateMatcher::getTrj()
{
    return this->trj;
}
void TemplateMatcher::setTimeStamp(double timeStamp)
{
    this->timeStamp = timeStamp;
}
double TemplateMatcher::getTimeStamp()
{
    return this->timeStamp;
}
void TemplateMatcher::setFrameCount(int frameCount)
{
    this->frameCount = frameCount;
}
int TemplateMatcher::getFrameCount()
{
    return this->frameCount;
}
void TemplateMatcher::Next()
{
    this->capture >> this->tmpMat;
    if(tmpMat.empty()) cout << "Video is empty!" << endl;
    this->transHomography();
}

void TemplateMatcher::transHomography()
{
    Mat homographyMtx = getPerspectiveTransform(this->srcPt, this->dstPt);
    warpPerspective(this->tmpMat, this->tmpMat, homographyMtx, this->getDstSize(), INTER_LANCZOS4 + WARP_FILL_OUTLIERS);
}

void TemplateMatcher::incrementTime()
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

void TemplateMatcher::exportData() {
    ofstream ofs;
    QFile f(QString(this->getExFileName().c_str()));
    ofs.open(f.fileName().toStdString().c_str());
   //basic_ofstream<char> ofs(this->getExFileName(), basic_ios::app);
    vector<struct traj>::iterator begin, end;
    begin = this->trj.begin();
    end = this->trj.end();
    for(; begin != end; begin++) {
        ofs << this->timeStamp << ",";
        ofs << begin->playerID << ",";
        ofs << begin->currPt.x << ",";
        ofs << begin->currPt.y << ",";
        ofs << begin->precision << ",";
        ofs << begin->isCorrect << ",";
        ofs << begin->occlusion << endl;
    }
    ofs.close();
    return;
}

void TemplateMatcher::match()
{
    if(this->tmpMat.empty())
    {
        cout << "Please initialize tmpMat" << endl;
        return;
    }
    this->tmpMat.copyTo(this->tmpDispMat);
    Mat tmpMat;
    this->tmpMat.copyTo(tmpMat);
    this->transHomography();
    for(size_t i = 0; i < this->trj.size(); i++)
    {
        Mat result;
        Rect searchRect;
        Rect roiRect = Rect(0, 0, this->trj[i].tmpl.cols, this->trj[i].tmpl.rows);
        int roiX = 0, roiY = 0, roiW = 0, roiH = 0;
        bool isFirst = true;
        double maxVal;
        Point maxPt;

        if(isFirst)
        {
            matchTemplate(tmpMat, this->trj[i].tmpl, result, TM_CCOEFF_NORMED);
            normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
        }
        else
        {
            roiX = this->trj[i].derivX > 0 ? this->trj[i].currPt.x : this->trj[i].currPt.x - 2;
            roiY = this->trj[i].derivY > 0 ? this->trj[i].currPt.y : this->trj[i].currPt.y - 2;
            roiW = trj[i].occlusion ? this->trj[i].tmpl.cols + 10 : this->trj[i].tmpl.cols;
            roiH = trj[i].occlusion ? this->trj[i].tmpl.rows + 10 : this->trj[i].tmpl.rows;

            if(roiY < 0) roiY = 0;
            else if(roiY > this->trj[i].tmpl.rows) roiY = 0;
            if(roiX > 0) roiX = 0;
            else if(roiX > this->trj[i].tmpl.cols) roiX = 0;

            searchRect = Rect(roiX, roiY, roiW, roiH);
            matchTemplate(tmpMat(searchRect), this->trj[i].tmpl, result, TM_CCOEFF_NORMED);
            normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
        }
        minMaxLoc(result, NULL, &maxVal, NULL, &maxPt);
        if(isFirst)
        {
            roiRect.x = maxPt.x;
            roiRect.y = maxPt.y;
        }
        else
        {
            roiRect.x = maxPt.x + roiX;
            roiRect.y = maxPt.y + roiY;
        }
        rectangle(tmpMat, roiRect, Scalar(0, 255, 255), -1);
        std::string pid = std::to_string(trj[i].playerID);
        putText(this->tmpDispMat, pid, Point(roiRect.x + 1, roiRect.y + 8), FONT_HERSHEY_SIMPLEX, 1.2, Scalar(0, 0, 255));
        rectangle(this->tmpDispMat, roiRect, Scalar(255, 0, 0));

        if(isFirst) trj[i].currPt = Point2d(roiRect.x, roiRect.y);
        else
        {
            swap(this->trj[i].prevPt, this->trj[i].currPt);
            this->trj[i].currPt = Point2f(roiRect.x, roiRect.y);

            this->trj[i].derivX = this->trj[i].currPt.x - this->trj[i].prevPt.x;
            this->trj[i].derivY = this->trj[i].currPt.y - this->trj[i].prevPt.y;

            this->trj[i].precision = maxVal;
            this->trj[i].isCorrect = (maxVal > 0);
        }
    }
    vector<struct traj>::iterator begin, end, tbegin, tend;
    begin = this->trj.begin();
    end = this->trj.end();
    for(; begin != end; begin++)
    {
        bool occlusion = false;
        tbegin = this->trj.begin();
        tend = this->trj.end();
        for(; tbegin != tend; tbegin++)
        {
            if(abs(tbegin->currPt.x - begin->currPt.x < 4) &&
               abs(tbegin->currPt.y - begin->currPt.y < 15)) occlusion = true;
        }
        begin->occlusion = occlusion;
    }
}

void TemplateMatcher::prepareTemplates()
{
    vector<string> templateNames;
    string tmplRepos = "/Users/yuukifujita/Develop/PlayerTracking/PlayerTracking/tmpl/";
    for(int i = 1; i < 21; i++) {
        stringstream ss;
        ss << i;
        templateNames.push_back(tmplRepos + "tmpl" + ss.str() + ".png");
    }
    vector<Mat> templates;
    for(size_t i = 0; i < templateNames.size(); i++)
    {
        this->trj[i].tmpl = imread(templateNames[i], 21);
    }
}

vector<struct traj> TemplateMatcher::prepareTraj()
{
    vector<struct traj> trj;

    struct traj trj1;
    struct traj trj2;
    struct traj trj3;
    struct traj trj4;
    struct traj trj5;
    struct traj trj6;
    struct traj trj7;
    struct traj trj8;
    struct traj trj9;
    struct traj trj10;

    struct traj trj11;
    struct traj trj12;
    struct traj trj13;
    struct traj trj14;
    struct traj trj15;
    struct traj trj16;
    struct traj trj17;
    struct traj trj18;
    struct traj trj19;
    struct traj trj20;

    trj.push_back(trj1);
    trj.push_back(trj2);
    trj.push_back(trj3);
    trj.push_back(trj4);
    trj.push_back(trj5);
    trj.push_back(trj6);
    trj.push_back(trj7);
    trj.push_back(trj8);
    trj.push_back(trj9);
    trj.push_back(trj10);
    trj.push_back(trj11);
    trj.push_back(trj12);
    trj.push_back(trj13);
    trj.push_back(trj14);
    trj.push_back(trj15);
    trj.push_back(trj16);
    trj.push_back(trj17);
    trj.push_back(trj18);
    trj.push_back(trj19);
    trj.push_back(trj20);

    vector<struct traj>::iterator begin, end;
    begin = trj.begin();
    end = trj.end();
    int pId = 1;
    for(; begin != end; begin++) {
        begin->playerID = pId;
        pId++;
    }
    return trj;
}
