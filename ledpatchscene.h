#pragma once

#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/FlatGL.h>
#include <Magnum/Timeline.h>
#include <Magnum/Math/Vector2.h> // Подключаем векторы координат Magnum
#include <vector>

class LedPatchScene {
public:
    LedPatchScene();
    ~LedPatchScene();

    void viewportChanged(int width, int height);
    void draw();

    // Сюда будут прилетать точные координаты клика мыши
    void handleMouseClick(float x, float y);

private:
    Magnum::GL::Mesh _mesh;
    Magnum::Shaders::FlatGL2D _shader;
    Magnum::Timeline _timeline;

    // Фиксированный размер для каждого устанавливаемого светодиода
    const float _ledSize = 0.04f;

    // Динамический массив, который хранит координаты ВСЕХ установленных светодиодов
    std::vector<Magnum::Vector2> _placedLeds;
};
