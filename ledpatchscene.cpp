#include "ledpatchscene.h"
#include <QBrush>
#include <QPen>







// === Реализация Сцены Патча ===
LedPatchScene::LedPatchScene(QObject *parent) : QGraphicsScene(parent) {
    setSceneRect(0, 0, 1000, 1000);
    setBackgroundBrush(QBrush(QColor(30, 30, 30))); // Темно-серый фон редактора
}

void LedPatchScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // ЛКМ — ставим светодиод в цепочку
    if (event->button() == Qt::LeftButton) {
        qreal u = event->scenePos().x() / sceneRect().width();
        qreal v = event->scenePos().y() / sceneRect().height();





        int newId = ledNodes.size();
        LedNodeItem *newNode = new LedNodeItem(newId, currentPort, QPointF(u, v));
        newNode->setPos(event->scenePos());

        addItem(newNode);
        ledNodes.append(newNode);

        // Соединяем проводом только если прошлый диод на этом же порту
        if (lastClickedNode && lastClickedNode->portId == currentPort) {
            LedCableItem *cable = new LedCableItem(lastClickedNode, newNode);
            addItem(cable);
            ledCables.append(cable);
        }

        lastClickedNode = newNode;
        emit nodeCountChanged(ledNodes.size());
    }
    // ПКМ — обрываем связь, чтобы начать новую ветку/стрип
    else if (event->button() == Qt::RightButton) {
        lastClickedNode = nullptr;
    }

    QGraphicsScene::mousePressEvent(event);
}
