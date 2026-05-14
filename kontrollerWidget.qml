import QtQuick
import QtQuick.Controls

Item {
    id: rootItem
    // Ширину больше не фиксируем — она будет приходить из C++ (докера)
    height: isExpanded ? 420 : 220

    property bool isExpanded: true

    Behavior on height {
        NumberAnimation { duration: 350; easing.type: Easing.InOutQuad }
    }

    // Визуальная карточка (теперь она адаптируется под ширину виджета)
    Rectangle {
        id: controllerCard
        anchors.fill: parent
        anchors.margins: 5 // Небольшой отступ от краев докера
        color: "#1a1c1e"   // Цвет самой карточки
        radius: 16
        border.color: "#3a3d40"
        border.width: 2
        clip: true

        // ==========================================
        // 1. ВЕРХНЯЯ ЧАСТЬ (Шапка контроллера)
        // ==========================================
        Item {
            id: topHeader
            width: parent.width
            height: 200
            anchors.top: parent.top

            Text {
                id: titleText
                text: "CAST NODE 16CH"
                color: "#ffffff"
                font.pixelSize: 26
                font.bold: true
                anchors { left: parent.left; top: parent.top; margins: 25 }
            }

            Row {
                anchors { right: parent.right; top: parent.top; margins: 25 }
                spacing: 8
                Text {
                    text: "STATUS\nONLINE"
                    color: "#a0a5aa"
                    font.pixelSize: 11
                    font.bold: true
                    horizontalAlignment: Text.AlignRight
                }
                Rectangle {
                    width: 18; height: 18; color: "#00ff33"; radius: 9
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Row {
                anchors { left: parent.left; bottom: toggleButton.top; leftMargin: 25; bottomMargin: 15 }
                spacing: 30
                Column {
                    spacing: 4
                    Text { text: "💾 SD CARD"; color: "#ffffff"; font.pixelSize: 12; font.bold: true }
                    Text { text: "1Gb / 16Gb\n15Gb free"; color: "#80858a"; font.pixelSize: 11 }
                }
                Column {
                    spacing: 4
                    Text { text: "📶 WiFi"; color: "#3399ff"; font.pixelSize: 12; font.bold: true }
                    Text { text: "-63dB"; color: "#80858a"; font.pixelSize: 11 }
                }
            }

            Button {
                id: toggleButton
                width: 160; height: 32
                anchors { bottom: parent.bottom; horizontalCenter: parent.horizontalCenter; bottomMargin: 10 }
                text: rootItem.isExpanded ? "🔽 Скрыть порты" : "🔼 Показать порты"

                background: Rectangle {
                    color: toggleButton.down ? "#3a3e42" : (toggleButton.hovered ? "#2d3135" : "#222528")
                    border.color: "#4a4e52"; border.width: 1; radius: 6
                }
                contentItem: Text {
                    text: toggleButton.text; font.pixelSize: 12; font.bold: true; color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                }
                onClicked: rootItem.isExpanded = !rootItem.isExpanded
            }
        }

        // ==========================================
        // 2. НИЖНЯЯ ЧАСТЬ (Сетка портов)
        // ==========================================
        Grid {
            id: portsGrid
            columns: 8
            spacing: 8
            // Центрируем сетку портов по горизонтали внутри карточки
            anchors {
                top: topHeader.bottom
                horizontalCenter: parent.horizontalCenter
                topMargin: 10
            }

            opacity: rootItem.isExpanded ? 1.0 : 0.0
            Behavior on opacity { NumberAnimation { duration: 200 } }

            Repeater {
                model: 16
                Rectangle {
                    width: 46; height: 46
                    color: index == 3 ? "#132542" : (index < 5 ? "#112e11" : "#25282b")
                    border.color: index == 3 ? "#3399ff" : (index < 5 ? "#00ff33" : "#4a4e52")
                    border.width: 1; radius: 6

                    Column {
                        anchors.centerIn: parent; spacing: 2
                        Text { text: "PORT " + (index + 1); color: "#a0a5aa"; font.pixelSize: 8; font.bold: true; anchors.horizontalCenter: parent.horizontalCenter }
                        Text { text: index == 3 ? "WS2801" : (index < 5 ? "WS2812B" : "EMPTY"); color: "#ffffff"; font.pixelSize: 7; anchors.horizontalCenter: parent.horizontalCenter }
                    }
                }
            }
        }
    }
}
