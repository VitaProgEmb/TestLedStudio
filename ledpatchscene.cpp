#include "ledpatchscene.h"

// Подключаем необходимые модули Magnum
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Primitives/Square.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/MeshTools/Compile.h>

#include <Magnum/Math/Matrix3.h>

using namespace Magnum::Math::Literals;

LedPatchScene::LedPatchScene() {
    // 1. Включаем базовые функции OpenGL (сглаживание и тест глубины)
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);

    // 2. Создаем геометрию простого квадрата (это будет основа нашей LED панели)
    Magnum::Trade::MeshData squareData = Magnum::Primitives::squareSolid();

    // 3. Компилируем геометрию в Mesh, который понимает видеокарта
    _mesh = Magnum::MeshTools::compile(squareData);
}

LedPatchScene::~LedPatchScene() {}

void LedPatchScene::viewportChanged(int width, int height) {
    // Обновляем область вывода при изменении размеров окна
    Magnum::GL::defaultFramebuffer.setViewport({{}, {width, height}});
}

void LedPatchScene::draw() {
    // 1. Очищаем экран фоновым цветом
    Magnum::GL::Renderer::setClearColor(0x1a1a1a_rgbf);
    Magnum::GL::defaultFramebuffer.clear(
        Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth
        );

    // 2. Параметры нашей светодиодной матрицы
    const int rows = 8;        // Количество строк
    const int cols = 8;        // Количество столбцов

    const float ledSize = 0.05f;  // Размер одного светодиода (уменьшили)
    const float spacing = 0.07f;  // Шаг сетки (размер диода + зазор между ними)

    // Вычисляем стартовую точку (левый нижний угол), чтобы матрица была строго по центру экрана
    float startX = -((cols - 1) * spacing) / 2.0f;
    float startY = -((rows - 1) * spacing) / 2.0f;

    // 3. Двойной цикл для отрисовки сетки светодиодов
    for(int r = 0; r < rows; ++r) {
        for(int c = 0; c < cols; ++c) {
            // Вычисляем позицию текущего диода на экране
            float posX = startX + (c * spacing);
            float posY = startY + (r * spacing);

            // Строим матрицу трансформации: сначала масштабируем примитив, затем сдвигаем в координаты posX, posY
            Magnum::Matrix3 transformation =
                Magnum::Matrix3::translation({posX, posY}) *
                Magnum::Matrix3::scaling(Magnum::Vector2{ledSize});

            // Передаем матрицу положения текущего диода в шейдер
            _shader.setTransformationProjectionMatrix(transformation);

            // Делаем чередование цветов для теста (например, шахматный порядок или просто один цвет)
            if((r + c) % 2 == 0) {
                _shader.setColor(0x00ff00_rgbf); // Ярко-зеленый
            } else {
                _shader.setColor(0x00aa00_rgbf); // Темно-зеленый (для эффекта разделения диодов)
            }

            // Команда видеокарте отрисовать этот конкретный диод
            _shader.draw(_mesh);
        }
    }
}

