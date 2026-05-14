#pragma once
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/FlatGL.h>

class LedPatchScene {
public:
    LedPatchScene();  // Конструктор
    ~LedPatchScene(); // Деструктор

    void viewportChanged(int width, int height);
    void draw();

private:
    // Пример объектов Magnum, которые вы будете инициализировать в .cpp
    Magnum::GL::Mesh _mesh;
    Magnum::Shaders::FlatGL2D _shader;
};
