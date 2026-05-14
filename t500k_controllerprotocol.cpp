#include "t500k_controllerprotocol.h"

T500K_ControllerProtocol::T500K_ControllerProtocol(QObject *parent)
    : QObject{parent}
{
    m_udpSocket = new QUdpSocket(this);
    // m_udpSocket->writeDatagram();
}

void T500K_ControllerProtocol::sendToLed(int port, QByteArray &rgbData)
{
    T500K_Header header;
    header.portId =  static_cast<uint8_t>(port);
    header.dataLen = static_cast<uint16_t>(rgbData.size());
    QByteArray packet;
    packet.append(reinterpret_cast<const char *>(&header), sizeof(header));
    packet.append(rgbData);
    m_udpSocket->writeDatagram(packet, QHostAddress("192.168.0.10"),6144);
}
