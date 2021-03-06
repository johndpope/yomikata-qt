#ifndef SCROLLER_H
#define SCROLLER_H

#include <QObject>

#include <QSizeF>
#include <QPointF>
#include <QTime>

class QWidget;

class Scroller: public QObject
{
    Q_OBJECT

public:
    Scroller(QObject *parent);
    ~Scroller();

    void reset(const QSize &extent);
    void update(const QSize &extent);

    QPoint position();

    void mouseMoved(const QPointF &pos);
    void resetMouse();

signals:
    void enableRefresh(bool enable);

private:
    void timeStep();
    void enforceBounds();

private:
    bool _isReset;
    QPointF _lastMousePos;

    QSizeF _extent;
    QPointF _velocity;
    QPointF _scrollPos;

    QTime _mouseTime;
    QTime _slideTime;
};

#endif
