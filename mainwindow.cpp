#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "magnumwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Просто создаем виджет как обычную кнопку или текстовое поле
    MagnumWidget* magnumWidget = new MagnumWidget(this);

    // Добавляем его в ваш слой на форме
    ui->verticalLayout->addWidget(magnumWidget);
}

MainWindow::~MainWindow() {
    delete ui;
}
