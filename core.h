#ifndef CORE_H
#define CORE_H

#include <QObject>
#include "leddiscovery.h"
#include <QTimer>

class Core : public QObject
{
    Q_OBJECT
private:
    LedDiscovery *m_ledDiscovery{nullptr};
    QTimer *m_timer{nullptr};
public slots:
    void sendDiscoverySignal();
private slots:
    void timeOut();
public:
    explicit Core(QObject *parent = nullptr);
signals:
};

#endif // CORE_H
