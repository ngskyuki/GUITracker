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

    this->ui->btnStop->setEnabled(false);

    ui->graphicsView->setMouseTracking(true);
    this->imgInfo = new ImageInfo();
    this->imgInfo->setSourceNumber(ImageInfo::SourceNumber::One);
    this->tracker = new Tracker(this->imgInfo);
//    this->imgInfo = new ImageInfo(fileNames);
//    this->tracker = new Tracker(this->imgInfo, 10);
//    this->ui->horizontalSlider->setMaximum(this->imgInfo->getCapture()[0].get(CAP_PROP_FRAME_COUNT));
//    this->ui->horizontalSlider->setMinimum(0);
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
                obj == this->ui->btnStart ||
                obj == this->ui->btnStop ||
                obj == this->ui->btnTrain) return false;
        QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
        if(mEvent->x() < this->ui->graphicsView->x() ||
                mEvent->y() < this->ui->graphicsView->y()) return false;
        if(mEvent->x() > this->ui->graphicsView->width() + this->ui->graphicsView->x() ||
                mEvent->y() > this->ui->graphicsView->height() + this->ui->graphicsView->y()) return false;
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

void MainWindow::init()
{
    this->fileNames[0] = this->leftFileName;
    if(this->imgInfo->getSourceNumber() == ImageInfo::SourceNumber::Two)
        this->fileNames[1] = this->rightFileName;
    this->imgInfo->setCapture(this->fileNames);
    this->imgInfo->setup(true);
    this->ui->horizontalSlider->setMaximum(this->imgInfo->getCapture()[0].get(CAP_PROP_FRAME_COUNT));
    this->ui->horizontalSlider->setMinimum(0);
}

void MainWindow::on_btnChooseSrcLeftFile_clicked()
{
    this->leftFileName = GuiUtils::getFilePath(this);
    if(this->imgInfo->getSourceNumber() == ImageInfo::SourceNumber::One && this->leftFileName != "")
    {
        this->init();
        return;
    }
    if(this->leftFileName != "" && this->rightFileName != "")
        this->init();
}

void MainWindow::on_btnChooseSrcRightFile_clicked()
{
    if(this->imgInfo->getSourceNumber() == ImageInfo::SourceNumber::One) {
        QMessageBox::warning(this, tr("Warning!"), tr("Please set Source Number '2'."),
                            QMessageBox::Ok);
        return;
    }
    this->rightFileName = GuiUtils::getFilePath(this);
    if(this->leftFileName != "" && this->rightFileName != "")
        this->init();
}

void MainWindow::on_btnChooseExFile_clicked() { this->tracker->setExFileName(GuiUtils::getFilePath(this)); }

//void MainWindow::setCoordinates(QPointF pt)
//{
//    QString x = QString::number(pt.x());
//    QString y = QString::number(pt.y());

//    cout << "Coordinates are: " << x.toStdString() + " " << y.toStdString() << endl;
//}

void MainWindow::on_btnStart_clicked()
{
    /*Validation area*/
    if(this->tracker->getObjectNumber() == 0)
    {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Please set object number!"),
                             QMessageBox::Ok);
        return;
    }
    if(!this->initialized)
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
    if(this->tracker->getExFileName() == "")
    {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Please set export file name!"),
                             QMessageBox::Ok);
        return;
    }

    this->stopFlag = false;
    this->ui->btnStart->setEnabled(false);
    this->ui->btnStop->setEnabled(true);
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
    if(this->leftFileName == "" && this->rightFileName == "" ||
            this->imgInfo->getSourceNumber() == ImageInfo::SourceNumber::Two && this->rightFileName == "")
    {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Please choose source file!"),
                             QMessageBox::Ok);
        return;
    }
    if(this->imgInfo == NULL)
    {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Please choose source file."),
                             QMessageBox::Ok);
    }
    if(!(this->imgInfo->validate()))
    {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Please initialize ImageInfo!"),
                             QMessageBox::Ok);
    }
    SettingDialog *dialog = new SettingDialog(this,
                                              this->imgInfo->getSourceNumber() == ImageInfo::SourceNumber::One ? 1 : 2,
                                              GuiUtils::Mat2QImg(this->imgInfo->getTmpLeftImg()),
                                              GuiUtils::Mat2QImg(this->imgInfo->getTmpRightImg()),
                                              QSize(this->imgInfo->getCapture()[0].get(CAP_PROP_FRAME_WIDTH),
                                                    this->imgInfo->getCapture()[0].get(CAP_PROP_FRAME_HEIGHT)));
    dialog->exec();
    if(dialog->result() == QDialog::Accepted)
    {
        this->imgInfo->setSrcPtLeft(dialog->getSrcPtLeft());
        if(this->imgInfo->getSourceNumber() == ImageInfo::SourceNumber::Two)
            this->imgInfo->setSrcPtRight(dialog->getSrcPtRight());
        this->imgInfo->setDstSize(Size(537, 269));
        this->imgInfo->setWarpPerspective();
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

void MainWindow::on_btnOk_clicked()
{
    int objNum = this->ui->txtObjectNumber->text().toInt();
    this->tracker->setObjectNumber(objNum);
}

void MainWindow::on_btnSouceNumberOk_clicked()
{
    this->imgInfo->setSourceNumber(this->ui->txtSourceNumber->text().toInt() == 1 ? ImageInfo::SourceNumber::One : ImageInfo::SourceNumber::Two);
}
