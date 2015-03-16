#ifndef MYQGRAPHICSSCENE_H
#define MYQGRAPHICSSCENE_H

#include <QtGui>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

class MyQGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyQGraphicsScene(QObject *parent);
//private slots:
//    void mousePressed(QGraphicsSceneMouseEvent *event);
signals:
//    void mouseCoordinates(QPointF pt);
};

#endif // MYQGRAPHICSSCENE_H
