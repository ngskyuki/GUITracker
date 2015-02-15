#ifndef GUIUTILS_H
#define GUIUTILS_H

#include <QtCore>
#include <QWidget>
#include <QImage>
#include <QFileDialog>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class GuiUtils
{
public:
    GuiUtils();
    ~GuiUtils();
    static std::string getFilePath(QWidget *parent);
    static void showInFinder(QWidget *parent, const QString &filePath);
    static QImage Mat2QImg(cv::Mat src);
};

#endif // GUIUTILS_H
