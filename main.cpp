#include <QApplication>
#include <QPushButton>
#include <iostream>
#include "mainwindow.h"
#include <fstream>

#include <vector>
using namespace std;

struct test {
    double ts;
    double x;
    double y;
};

int main(int argc, char** argv)
{

    std::vector<struct test*> *tbox = new vector<struct test*>();
    struct test *t;
    for(int i = 0; i < 10; i++) {
         t = new struct test;
         t->ts = 1.0 * i;
         t->x = 1.0 * (i + 3);
         t->y = 1.0 * (i + 5);
         tbox->push_back(t);
    }
    std::vector<struct test*>::iterator begin, end;
    begin = tbox->begin();
    end = tbox->end();

    ofstream ofs;
    ofs.open("/users/yuukifujita/develop/test.csv", ios_base::app);
    for(; begin != end; begin++)
    {
        ofs << (*begin)->ts << ",";
        ofs << (*begin)->x << ",";
        ofs << (*begin)->y << endl;
    }
    ofs.close();

//    QApplication app(argc, argv);
//    MainWindow *window = new MainWindow();
//    window->show();
//    return app.exec();
    return 1;
}
