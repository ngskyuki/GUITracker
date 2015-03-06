#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->initialized = false;
    ui->setupUi(this);
    this->model = new EMAlgorithm();
    matcher = new TemplateMatcher();
    matcher->setSrcFileName("/users/yuukifujita/develop/playertracking/playertracking/uru1stfull.mp4");
    matcher->setBgFileName("/users/yuukifujita/develop/tracking/tracking/background.png");
    matcher->setExFileName("/users/yuukifujita/develop/tracking/tracking/export.csv");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setFrameCount(int frameCount)
{
    this->frameCount = frameCount;
}

int MainWindow::getFrameCount()
{
    return this->frameCount;
}

void MainWindow::on_btnChooseSrcFile_clicked()
{
    this->matcher->setSrcFileName(GuiUtils::getFilePath(this));
    this->matcher->setup();
}

void MainWindow::on_btnChooseBgFile_clicked()
{
    this->matcher->setBgFileName(GuiUtils::getFilePath(this));
}

void MainWindow::on_btnChooseExFile_clicked()
{
    this->matcher->setExFileName(GuiUtils::getFilePath(this));
}

void MainWindow::on_btnStart_clicked()
{
    if(!initialized)
    {
        this->setFrameCount(0);
        this->matcher->setup();
        this->initialized = true;
    }
    if(this->stopFlag) stopFlag = false;
    char buff[20];
    while(!this->stopFlag)
    {\
        this->setFrameCount(this->frameCount + 1);
        //this->scene = new QGraphicsScene(0, 0, 550, 375, ui->graphicsView);
        matcher->Next();
        matcher->match();
        this->srcMat = matcher->getTmpDispMat();
        //QImage image = GuiUtil::Mat2QImg(srcMat);
        cv::cvtColor(this->srcMat, this->tmp, cv::COLOR_BGR2RGB);
        QImage image((const unsigned char*)tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGB888);
        this->item = QPixmap::fromImage(image);
        this->scene = new QGraphicsScene();
        this->scene->addPixmap(item);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->update();
        ui->graphicsView->repaint();
        ui->graphicsView->show();
        sprintf(buff, "%d", this->getFrameCount());
        ui->frameCountLabel->setText(QString(buff));
        qApp->processEvents();
    }
}

void MainWindow::on_btnStop_clicked()
{
    this->stopFlag = true;
}

void MainWindow::on_btnSetup_clicked()
{
    if(!this->initialized)
    {
        this->setFrameCount(0);
        this->initialized = true;
        this->matcher->setup();
        this->matcher->Next();
        this->matcher->setTmpMat(this->matcher->transHomography());
        this->matcher->initCapture();
    }
    if(this->initialized)
    {
        cout << "Initialize is done already!" << endl;
    }
}

void MainWindow::on_btnTrain_clicked()
{
    this->model->applyEM(this->matcher->getTmpMat(), 3);
    this->matcher->setTrained(true);
}
