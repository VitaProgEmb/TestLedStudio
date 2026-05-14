#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>

class LedPatchScene {
public:
    LedPatchScene() {
        // Конструктор: тут настраиваем базовые параметры OpenGL через Magnum
        Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
    }

    void viewportChanged(int width, int height) {
        // Изменение размеров: обновляем область вывода Magnum
        Magnum::GL::defaultFramebuffer.setViewport({{}, {width, height}});
    }

    void draw() {
        // Отрисовка: очищаем экран и рисуем ваши LED-панели/сцену
        Magnum::GL::defaultFramebuffer.clear(
            Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth
            );

        // ТУТ БУДЕТ ВАШ КОД ОТРИСОВКИ (Отрендерить сетку, диоды, патчи и т.д.)
    }
};
