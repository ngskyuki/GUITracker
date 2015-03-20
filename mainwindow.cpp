#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->initialized = false;
    ui->setupUi(this);

    qApp->installEventFilter(this);

    ui->graphicsView->setMouseTracking(true);
    string fileNames[] = {
        "/users/yuukifujita/develop/playertracking/playertracking/footballSampleLeft.mp4",
        "/users/yuukifujita/develop/playertracking/playertracking/footballSampleRight.mp4"
    };
    this->imgInfo = new ImageInfo(fileNames);
    this->tracker = new Tracker(this->imgInfo, 10);
}

MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        if(obj != this) return false;
        if(obj == this->ui->frameCountLabel ||
                obj == this->ui->constFrameLabel ||
                obj == this->ui->btnInit ||
                obj == this->ui->btnChooseExFile ||
                obj == this->ui->btnChooseSrcFile ||
                obj == this->ui->btnStart ||
                obj == this->ui->btnStop ||
                obj == this->ui->btnTrain) return false;
        QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
        if(mEvent->x() < 10 || mEvent->y() < 10) return false;
        if(mEvent->x() > 560 || mEvent->y() > 385) return false;
        this->ui->coordinates->setText(QString::number(mEvent->x()) + ", " + QString::number(mEvent->y()));
        return true;
    }
    return false;
}


void MainWindow::on_btnChooseSrcFile_clicked()
{
//    this->matcher->setLeftSrcFileName(GuiUtils::getFilePath(this));
//    this->matcher->setup();
}

void MainWindow::on_btnChooseBgFile_clicked()
{
//    this->matcher->setBgFileName(GuiUtils::getFilePath(this));
}

void MainWindow::on_btnChooseExFile_clicked()
{
    this->tracker->setExFileName(GuiUtils::getFilePath(this));
}

//void MainWindow::setCoordinates(QPointF pt)
//{
//    QString x = QString::number(pt.x());
//    QString y = QString::number(pt.y());

//    cout << "Coordinates are: " << x.toStdString() + " " << y.toStdString() << endl;
//}

void MainWindow::on_btnStart_clicked()
{
    this->stopFlag = false;
    if(!this->imgInfo->getInitialized())
    {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Please define corners of src image!"),
                             QMessageBox::Ok);
        return;
    }
    if(!(this->tracker->validate()))
    {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Please initialize Tracker!"),
                             QMessageBox::Ok);
        return;
    }
    char buff[20];

    if(this->tracker->getAutomatic())
    {
        while(!this->stopFlag)
        {
            if(this->currentId == this->tracker->getObjectNumber())
                this->prepareTracker();
            this->tracker->match();
            QImage image = GuiUtils::Mat2QImg(this->imgInfo->getDispImg());
            this->setImg(image);
            sprintf(buff, "%d", this->tracker->getFrameCount());
            ui->frameCountLabel->setText(QString(buff));
            qApp->processEvents();
            this->tracker->nextPlayer();
            this->currentId = this->tracker->getCurrentId();
        }
    }
}

void MainWindow::tracking()
{
    if(this->currentId == this->tracker->getObjectNumber())
        this->prepareTracker();
    this->currPoint.x = stod(this->ui->coordinates->text().split(",")[0].toStdString());
    this->currPoint.y = stod(this->ui->coordinates->text().split(",")[1].toStdString());
    this->tracker->track(this->currPoint);
}

void MainWindow::setImg(QImage img)
{
    this->item = QPixmap::fromImage(img);
    this->scene = new QGraphicsScene(this);
    this->scene->addPixmap(item);
    this->ui->graphicsView->setScene(scene);
    this->ui->graphicsView->update();
    this->ui->graphicsView->repaint();
    this->ui->graphicsView->show();
}

void MainWindow::on_btnStop_clicked()
{
    this->stopFlag = true;
}

void MainWindow::on_btnTrain_clicked()
{
//    this->model->applyEM(this->matcher->getTmpMat(), 3);
//    this->matcher->setTrained(true);
}

void MainWindow::prepareTracker()
{
    this->tracker->setCurrentId(0);
    this->currentId = this->tracker->getCurrentId();
    this->tracker->next();
    this->tracker->incrementTime();
}

void MainWindow::on_btnInit_clicked()
{
    if(!(this->imgInfo->validate()))
    {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Please initialize ImageInfo!"),
                             QMessageBox::Ok);
    }
    SettingDialog *dialog = new SettingDialog(this, GuiUtils::Mat2QImg(this->imgInfo->getTmpLeftImg()),
                                             GuiUtils::Mat2QImg(this->imgInfo->getTmpRightImg()));
    dialog->exec();
    if(dialog->result() == QDialog::Accepted)
    {
        this->imgInfo->setSrcPtLeft(dialog->getSrcPtLeft());
        this->imgInfo->setSrcPtRight(dialog->getSrcPtRight());
        this->imgInfo->setInitialized(true);
        this->initialized = true;
    }
}

void MainWindow::on_isAutomatic_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
        this->tracker->setAutomatic(true);
    else
        this->tracker->setAutomatic(false);

}

void MainWindow::on_btnCommit_clicked()
{
    vector<struct exData*>::iterator begin, end;
    begin = this->tracker->getExData()->begin();
    end = this->tracker->getExData()->end();

    for(; begin != end; begin++)
    {
        this->tracker->exportData(*begin);
    }
}

void MainWindow::on_btnPreview_clicked()
{
    vector<struct exData*> *tmp = this->tracker->getExData();
    tmp->pop_back();
    if(this->tracker->getCurrentId() == 0)
    {
        this->tracker->setTimeStamp(this->tracker->getTimeStamp() - 0.03);
        this->tracker->setCurrentId(this->tracker->getObjectNumber());
    }
    else
        this->tracker->setCurrentId(this->tracker->getCurrentId() - 1);
}
