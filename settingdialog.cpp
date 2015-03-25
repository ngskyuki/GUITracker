#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent, QImage imgLeft, QImage imgRight, QSize size) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);
    this->imgLeft = imgLeft;
    this->imgRight = imgRight;
    this->pointNum = 0;
    this->imageNum = 0;
    this->ui->graphicsView->resize(size);
    this->ui->constLblPoint->setText("Left Upper Corner");

    this->width = size.width();
    this->height = size.height();

    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(imgLeft));
    this->ui->graphicsView->setScene(scene);
    this->ui->graphicsView->update();
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

int SettingDialog::getWidth() { return this->width; }

int SettingDialog::getHeight() { return this->height; }

Point2f *SettingDialog::getSrcPtLeft() { return this->srcPtLeft; }

Point2f *SettingDialog::getSrcPtRight() { return this->srcPtRight; }

bool SettingDialog::eventFilter(QObject *obj, QEvent *event)
{
    if(obj != this) return false;
    if(obj == this->ui->btnCancel ||
            obj == this->ui->btnOk ||
            obj == this->ui->constLblPoint ||
            obj == this->ui->txtPoint) return false;
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mEvent = static_cast<QMouseEvent*>(event);
        if(mEvent->x() < 10 || mEvent->y() < 10) return false;
        if(mEvent->x() > this->width + 10 || mEvent->y() > this->height + 10) return false;
        this->ui->txtPoint->setText(QString::number(mEvent->x() - this->ui->graphicsView->x())
                                    + ", "
                                    + QString::number(mEvent->y() - this->ui->graphicsView->y()));
        return true;
    }
    return false;
}

void SettingDialog::setPointImage()
{
    Point2f currPt = Point2f(this->ui->txtPoint->text().split(",")[0].toInt(), this->ui->txtPoint->text().split(",")[1].toInt());
    switch (this->pointNum) {
    case 0:
        if(this->imageNum == 0)
            this->srcPtLeft[0] = currPt;
        else
            this->srcPtRight[0] = currPt;

        this->ui->constLblPoint->setText("Left Lower Corner");
        break;
    case 1:
        if(this->imageNum == 0)
            this->srcPtLeft[1] = currPt;
        else
            this->srcPtRight[1] = currPt;

        this->ui->constLblPoint->setText("Right Lower Corner");
        break;
    case 2:
        if(this->imageNum == 0)
            this->srcPtLeft[2] = currPt;
        else
            this->srcPtRight[2] = currPt;

        this->ui->constLblPoint->setText("Right Upper Corner");
        break;
    case 3:
        if(this->imageNum == 0)
        {
            this->srcPtLeft[3] = currPt;
            this->imageNum = 1;
            this->pointNum = -1;
            QGraphicsScene *scene = new QGraphicsScene;
            scene->addPixmap(QPixmap::fromImage(this->imgRight));
            this->ui->graphicsView->setScene(scene);
            this->ui->graphicsView->update();
            this->ui->constLblPoint->setText("Left Upper Corner");
        }
        else this->srcPtRight[3] = currPt;
        break;
    default:
        break;
    }
}

void SettingDialog::on_btnOk_clicked()
{
//    if(true)
    if(this->imageNum == 1 && this->pointNum == 3)
    {
//        if(false)
        if(!(this->validate()))
        {
            QMessageBox::warning(this, tr("Warning!"),
                                 tr("Please retry to input corner!"),
                                 QMessageBox::Ok);
            return;
        }
        this->setPointImage();
        this->accept();
        return;
    }
    if(this->ui->txtPoint->text() == "") return;
    this->setPointImage();
    this->pointNum++;
}

void SettingDialog::on_btnCancel_clicked()
{
    this->setResult(QDialog::Rejected);
    this->close();
}

bool SettingDialog::validate()
{
    return true;
}
