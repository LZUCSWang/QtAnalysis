
#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QMap>
#include <QMainWindow>
class Node;
class GraphWidget : public QGraphicsView
{
    Q_OBJECT
public:
    GraphWidget(QWidget *parent = nullptr);
    QColor nodeColor;
    void itemMoved();
    void init(QVariantList&);
public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();
    void receiveColor(QColor);
    void receiveData(QVariantList&);
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void scaleView(qreal scaleFactor);
private slots:
    void setCenter();
private:
    QGraphicsScene *gScene;
    int timerId = 0;
    Node *centerNode;
    QMap<QString,Node*>map;
};
#endif
