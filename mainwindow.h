#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdlib.h>

#include <QMainWindow>
#include <QGraphicsView>
#include <QMatrix>
#include <QMessageBox>
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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_btnChooseSrcFile_clicked();

    void on_btnChooseBgFile_clicked();

    void on_btnChooseExFile_clicked();

    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_btnTrain_clicked();

    void on_btnInit_clicked();

    void on_isAutomatic_stateChanged(int arg1);

    void on_btnCommit_clicked();

    void on_btnPreview_clicked();

private:
    bool initialized;
    bool stopFlag;
    int currentId;
    Point2d currPoint;
    Ui::MainWindow *ui;

    ImageInfo *imgInfo;
    Tracker *tracker;

    QGraphicsScene* scene;

    cv::Mat tmp;
    cv::Mat srcMat;
    QPixmap item;

    void setImg(QImage img);
    void tracking();
    void prepareTracker();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
