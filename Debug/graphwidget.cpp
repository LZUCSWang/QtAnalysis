#include "edge.h"
#include "node.h"

#include <math.h>
#include "graphwidget.h"
#include <QKeyEvent>
//#include <QRandomGenerator>
#include <QDebug>
#include <QErrorMessage>
//! [0]
extern QString showName;
GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    gScene = new QGraphicsScene(this);
    gScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    gScene->setSceneRect(-4000, -4000, 4000, 4000);
    setScene(gScene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Elastic Nodes"));

    //    this->installEventFilter(this);
}
//! [1]

//! [2]
void GraphWidget::init(QVariantList&dataList){

    qDebug()<<"initing graph";
    if (dataList.empty()){
        QErrorMessage* errordlg = new QErrorMessage(this);
        errordlg->setWindowTitle("错误对话框");
        errordlg->showMessage("你还没有导入文件！");
        return;
    }

    gScene->clear();

    int nums;

    nums=dataList.size();
    for (int i=1;i<nums;i++){
        QVariantList row=dataList[i].toList();
        QString from,to;
        from=row[1].toString();
        to=row[0].toString();
        if (!map.contains(from)){map[from]=new Node(this);map[from]->name=from;map[from]->setPos(-1000,-1000);gScene->addItem(map[from]);}
        if (!map.contains(to)){map[to]=new Node(this);map[to]->name=to;map[to]->setPos(-1000,-1000);gScene->addItem(map[to]);}
        gScene->addItem(new Edge(map[from], map[to]));
        map[from]->cnt++;
        map[to]->cnt++;}
    shuffle();
}
void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}
//! [2]

//! [3]
void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        centerNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        centerNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        centerNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        centerNode->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        shuffle();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}
//! [3]

//! [4]
void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QVector<Node *> nodes;
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item : items) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    for (Node *node : qAsConst(nodes))
        node->calculateForces();

    bool itemsMoved = false;
    for (Node *node : qAsConst(nodes)) {
        if (node->advancePosition())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}
//! [4]

#if QT_CONFIG(wheelevent)
//! [5]
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow(2., -event->angleDelta().y() / 240.0));
}
//! [5]
#endif

//! [6]
void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
}
//! [6]

//! [7]
void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}
//! [7]

void GraphWidget::shuffle()
{
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item : items) {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);    }
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
void GraphWidget::receiveColor(QColor color){
    nodeColor=color;
    //    emit sendNodeColor(color);
}
void GraphWidget::receiveData(QVariantList&datalist){
    qDebug()<<"receiveData";
    init(datalist);
}
void GraphWidget::setCenter(){
    //    map[showName]->setPos(0,0);
}
