#include <QApplication>
#include <QPushButton>
#include <iostream>
#include "mainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MainWindow *window = new MainWindow();
    window->show();
    //QImage image;
    //image.load("/users/yuukifujita/develop/playertracking/playertracking/tmpl/tmpl1.png");
    //QLabel label;
    //label.setPixmap(QPixmap::fromImage(image));
    //label.show();
    return app.exec();
}
