#include "guiutils.h"

GuiUtils::GuiUtils()
{

}

GuiUtils::~GuiUtils()
{

}

std::string GuiUtils::getFilePath(QWidget *parent)
{
    QFileDialog *dialog = new QFileDialog(parent);
    QString fileName = dialog->getOpenFileName(parent, QString("Open File"), QString(), QString());
    dialog->close();
    dialog->~QFileDialog();
    return fileName.toStdString();
}

void GuiUtils::showInFinder(QWidget *parent, const QString &filePath)
{
    Q_UNUSED(parent);
    QStringList args;
    args << "-e";
    args << "tell application \"Finder\"";
    args << "-e";
    args << "activate";
    args << "-e";
    args << "select POSIX file \"" + filePath + "\"";
    args << "-e";
    args << "end tell";
    QProcess::startDetached("osascript", args);
}

QImage GuiUtils::Mat2QImg(cv::Mat src)
{
    cv::Mat tmp;
    cv::cvtColor(src, tmp, cv::COLOR_BGR2RGB);
    QImage dst((const unsigned char*)tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGB888);
    dst.bits();
    return dst;
}

