#include "ledpatch.h"
#include <QBrush>
#include <QPen>

// === Конструктор Светодиода ===
LedNodeItem::LedNodeItem(int id, int port, QPointF uv, QGraphicsItem *parent)
    : QGraphicsEllipseItem(-7, -7, 14, 14, parent) // Задаем размер круга
{
    this->id = id;
    this->portId = port;
    this->uvCoord = uv;

    // Настройки интерактивности Qt Quick/Graphics
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setBrush(Qt::black);
    setPen(QPen(Qt::white, 1));
}

// === Конструктор Кабеля ===
LedCableItem::LedCableItem(LedNodeItem *from, LedNodeItem *to, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    this->fromNode = from;
    this->toNode = to;

    setPen(QPen(QColor(150, 150, 150, 150), 2, Qt::DashLine));
    setZValue(-1); // Провод под точками диодов
    updatePosition();
}

// Метод обновления геометрии кабеля
void LedCableItem::updatePosition() {
    if (fromNode && toNode) {
        setLine(QLineF(fromNode->scenePos(), toNode->scenePos()));
    }
}

// === Конструктор Сцены ===
LedPatchScene::LedPatchScene(QObject *parent) : QGraphicsScene(parent) {
    setSceneRect(0, 0, 1000, 1000);
    setBackgroundBrush(QBrush(QColor(30, 30, 30))); // Темно-серый фон
}

// Обработка кликов мыши на сцене
void LedPatchScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        qreal u = event->scenePos().x() / sceneRect().width();
        qreal v = event->scenePos().y() / sceneRect().height();

        int newId = ledNodes.size();
        LedNodeItem *newNode = new LedNodeItem(newId, currentPort, QPointF(u, v));
        newNode->setPos(event->scenePos());

        addItem(newNode);
        ledNodes.append(newNode);

        if (lastClickedNode && lastClickedNode->portId == currentPort) {
            LedCableItem *cable = new LedCableItem(lastClickedNode, newNode);
            addItem(cable);
            ledCables.append(cable);
        }
        //=================
        lastClickedNode = newNode;
        emit nodeCountChanged(ledNodes.size());
    }
    else if (event->button() == Qt::RightButton) {
        lastClickedNode = nullptr; // Обрыв линии по ПКМ
    }

    QGraphicsScene::mousePressEvent(event);
}
