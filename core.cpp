#include "core.h"
#include <QDebug>
#include <QMessageBox>
/**
 * @brief Core::Core
 * @param parent
 */
Core::Core(QObject *parent)
    : QObject{parent}
{

    m_ledDiscovery = new LedDiscovery;
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Core::timeOut);
    m_timer->setTimerType(Qt::CoarseTimer);
    // m_timer->start(3000);

}

/**
 * @brief Core::sendDiscoverySignal
 */
void Core::sendDiscoverySignal()
{
    m_ledDiscovery->sendDiscoveryRequest();
}


/**
 * @brief Core::timeOut
 */
void Core::timeOut()
{
    static int cnt =0;
    qDebug() << "timeout:" << cnt;
    cnt ++;
}
