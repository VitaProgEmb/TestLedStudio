#pragma once

#include <QtGui/QWindow>

class MagnumWidget: public QWindow {
    Q_OBJECT

public:
    explicit MagnumWidget(QWindow* parent = nullptr);
    ~MagnumWidget() override;

protected:
    void exposeEvent(QExposeEvent* event) override;

private:
    void render();
};
