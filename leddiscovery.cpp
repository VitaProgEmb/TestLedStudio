#include "leddiscovery.h"

LedDiscovery::LedDiscovery(QObject *parent) : QObject(parent) {
    udpSocket = new QUdpSocket(this);

    // Привязываемся к порту. ShareAddress позволяет не конфликтовать с другими прогами
    udpSocket->bind(QHostAddress::AnyIPv4, PORT, QUdpSocket::ShareAddress);

    connect(udpSocket, &QUdpSocket::readyRead, this, &LedDiscovery::processPendingDatagrams);
}

void LedDiscovery::sendDiscoveryRequest() {
    QByteArray data = "DISCOVER_LED_DEVICES"; // Твоя секретная фраза для контроллеров
    udpSocket->writeDatagram(data, QHostAddress::Broadcast, PORT);
}

void LedDiscovery::processPendingDatagrams() {
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        QString rawData = QString::fromUtf8(datagram.data());

        // Предположим, контроллер отвечает: "LED_CONF:MyRoomLight"
        if (rawData.startsWith("LED_CONF:")) {
            QString name = rawData.mid(9);
            QString ip = datagram.senderAddress().toString();

            emit deviceFound(ip, name);
        }
    }
}
