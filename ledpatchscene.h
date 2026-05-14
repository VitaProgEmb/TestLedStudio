#pragma once

#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/FlatGL.h>
#include <Magnum/Timeline.h> // <-- Исправили путь к таймлайну Magnum

class LedPatchScene {
public:
    LedPatchScene();
    ~LedPatchScene();

    void viewportChanged(int width, int height);
    void draw();

    void handleMouseClick(float x, float y);

private:
    Magnum::GL::Mesh _mesh;
    Magnum::Shaders::FlatGL2D _shader;

    Magnum::Timeline _timeline; // <-- Исправили тип на Magnum::Timeline

    const int _rows = 8;
    const int _cols = 8;
    const float _ledSize = 0.05f;
    const float _spacing = 0.07f;
};
