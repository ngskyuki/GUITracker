#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QWidget>
#include <QPointF>
#include <QDialog>
#include <QImage>
#include "guiutils.h"

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    SettingDialog(QWidget *parent, QImage img1, QImage img2);
    ~SettingDialog();
    void setPointImage();

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::SettingDialog *ui;
    QImage img1;
    QImage img2;
    int pointNum;
    int imageNum;

    QPointF srcPt1[4];
    QPointF dstPt1[4];
    QPointF srcPt2[4];
    QPointF dstPt2[4];

};

#endif // SETTINGDIALOG_H
