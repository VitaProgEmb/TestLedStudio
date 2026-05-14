#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "core.h"
#include "isfplayer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void btnClick();
    void sliderChenge(int val);
    void on_loadButton_clicked();
private:
    ISFPlayer *m_player{nullptr};
    Core *m_core;
    void buildDynamicUI(const QJsonArray &inputs);
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
