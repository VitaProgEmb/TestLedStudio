#include "magnumwidget.h"

// 1. Подключаем базовые заголовки Magnum
#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/OpenGL.h>

// 2. Подключаем открытый не абстрактный мост контекста платформы
#include <Magnum/Platform/GLContext.h>

MagnumWidget::MagnumWidget(QWindow* parent) : QWindow(parent) {
    setSurfaceType(QWindow::OpenGLSurface);
}

MagnumWidget::~MagnumWidget() {}

void MagnumWidget::exposeEvent(QExposeEvent* event) {
    Q_UNUSED(event);
    if (isExposed()) {
        render();
    }
}

void MagnumWidget::render() {
    if(!Magnum::GL::Context::hasCurrent()) {
        // Конструктор GLContext полностью открыт (public).
        // Он не абстрактный, у него нет параметров.
        // Он берет текущий активный OpenGL-контекст от QWindow,
        // безопасно инициализирует Magnum и подгружает функции flextGLInit.
        static Magnum::Platform::GLContext context;
    }

    // Очищаем экран средствами Magnum (окно станет черным/темно-серым)
    Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);

    requestUpdate();
}
