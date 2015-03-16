#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent, QImage img1, QImage img2) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    this->img1 = img1;
    this->img2 = img2;
    this->pointNum = 0;
    this->imageNum = 0;

    this->ui->lblPoint->setText("click left upper corner.");

    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(img1));
    this->ui->graphicsView->setScene(scene);
    this->ui->graphicsView->update();
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::setPointImage()
{
    QPointF currPt = QPointF(this->ui->txtPoint->text().split(",")[0].toInt(), this->ui->txtPoint->text().split(",")[1].toInt());
    switch (this->pointNum) {
    case 0:
        if(this->imageNum == 0) this->srcPt1[0] = currPt;
        else this->srcPt2[0] = currPt;
        break;
    case 1:
        if(this->imageNum == 0) this->srcPt1[1] = currPt;
        else this->srcPt2[1] = currPt;
        break;
    case 2:
        if(this->imageNum == 0) this->srcPt1[2] = currPt;
        else this->srcPt2[2] = currPt;
        break;
    case 3:
        if(this->imageNum == 0)
        {
            this->srcPt1[3] = currPt;
            this->imageNum = 1;
            QGraphicsScene *scene = new QGraphicsScene;
            scene->addPixmap(QPixmap::fromImage(this->img2));
            this->ui->graphicsView->setScene(scene);
            this->ui->graphicsView->update();
        }
        else this->srcPt2[3] = currPt;
        break;
    default:
        break;
    }
}

void SettingDialog::on_btnOk_clicked()
{
    if(this->imageNum == 1 && this->pointNum == 3)
    {
        this->setResult(QDialog::Accepted);
        this->close();
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
