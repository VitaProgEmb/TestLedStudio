#pragma once

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <memory>

class LedPatchScene;

class MagnumWidget : public QOpenGLWidget {
    Q_OBJECT

public:
    explicit MagnumWidget(QWidget* parent = nullptr);
    ~MagnumWidget() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    std::unique_ptr<LedPatchScene> _scene;
    bool _initialized{false};
};
