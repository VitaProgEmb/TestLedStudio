#pragma once

#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/FlatGL.h>

class LedPatchScene {
public:
    LedPatchScene();
    ~LedPatchScene();

    void viewportChanged(int width, int height);
    void draw();

private:
    Magnum::GL::Mesh _mesh;
    Magnum::Shaders::FlatGL2D _shader;
};
