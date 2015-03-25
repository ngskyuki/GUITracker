#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->initialized = false;
    ui->setupUi(this);
    namedWindow("TrackBar");
    qApp->installEventFilter(this);

    ui->graphicsView->setMouseTracking(true);
    string fileNames[] = {
        "/users/yuukifujita/develop/playertracking/playertracking/footballSampleLeft.mp4",
        "/users/yuukifujita/develop/playertracking/playertracking/footballSampleRight.mp4"
    };
    this->imgInfo = new ImageInfo(fileNames);
    cout << "FPS is: " << this->imgInfo->getCapture()[0].get(CAP_PROP_FPS) << endl;
    this->tracker = new Tracker(this->imgInfo, 10);
    this->ui->horizontalSlider->setMaximum(this->imgInfo->getCapture()[0].get(CAP_PROP_FRAME_COUNT));
    this->ui->horizontalSlider->setMinimum(0);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    this->setCapturePosition(value);
    this->tracker->setFrameCount(
                (int)this->imgInfo->getCapture()[0].get(CAP_PROP_POS_FRAMES)
            );
    this->tracker->incrementTime();
    this->setImg(GuiUtils::Mat2QImg(this->imgInfo->getDispImg()));
}

void MainWindow::setCapturePosition(int pos)
{
    //this->sendData();
    this->imgInfo->setCaptureNumber((double) pos - 1.0);
    this->tracker->next();
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
        if(mEvent->x() > 537 || mEvent->y() > 260) return false;
        this->ui->coordinates->setText(QString::number(mEvent->x() - this->ui->graphicsView->x())
                                       + ", "
                                       + QString::number(mEvent->y() - this->ui->graphicsView->y()));
        if(!(this->stopFlag))
        {
            this->tracking(Point2f(mEvent->x() - this->ui->graphicsView->x(), mEvent->y() - this->ui->graphicsView->y()));
//            this->tracker->track(Point2f(mEvent->x() - this->ui->graphicsView->x(), mEvent->y() - this->ui->graphicsView->y()));
        }
        return true;
    }
    return false;
}


void MainWindow::on_btnChooseSrcFile_clicked()
{
//    this->matcher->setLeftSrcFileName(GuiUtils::getFilePath(this));
//    this->matcher->setup();
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
    this->ui->btnStart->setEnabled(false);
    this->ui->btnStop->setEnabled(true);

    /*Validation area*/
    if(!(this->imgInfo->getInitialized()))
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

    /*Tracking start*/
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
            sprintf(buff, "%d", (int)this->tracker->getFrameCount());
            ui->frameCountLabel->setText(QString(buff));
            qApp->processEvents();
            this->tracker->nextPlayer();
            this->currentId = this->tracker->getCurrentId();
        }
    }
    else
    {
        if(this->tracker->getExData() == NULL)
        {
            vector<struct exData*> *tmp = new vector<struct exData*>();
            this->tracker->setExData(tmp);
        }
        /*TODO: Implement process of manual traking
         *like show message that tell user to start tracking
         *with click to pick position of current player.
         */
    }
}

void MainWindow::tracking(Point2f pt)
{
    if(this->tracker->getCurrentId() == this->tracker->getObjectNumber())
        this->prepareTracker();
    this->currPoint.x = stod(this->ui->coordinates->text().split(",")[0].toStdString());
    this->currPoint.y = stod(this->ui->coordinates->text().split(",")[1].toStdString());
    this->tracker->track(pt);
    this->currentId = this->tracker->getCurrentId();
    this->setImg(GuiUtils::Mat2QImg(this->imgInfo->getDispImg()));
}

void MainWindow::setImg(QImage img)
{
//    this->ui->graphicsView->resize(img.width, img.height);
    this->ui->isAutomatic->setEnabled(false);
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
    this->ui->btnStart->setEnabled(true);
    this->ui->btnStop->setEnabled(false);
    this->ui->isAutomatic->setEnabled(true);
    this->sendData();
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
    this->setImg(GuiUtils::Mat2QImg(this->imgInfo->getDispImg()));
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
                                             GuiUtils::Mat2QImg(this->imgInfo->getTmpRightImg()),
                                              QSize(this->imgInfo->getCapture()[0].get(CAP_PROP_FRAME_WIDTH),
                                                    this->imgInfo->getCapture()[0].get(CAP_PROP_FRAME_HEIGHT)));
    dialog->exec();
    if(dialog->result() == QDialog::Accepted)
    {
        this->imgInfo->setSrcPtLeft(dialog->getSrcPtLeft());
        this->imgInfo->setSrcPtRight(dialog->getSrcPtRight());
        this->imgInfo->setDstSize(Size(537, 269));
        this->imgInfo->setInitialized(true);
        this->initialized = true;

        this->imgInfo->setup();
        this->imgInfo->setDispImg(this->imgInfo->getTmpImg());
        this->setImg(GuiUtils::Mat2QImg(this->imgInfo->getDispImg()));
    }
}

void MainWindow::on_isAutomatic_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        this->tracker->setAutomatic(true);
        if(!(this->ui->btnStart->isEnabled()))
            this->ui->btnStart->setEnabled(true);
        if(this->tracker->getCurrentId() - this->currentId > 0)
        {
            cout << "Please track other ";
            cout << this->tracker->getObjectNumber() - this->currentId;
            cout << " number objects. Then can track automatic." << endl;
        }
    }
    else
    {
        this->tracker->setAutomatic(false);
        if(!(this->ui->btnStart->isEnabled()))
            this->ui->btnStart->setEnabled(true);
    }
}

void MainWindow::on_btnCommit_clicked()
{
    this->sendData();
}

void MainWindow::sendData()
{
    if(this->tracker->getExData() == NULL || this->tracker->getExData()->size() == 0)
        return;
    vector<struct exData*>::iterator begin, end;
    begin = this->tracker->getExData()->begin();
    end = this->tracker->getExData()->end();

    for(; begin != end; begin++)
    {
        this->tracker->exportData(*begin);

        vector<struct exData*> *newVec = new vector<struct exData*>();
        this->tracker->setExData(newVec);
    }
}

void MainWindow::on_btnPreview_clicked()
{
    vector<struct exData*> *tmp = this->tracker->getExData();
    tmp->pop_back();
    if(this->tracker->getCurrentId() == 0)
    {
        this->tracker->setFrameCount(this->tracker->getFrameCount() - 1);
        this->tracker->setTimeStamp(this->tracker->getTimeStamp() - 0.03);
        this->tracker->setCurrentId(this->tracker->getObjectNumber());
        this->imgInfo->setCaptureNumber(this->imgInfo->getCaptureNumber() - 1);
    }
    else
        this->tracker->setCurrentId(this->tracker->getCurrentId() - 1);
}


