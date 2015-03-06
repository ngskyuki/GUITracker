#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdlib.h>

#include <QMainWindow>
#include <QGraphicsView>
#include <QMatrix>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "guiutils.h"
#include "templatematcher.h"
#include "emalgorithm.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setFrameCount(int frameCount);
    int getFrameCount();
    ~MainWindow();
private slots:

    void on_btnChooseSrcFile_clicked();

    void on_btnChooseBgFile_clicked();

    void on_btnChooseExFile_clicked();

    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_btnSetup_clicked();

    void on_btnTrain_clicked();

private:
    bool initialized;
    bool stopFlag;
    int frameCount;
    Ui::MainWindow *ui;
    TemplateMatcher *matcher;
    EMAlgorithm *model;
    cv::Mat tmp;
    cv::Mat srcMat;
    QPixmap item;
    QGraphicsScene* scene;
};

#endif // MAINWINDOW_H
