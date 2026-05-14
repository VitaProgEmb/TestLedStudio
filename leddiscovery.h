#ifndef LEDDISCOVERY_H
#define LEDDISCOVERY_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>

class LedDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit LedDiscovery(QObject *parent = nullptr);

    // Запуск полива сети
    void sendDiscoveryRequest();

signals:
    // Сигнал, который прилетит в главное окно, когда найдется новый контроллер
    void deviceFound(QString ip, QString name);

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket *udpSocket;
    const quint16 PORT = 12345; // Порт, который слушают твои LED-железки
};

#endif // LEDDISCOVERY_H
