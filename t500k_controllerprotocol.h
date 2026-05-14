#ifndef T5__K_CONTROLLERPROTOCOL_H
#define T5__K_CONTROLLERPROTOCOL_H

#include <QObject>
#include <QUdpSocket>


class T500K_ControllerProtocol : public QObject
{
    Q_OBJECT
private:

#pragma pack(push, 1) // Отключаем выравнивание байтов
    struct T500K_Header {
        uint8_t sync = 0x55;      // Стартовый байт
        uint8_t magic = 0xAA;     // Тип протокола
        uint8_t controllerId = 1; // Номер контроллера (DIP-свитч)
        uint8_t portId = 0;       // Номер выхода (0-7)
        uint16_t dataLen;         // Длина RGB данных (Big-Endian)
        uint8_t command = 0x00;   // Обычно 0
    };

#pragma pack(pop)
    QUdpSocket *m_udpSocket{nullptr};
public:
    explicit T500K_ControllerProtocol(QObject *parent = nullptr);
    void sendToLed(int port, QByteArray &rgbData);

signals:
};

#endif // T5__K_CONTROLLERPROTOCOL_H
