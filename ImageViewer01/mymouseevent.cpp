#include "mymouseevent.h"

MyMouseEvent::MyMouseEvent(QGraphicsPixmapItem *parent) :
    QGraphicsPixmapItem(parent),
    rate(0.1)
{
}

void MyMouseEvent::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

/**
 * 重写鼠标拖拽事件
 */
void MyMouseEvent::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    setPos(pos() + mapToParent(event->pos()) - mapToParent(event->lastPos()));//拖拽位置计算
}

/**
 * 重写滚轮事件
 */
void MyMouseEvent::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    int delta = event->delta();
    double factor = scale();
    if(delta > 0)//放大
    {
        factor *= (1 + rate);//rate为缩放率
    }
    else if (delta < 0)//缩小
    {
        factor *= (1 - rate);
    }
    //将缩放中心设置在Item中心
    setTransformOriginPoint(boundingRect().width()/2, boundingRect().height()/2);
    //设置缩放后大小
    setScale(factor);
}

/**
 * 设置图元位置为（0，0），初始比例因素1.0
 */
void MyMouseEvent::reset()
{
    setPos(0,0);
    setScale(1.0);
}
