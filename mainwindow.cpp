#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QFileDialog>
#include <QQuickWidget>
#include <QQuickItem>
#include <QQmlEngine>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::btnClick);
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::sliderChenge);


    connect(ui->loadSheder, &QPushButton::clicked, this, &MainWindow::on_loadButton_clicked);
    // ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    // 1. Включаем растягивание содержимого QML под размеры виджета C++


    // 2. Делаем фон виджета прозрачным (убирает белый цвет)







    //=========================================================//



}


void MainWindow::on_loadButton_clicked() {
    // Открываем диалог выбора файла
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Открыть ISF шейдер"),
        "", // Начальная папка (можно указать путь к коллекции)
        tr("ISF Files (*.fs);;All Files (*)")
        );

    if (!filePath.isEmpty()) {
        // Вызываем метод загрузки у нашего виджета

        // Опционально: выведем путь в заголовок окна, чтобы знать что загружено
        this->setWindowTitle("ISF Player - " + filePath.section('/', -1));
    }
}


void MainWindow::buildDynamicUI(const QJsonArray &inputs) {
    // 1. Очищаем старые виджеты
    QLayoutItem *child;
    while ((child = ui->controlsLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    // 2. Создаем новые виджеты
    for (int i = 0; i < inputs.size(); ++i) {
        QJsonObject input = inputs[i].toObject();
        QString name = input["NAME"].toString();
        QString type = input["TYPE"].toString();
        QString labelText = input["LABEL"].toString().isEmpty() ? name : input["LABEL"].toString();



        if (type == "float") {
            QLabel *label = new QLabel(labelText, this);
            QSlider *slider = new QSlider(Qt::Horizontal, this);

            float min = input["MIN"].toDouble(0.0);
            float max = input["MAX"].toDouble(1.0);
            float def = input["DEFAULT"].toDouble(0.5);

            slider->setRange(0, 1000); // 1000 шагов для плавности
            slider->setValue((def - min) / (max - min) * 1000);

            connect(slider, &QSlider::valueChanged, [=](int val) {
                float realVal = min + (float(val) / 1000.0f) * (max - min);
            });
            // ui->dockWidget->widget()->layout()->addWidget(label);
            ui->controlsLayout->addWidget(label);
            ui->controlsLayout->addWidget(slider);
        }else if(type == "point2D"){
            QLabel *label = new QLabel(labelText, this);
            ui->controlsLayout->addWidget(label);

            // Создаем два горизонтальных слайдера
            QSlider *sliderX = new QSlider(Qt::Horizontal, this);
            QSlider *sliderY = new QSlider(Qt::Horizontal, this);

            sliderX->setRange(0, 1000);
            sliderY->setRange(0, 1000);

            // Получаем дефолтные значения (в JSON это массив [x, y])
            QJsonArray defArray = input["DEFAULT"].toArray();
            float defX = defArray.at(0).toDouble(0.5);
            float defY = defArray.at(1).toDouble(0.5);

            sliderX->setValue(defX * 1000);
            sliderY->setValue(defY * 1000);

            // Общая лямбда для обновления обоих значений сразу
            auto updatePoint = [=]() {
                float x = sliderX->value() / 1000.0f;
                float y = sliderY->value() / 1000.0f;
                // Передаем как вектор
                qDebug() << "call slider update :x " << x << "y: " << y << "name :" << name;

            };

            connect(sliderX, &QSlider::valueChanged, updatePoint);
            connect(sliderY, &QSlider::valueChanged, updatePoint);

            ui->controlsLayout->addWidget(new QLabel("X:"));
            ui->controlsLayout->addWidget(sliderX);
            ui->controlsLayout->addWidget(new QLabel("Y:"));
            ui->controlsLayout->addWidget(sliderY);
        }
        // Можно добавить чекбоксы для типа "bool"
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnClick()
{
    qDebug() << "button click";
}

void MainWindow::sliderChenge(int val)
{
    qDebug() << "slider value:" << val;
}
