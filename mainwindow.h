#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdlib.h>

#include <QMainWindow>
#include <QGraphicsView>
#include <QMatrix>
#include <QMessageBox>
#include <QFileDialog>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QGraphicsSceneMouseEvent>
#include "guiutils.h"
#include "settingdialog.h"
#include "tracker.h"
#include "imageinfo.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setImg(QImage img);
private slots:

    void on_btnChooseSrcLeftFile_clicked();

    void on_btnChooseSrcRightFile_clicked();

    void on_btnChooseExFile_clicked();

    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_btnTrain_clicked();

    void on_btnInit_clicked();

    void on_isAutomatic_stateChanged(int arg1);

    void on_btnCommit_clicked();

    void on_btnPreview_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_btnOk_clicked();

private:
    bool initialized;
    bool stopFlag;
    int currentId;
    int position;
    string leftFileName;
    string rightFileName;

    string fileNames[2];
    Point2d currPoint;
    Ui::MainWindow *ui;

    ImageInfo *imgInfo;
    Tracker *tracker;

    QGraphicsScene* scene;

    cv::Mat tmp;
    cv::Mat srcMat;
    QPixmap item;

    void init();
    void tracking(Point2f pt);
    void prepareTracker();
    void setCapturePosition(int pos);
    void sendData();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
