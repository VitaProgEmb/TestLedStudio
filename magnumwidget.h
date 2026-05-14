#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>

class MagnumWidget: public QOpenGLWidget, protected QOpenGLFunctions {
protected:
    void initializeGL() override {
        initializeOpenGLFunctions();

        // Передаем управление контекстом OpenGL движку Magnum
        if(!Magnum::GL::Context::hasCurrent()) {
            Magnum::GL::Context::makeCurrent(new Magnum::GL::Context{});
        }
    }

    void resizeGL(int w, int h) override {
        // Обновляем размер области отрисовки в Magnum при изменении окна Qt
        Magnum::GL::defaultFramebuffer.setViewport({{}, {w, h}});
    }

    void paintGL() override {
        // Очищаем экран средствами Magnum
        Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);

        // ТУТ ПИШЕТСЯ ВАШ КОД ОТРИСОВКИ НА MAGNUM ENGINE
    }
};
