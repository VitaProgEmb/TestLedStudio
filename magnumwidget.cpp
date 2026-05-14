// 1. СТРОГО ПЕРВЫМИ подключаем Magnum
#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/OpenGL.h>
#include <Magnum/Platform/GLContext.h>

// 2. Только потом подключаем наш заголовок и сцену
#include "magnumwidget.h"
#include "ledpatchscene.h"

MagnumWidget::MagnumWidget(QWidget* parent) : QOpenGLWidget(parent) {}

MagnumWidget::~MagnumWidget() {}

void MagnumWidget::initializeGL() {
    // Делаем текущий контекст Qt активным
    makeCurrent();

    if(!_initialized) {
        if(!Magnum::GL::Context::hasCurrent()) {
            // Инициализируем мост Magnum поверх контекста Qt
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
        // Сбрасываем кэш состояний, так как Qt мог изменить параметры OpenGL
        if(Magnum::GL::Context::hasCurrent()) {
            Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::EnterExternal);
        }
        _scene->viewportChanged(w, h);
    }
}

void MagnumWidget::paintGL() {
    if(_initialized && _scene) {
        makeCurrent();

        // КРИТИЧЕСКИ ВАЖНО: говорим Magnum, что он входит во внешний контекст Qt.
        // Это заставит движок принудительно обновить внутренние указатели на буфер кадра.
        if(Magnum::GL::Context::hasCurrent()) {
            Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::EnterExternal);
        }

        _scene->draw();

        // Говорим Magnum, что мы возвращаем управление обратно системе Qt
        Magnum::GL::Context::current().resetState(Magnum::GL::Context::State::ExitExternal);
    }
    update();
}
