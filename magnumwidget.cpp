#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/OpenGL.h>
#include <Magnum/Platform/GLContext.h>

#include "magnumwidget.h"
#include "ledpatchscene.h"
#include <QMouseEvent>


MagnumWidget::MagnumWidget(QWidget* parent) : QOpenGLWidget(parent) {}

MagnumWidget::~MagnumWidget() {}

void MagnumWidget::initializeGL() {
    makeCurrent();
    if(!_initialized) {
        if(!Magnum::GL::Context::hasCurrent()) {
            static Magnum::Platform::GLContext context;
        }
        _scene = std::make_unique<LedPatchScene>();
        _scene->viewportChanged(width(), height());
        _initialized = true;
    }
}

void MagnumWidget::resizeGL(int w, int h) {
    if(_initialized && _scene) {
        makeCurrent();
        if(Magnum::GL::Context::hasCurrent()) {
            Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::EnterExternal);
        }
        _scene->viewportChanged(w, h);
    }
}

void MagnumWidget::paintGL() {
    if(_initialized && _scene) {
        makeCurrent();

        if(Magnum::GL::Context::hasCurrent()) {
            Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::EnterExternal);
        }

        // Передаем ID текущего буфера Qt ( defaultFramebufferObject() ) в метод draw сцены!
        _scene->draw(defaultFramebufferObject());

        if(Magnum::GL::Context::hasCurrent()) {
            Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::ExitExternal);
        }
    }
    update();
}




void MagnumWidget::mousePressEvent(QMouseEvent* event) {
    if(_initialized && _scene && event->button() == Qt::LeftButton) {
        float x = (2.0f * event->position().x() / width()) - 1.0f;
        float y = 1.0f - (2.0f * event->position().y() / height());
        _scene->handleMouseClick(x, y);

        // КРИТИЧЕСКИ ВАЖНО: принудительно обновляем виджет Qt
        update();
    }
}

