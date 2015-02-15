#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMatrix>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "guiutils.h"
#include "templatematcher.h"
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
    ~MainWindow();
private slots:

    void on_btnChooseSrcFile_clicked();

    void on_btnChooseBgFile_clicked();

    void on_btnChooseExFile_clicked();

    void on_btnStart_clicked();

    void on_btnStop_clicked();

private:
    bool stopFlag;
    bool initialized;
    Ui::MainWindow *ui;
    TemplateMatcher *matcher;

    cv::Mat tmp;
    cv::Mat srcMat;
    QPixmap item;
    QGraphicsScene* scene;
};

#endif // MAINWINDOW_H
