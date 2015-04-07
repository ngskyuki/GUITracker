#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QWidget>
#include <QPointF>
#include <QDialog>
#include <QImage>
#include <QMouseEvent>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "guiutils.h"

using namespace cv;

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    SettingDialog(QWidget *parent, int frameNum, QImage imgLeft, QImage imgRight, QSize size);
    ~SettingDialog();
    void setPointImage();
    int getWidth();
    int getHeight();
    Point2f *getSrcPtLeft();
    Point2f *getSrcPtRight();
    bool validate();

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::SettingDialog *ui;
    QImage imgLeft;
    QImage imgRight;
    int pointNum;
    int imageNum;

    int width;
    int height;

    Point2f currPt;

    Point2f srcPtLeft[4];
    Point2f dstPtLeft[4];
    Point2f srcPtRight[4];
    Point2f dstPt2Right[4];

    int frameNumber;
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // SETTINGDIALOG_H
