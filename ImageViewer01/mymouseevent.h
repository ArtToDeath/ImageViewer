#ifndef MYMOUSEEVENT_H
#define MYMOUSEEVENT_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>

class MyMouseEvent : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit MyMouseEvent(QGraphicsPixmapItem *parent = 0);

    const double rate;
    void reset();

signals:
private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
public slots:

};

#endif // MYMOUSEEVENT_H
