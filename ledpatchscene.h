#pragma once

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include "ledpatch.h"


// Класс Интерактивной Сцены Патча
class LedPatchScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit LedPatchScene(QObject *parent = nullptr);

    QList<LedNodeItem*> ledNodes;
    QList<LedCableItem*> ledCables;

    int currentPort = 0; // Текущий порт для новых диодов

signals:
    void nodeCountChanged(int count); // Сигнал для вывода статистики в UI

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    LedNodeItem *lastClickedNode = nullptr;
};
