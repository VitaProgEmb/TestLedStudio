#pragma once

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QList>

// Светодиод (Нода) — ТУТ ТОЛЬКО ОБЪЯВЛЕНИЕ
class LedNodeItem : public QGraphicsEllipseItem {
public:
    int id;
    int portId;
    QPointF uvCoord;

    LedNodeItem(int id, int port, QPointF uv, QGraphicsItem *parent = nullptr);
};

// Провод (Кабель) — ТУТ ТОЛЬКО ОБЪЯВЛЕНИЕ
class LedCableItem : public QGraphicsLineItem {
public:
    LedNodeItem *fromNode;
    LedNodeItem *toNode;

    LedCableItem(LedNodeItem *from, LedNodeItem *to, QGraphicsItem *parent = nullptr);
    void updatePosition();
};

// Интерактивная Сцена Патча
class LedPatchScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit LedPatchScene(QObject *parent = nullptr);

    QList<LedNodeItem*> ledNodes;
    QList<LedCableItem*> ledCables;
    int currentPort = 0;

signals:
    void nodeCountChanged(int count);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    LedNodeItem *lastClickedNode = nullptr;
};
